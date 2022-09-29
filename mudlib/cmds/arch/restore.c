/*  restore.c

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

int
main(object me, string arg)
{
    string save_file;
    object ob;

    SECURED_ARCH_COMMAND;

    if( !arg ) return notify_fail("指令格式：restore <玩家>\n");
    seteuid(getuid());
    ob = new(USER_OB);
    seteuid(arg);
    export_uid(ob);
    seteuid(getuid());

#ifdef SAVE_USER
    save_file = ob->query_save_file();
    destruct(ob);
    if( cp(save_file+".backup", save_file) ) {
          write("備份回存完畢。\n");
          log_file("static/RESTORE",
              sprintf("%s restored by %s [%s]\n", arg, geteuid(this_player(1)), ctime(time())) );
    } else
        write("備份回存失敗！\n");
    return 1;
#else
    return notify_fail("系統目前不儲存使用者資料。\n");
#endif
}

int help(object me)
{
    write(@HELP
指令格式：restore <玩家>

取回一個玩家先前儲存的備份資料。
HELP
    );
    return 1;
}
