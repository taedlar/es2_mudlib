/*  reset_okip.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <login.h>
#include <command.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object ob, link_ob;
    string save_file;

    SECURED_ARCH_COMMAND;

    if( !arg ) return notify_fail("要幫那個玩家消除OKIP設定?\n");

    seteuid(ROOT_UID);
    ob = FINGER_D->acquire_login_ob(arg);
    if( !ob ) return notify_fail("沒有這個玩家...\n");
    ob->set("okip", "");
#ifdef SAVE_USER
    ob->save();
#endif
    log_file("static/OKIP", sprintf("[%s] %s reseted okip by %s\n",
    ctime(time())[0..15], ob->query("id"), me->query("id")));
    tell_object(me, "Ok.\n");
    return 1;
}

int help(object me)
{
    write(@HELP
指令格式：reset_okip <玩家>

取消一個玩家先前設定的OKIP資料。
HELP
    );
    return 1;
}

