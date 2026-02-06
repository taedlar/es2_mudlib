

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


