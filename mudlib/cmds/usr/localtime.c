

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    int time;
    seteuid(getuid());
    time = time();
    printf("%i\n",time);
    printf("%s\n",ctime(time));
    write("現在是" + NATURE_D->game_time() + "。\n");
    return 1;
}

int help(object me)
{
     write(@HELP
指令格式: time

這個指令讓你(妳)知道現在的時辰。

HELP
    );
    return 1;
}

