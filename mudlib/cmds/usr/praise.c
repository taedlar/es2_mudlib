/*  praise.c - praise command to give user reputation

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object ob;
    int level = me->query_level();
    string pro = me->query_class();

    seteuid(getuid());

    if( wizardp(me) ) return notify_fail("巫師不准讚揚別人。\n");

    // This prevent abuse of praise from new characters.
    if( level < 2 ) return notify_fail("你必須升到第二級之後，讚揚才有效果。\n");

    if( me->query("praise_done") )
        return notify_fail("你不久前才讚揚過別人，必須升級之後才能再讚揚他人。\n");

    if( !arg || arg=="" )
        return notify_fail("你要讚揚誰？\n");

    ob = find_player(arg);
    if( !ob ) return notify_fail("這個人不在線上。\n");
    if( wizardp(ob) )
        return notify_fail( ob->name() + "謝謝你的好意，不過請你讚揚你欣賞的玩家。\n");
    if( ob==me )
        return notify_fail("這個指令不是用來自誇的，你只能接受別人的讚揚。\n");

    log_file("praise", sprintf("[%s] %s praised %s at level %d.\n",
        ctime(time()), getuid(me), getuid(ob), level) );
    
    if( pro=="commoner")
        ob->gain_score("reputation", level-1);
    else
        ob->gain_score("reputation", (level-1) * (level-1) );

    me->gain_score("reputation", 1);
    if( pro!="commoner" && ob->query_class()==pro )
        CLASS_D(pro)->brother_praise(me, ob);
    me->set("praise_done", 1);

    shout( sprintf(YEL "%s對%s的讚揚獲得了許\多人的認同，並提高了%s的聲望！\n" NOR,
            me->name(), ob->name(), ob->name() ));
    write("Ok.\n");

    return 1;
}

int help()
{
    write(@TEXT
指令格式：praise <玩家>

這個指令讓你讚揚某位你欣賞的玩家，並提高對方的聲望，當你升到第二級之後，每
次升級可以提出你的讚揚一次，而接受別人的讚揚並沒有任何限制，如果讚揚你的玩
家和你具有相同職業，聲望提升的效果可能更好，或者一併增加一些專業的聲望。若
是本身仍屬平民身份，則由於本身在武林中未有足夠之聲望，聲望之提升效果較差。
TEXT
    );
    return 1;
}
