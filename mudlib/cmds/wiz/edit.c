

#include <command.h>

inherit F_CLEAN_UP;

int main(object me, string file)
{
    SECURED_WIZARD_COMMAND;

    if( !file ) return notify_fail("指令格式﹕edit <檔名>\n");

    if( in_edit(me) ) return notify_fail("你已經在使用編輯器了。\n");

    seteuid(geteuid(this_player(1)));
    ed(resolve_path(me->query("cwd"), file));
    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : edit <檔名>|edit here

利用此一指令可直接在線上編輯檔案。
HELP
    );
    return 1;
}

