// vim: set ts=4 sw=4 syntax=lpc

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

