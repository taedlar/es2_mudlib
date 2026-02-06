

#include <command.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    string src, dst, *dirs;

    SECURED_WIZARD_COMMAND; 

    seteuid(geteuid(this_player(1)));

    if (!arg || sscanf(arg, "%s %s", src, dst)!=2 ) return
        notify_fail("指令格式: cp <原檔名> <目標檔名> \n");

    src = resolve_path(me->query("cwd"), src);
    dst = resolve_path(me->query("cwd"), dst);

    if( file_size(dst)==-2 ) {
        dirs = explode(src, "/");
        dst += "/" + dirs[<1];
    }

    if( cp(src, dst) )
        write("Ok.\n");
    else
        write("你沒有足夠的讀寫權利。\n");
    return 1;
}
 
int help(object me)
{
    write(@HELP
指令格式 : cp <原檔名> <目標檔名>
 
此指令可讓你(妳)拷貝檔案。
HELP
    );
    return 1;
}
 

