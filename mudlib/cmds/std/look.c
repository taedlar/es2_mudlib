/*  look.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

//#pragma optimize
#pragma save_binary

#include <room.h>
#include <ansi.h>

inherit F_CLEAN_UP;

int look_room(object me, object env);
int look_item(object me, object obj);
int look_living(object me, object obj);
int look_room_item(object me, string arg);

void create() {seteuid(getuid());}

int main(object me, string arg)
{
    object obj;
    int result;

    if( me->query_temp("block_msg/vision") )
        return notify_fail("你現在什麼也看不到﹗\n");

    if( arg ) {
        if( (obj = present(arg, me))
        ||    (obj = present(arg, environment(me)))) 
            if( obj->is_character() ) result = look_living(me, obj);
            else result = look_item(me, obj);
    }

    if( !result && environment(me) )
        return environment(me)->do_look(me, arg);
    return result;
}

int look_item(object me, object obj)
{
    mixed *inv;

    me->start_more(obj->long());
    inv = all_inventory(obj);
    if( sizeof(inv) ) {
	// if too many items, show a simple message -dragoon
	if( sizeof(inv) < 30 ) {
            inv = map_array(inv, "inventory_look", this_object() );
            message("vision", sprintf("裡面有﹕\n  %s\n",
                implode(inv, "\n  ") ), me); }
	else tell_object(me, YEL"裡面雜七雜八的放著許多東西..\n"NOR);
    }
    return 1;
}

int look_living(object me, object obj)
{
    string str, pro, race;
    mixed *inv;
    int age;

    if( me!=obj )
        message("vision", me->name() + "正盯著你看﹐不知道打些什麼主意。\n", obj);

    str = obj->long();

    pro = (obj==me) ? gender_self(obj->query("gender")) : gender_pronoun(obj->query("gender"));

    race = obj->query_race();
    if( (string)obj->query("humanoid") && intp(age = obj->query("age")) ) {
        if( race=="human" )
            str += break_chinese_string(
                sprintf("%s%s，%s看起來%s。",
                    obj->name(), RACE_D(race)->query_appearance(obj),
                    pro, (age > 10) ? ("約" + chinese_number(age / 10 * 10) + "多歲") : "不到十歲"),
                70 ) + "\n";
        else
            str += break_chinese_string(
                sprintf("%s屬於%s族，%s%s，%s看起來%s。",
                    obj->name(), to_chinese(race),
                    pro, RACE_D(race)->query_appearance(obj),
                    pro, (age > 10) ? ("約" + chinese_number(age / 10 * 10) + "多歲") : "不到十歲"),
                70 ) + "\n";
    }

    inv = all_inventory(obj);
    if( sizeof(inv) ) {
        inv = map_array(inv, "inventory_look", this_object(), obj->is_corpse()? 0 : 1 );
        inv -= ({ 0 });
        if( sizeof(inv) )
            str += sprintf( obj->is_corpse() ? "%s的遺物有﹕\n%s\n" : "%s身上帶著﹕\n%s\n",
                pro, implode(inv, "\n") );
    }

    message("vision", str, me);

    return 1;
}

string inventory_look(object obj, int flag)
{
    string str;

    str = obj->short();
    if( obj->query("equipped") )
        str = HIY "  ˇ" + str + NOR;
    else if( !flag )
        str = "    " + str;
    else
        return 0;

    return str;
}

int help (object me)
{
    write(@HELP
指令格式: look [<物品>|<生物>|<方向>]
 
這個指令讓你查看你所在的環境、某件物品、生物、或是方向。
 
HELP
);
    return 1;
}
