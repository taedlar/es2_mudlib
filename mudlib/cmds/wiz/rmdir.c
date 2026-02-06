

#include <command.h>

inherit F_CLEAN_UP;

int help(object me);

int main(object me, string arg)
{
    string dir;

    SECURED_WIZARD_COMMAND;

    if(!arg) return help(me);

    dir = resolve_path(me->query("cwd"), arg);

    seteuid(geteuid(this_player(1)));
    if( rmdir(dir) )
        write("Ok.\n");
    else
        write("刪除失敗。\n");
    return 1;    
}

int help(object me)
{
    write(@HELP
指令格式 : rmdir <子目錄名>

刪除一個子目錄。
HELP
    );
    return 1;
}

