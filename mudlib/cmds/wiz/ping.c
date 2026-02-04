// vim: set ts=4 sw=4 syntax=lpc

#include <intermud.h>
#include <command.h>

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string addr;
    int port;

    SECURED_WIZARD_COMMAND;

    if( !arg || sscanf(arg, "%s %d", addr, port)!=2 )
	return notify_fail("指令格式：ping <IP 地址> <UDP 埠>\n");

    INTERMUD_SERVICE("ping")->send_request(addr, port);
    write("Ok.\n");
    return 1;
}

int help(object me)
{
    write(@HELP
指令格式：ping <IP 地址> <UDP 埠>

這個指令可以用來測試遠端的 MUD 是否執行中。

請注意第二個參數的埠號是 Intermud 的 UDP 埠號，不是 MUD 連線登入的
埠號。如果對方是 ES2-based 的 MUD 而你不知道他們的 UDP 埠，可以試
試看 ES2 mudlib 的預設值，也就是登入的埠號 +4。

相關說明：
	mudlist
HELP
    );
    return 1;
}

