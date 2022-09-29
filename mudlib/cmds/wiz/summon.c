/*  summon.c

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

int main(object me, string str)
{
    object ob;
    int i;

    seteuid(getuid(me));
    if(!str) return notify_fail("指令格式: summon <使用者>\n");
    ob = LOGIN_D->find_body(str);
    if(!ob) return notify_fail("線上的使用者中沒有這個人。\n");

    // moving
    tell_room(environment(ob), "天空中伸出一隻大手把"+
	ob->name() + "抓了起來﹐然後不見了。\n", ob);
    tell_object(ob,"一隻手把你抓了起來﹐你眼前一陣黑 ....\n");
    ob->move(environment(me));
    tell_object(me, "你把" + ob->name() + "抓到你的面前。\n");
    tell_object(ob,".... 醒來時發現是" + me->name() +
	"把你弄過來的。\n");
    log_file("static/SUMMON",
	sprintf("%s(%s) summon %s(%s) 到 %s at  %s\n",
	me->name(1), geteuid(me), ob->name(1), geteuid(ob),file_name(environment(me)),ctime(time()) ) );
    tell_room(environment(ob), ob->name() + "突然出現在你眼前。\n",({me,ob}));

    // ok ..
    return 1;
}

int help(object me)
{
write(@HELP
指令格式 : summon <某人>

此指令可讓你(妳)將某人抓到你面前。
HELP
    );
    return 1;
}
