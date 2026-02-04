// vim: set ts=4 sw=4 syntax=lpc

int main(object me, string arg)
{
    write("UID = " + getuid(this_player()) + "\n" );
    write("EUID = " + geteuid(this_player()) + "\n" );
    return 1;
}

int help (object me)
{
    write(@HELP
指令格式 : whoami 

顯示你目前的 UID 及 EUID。
HELP
    );
    return 1;
}

