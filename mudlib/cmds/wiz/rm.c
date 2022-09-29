/*  rm.c

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

int main(object me, string file)
{
    SECURED_WIZARD_COMMAND;

    seteuid(geteuid(this_player(1)));
    if (!file) return notify_fail("你要刪除那個檔案?\n");

    file = resolve_path(me->query("cwd"), file);
    if( file_size(file)==-1 )
        return notify_fail("沒有這個檔案。\n");

    if( rm(file) ) write("Ok.\n");
    else write("你沒有刪除這個檔案的權利。\n");

    return 1;
}

int help(object me)
{
  write(@HELP
指令格式 : rm <檔名>

此指令可讓你(妳)刪除有權修改的檔案。
HELP
    );
    return 1;
}
