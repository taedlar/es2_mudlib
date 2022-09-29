/*  snoop.c

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
#include <command.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object ob;

    SECURED_WIZARD_COMMAND;

    if( !arg ) {
        if( objectp(ob = query_snooping(me)) )
            write("你現在正在監聽" + ob->query("name") + "所收到的訊息。\n");
        return 1;
    } else if( arg=="none" ) {
        if( objectp(ob = query_snooping(me))
        &&    wiz_level(ob) >= wiz_level(me) )
            tell_object(ob, HIW + me->name(1) + "停止監聽你所收到的訊息。\n" NOR);
        snoop(me);
        write("Ok.\n");
        return 1;
    }

    ob = find_player(arg);
    if(!ob) ob = find_living(arg);
    if(!ob || !ob->visible(me)) return notify_fail("沒有這個人。\n");

    if( wizhood(me) != "(admin)"
    &&    wiz_level(me) <= wiz_level(ob) || wiz_level(me) < 3)
        return notify_fail("你沒有監聽" + ob->name() + "所收聽訊息的權利。\n");

    if( me==ob ) return notify_fail("請用 snoop none 解除監聽。\n");
        
    snoop(me, ob);
    write("你現在開始竊聽" + ob->name(1) + "所收到的訊息。\n");
    if( userp(ob) ) {
	log_file("SNOOP_PLAYER",
	    sprintf("[%s] %s snoops %O\n", ctime(time()), geteuid(me), ob));
	if( wizardp(ob) && wiz_level(me) < wiz_level("(arch)") )
	    tell_object(ob, HIW + me->name(1) + "開始監聽你所收到的訊息。\n" NOR);
    }

    return 1;
}

int help()
{
    write(@TEXT
指令格式﹕snoop <某人>|none

監聽其他使用者所收聽的訊息﹐snoop none 則取消監聽。
TEXT
    );
    return 1;
}
