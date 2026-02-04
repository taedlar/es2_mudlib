// vim: set ts=4 sw=4 syntax=lpc

inherit F_CLEAN_UP;

// This command is also called by LOGIN_D, so we don't want it use 
// arguments.
int main()
{
    seteuid(getuid());
    write("東方故事Ⅱ已經執行了" + CHINESE_D->chinese_period(uptime()) + "。\n");
    printf("現在時間 "+ctime(time())+"\n");
    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : uptime
 
這個指令告訴你東方故事 II 已經連續執行了多久。
及這台機器目前的時間。
 
HELP
    );
    return 1;
}

