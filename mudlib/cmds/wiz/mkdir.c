/*  mkdir.c

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

int help(object me);

int main(object me, string arg)
{
    string dir;

    SECURED_WIZARD_COMMAND;

    seteuid(geteuid(this_player(1)));
    if(!arg) return help(me);

    dir = resolve_path(me->query("cwd"), arg);

    seteuid(geteuid(me));
    if( mkdir(dir) )
        write("Ok.\n");
    else
        write("你沒有在這裡建子目錄的權利。\n");
    return 1;    
}

int help(object me)
{
    write(@HELP
指令格式 : mkdir <子目錄名>

建立一個子目錄。
HELP
    );
    return 1;
}
