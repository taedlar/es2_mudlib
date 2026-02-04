// vim: set ts=4 sw=4 syntax=lpc

#include <command.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    write("目前工作目錄：" + (string)me->query("cwd") +  "\n");
    write("目前工作檔案：" + (string)me->query("cwf") +  "\n");
    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : pwd
 
這個指令會顯示出你目前的工作目錄(cwd)及工作檔案(cwf)，當你使用 ls 或其他
與檔案有關的指令時，會先搜尋目前的工作目錄。

工作檔案是你上一次使用 update 所更新的檔案。 
HELP
    );
    return 1;
}

