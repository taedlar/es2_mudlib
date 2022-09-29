/*  possess.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <command.h>

inherit F_CLEAN_UP;

int
main(object me, string arg)
{
    object ob;

    SECURED_ARCH_COMMAND;

    if( !arg || me != this_player(1)) return 0;
    ob = present(arg, environment(me));
    if( !ob || !living(ob))
        return notify_fail("這裡沒有這個生物。\n");
    if( geteuid(ob) != geteuid(me) )
        return notify_fail( ob->name() + "的 euid 和你不合﹐無法進行附身。\n");
    if( ob->query_temp("body_ob") )
        return notify_fail( ob->name() + "已經被人附身過了。\n");
    message_vision("$N化成一道光芒鑽進$n體內。\n", me, ob);
    LOGIN_D->enter_world(me, ob, 1);

    return 1;
}

int help()
{
    write(@TEXT
指令格式﹕posssess <目標生物>

附身到另一個生物上﹐這個生物必須和你有相同的 EUID。
附身後用 quit 指令會回到原來的身體。
TEXT
    );
    return 1;
}
