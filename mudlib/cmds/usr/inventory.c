/*  inventory.c

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
    int i;
    object *inv, ob;

    if( wizardp(me) && arg ) {
        ob = find_player(arg);
        if( !ob ) ob = find_living(arg);
        if( !ob ) ob = present(arg, environment(me));
        if( !ob ) return notify_fail("你要看誰的物品？\n");
    }
    else ob = me;

    inv = all_inventory(ob);
    if( !sizeof(inv) ) {
        write((ob==me)? "目前" + gender_self(me) + "身上沒有任何東西。\n"
            : ob->name() + "身上沒有攜帶任何東西。\n");
        return 1;
    }
    printf("%s身上帶著下列這些東西(負重 %d%%)﹕\n%s\n",
        (ob==me)? gender_self(me) : ob->name(),
        (int)ob->query_encumbrance() * 100 / (1+(int)ob->query_strength("carriage")),
        implode(map_array(inv, "inventory_desc", this_object()), "\n") );

    return 1;
}

string inventory_desc(object ob)
{
    string desc = ob->short();

    if( ob->query("keep_this")) desc += "◎";
//    else desc = "  " + desc;

    if( ob->query("equipped"))  desc = HIY "ˇ" + desc + NOR;
    else desc = "  " + desc;

    return desc;
}

int help (object me)
{
    write(@HELP
指令格式: inventory
 
可列出你(妳)目前身上所攜帶的所有物品。
ˇ  記號及亮黃色表示已在使用， ◎ 記號表示已 keep。
註 : 此指令可以 " i " 代替。
 
HELP
    );
    return 1;
}
