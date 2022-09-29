/*  time.c

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

int main(object me, string arg)
{
    seteuid(getuid());
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
