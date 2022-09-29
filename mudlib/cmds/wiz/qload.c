// qload.c

inherit F_CLEAN_UP;

int main()
{
    write(query_load_average() + "\n");
    return 1;
}

int help(object me)
{
write(@HELP
指令格式 : qload 

這個指令顯示出目前 CPU 的負擔, cmds/s 是每秒編譯幾個 cmds
comp lines/s 是每秒幾行。
HELP
    );
    return 1;
}
