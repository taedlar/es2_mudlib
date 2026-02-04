// vim: set ts=4 sw=4 syntax=lpc

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

