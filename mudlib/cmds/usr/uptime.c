/*  uptime.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

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
