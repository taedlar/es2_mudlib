/*  wizlock.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <command.h>

inherit F_CLEAN_UP;

int
main(object me, string arg)
{
    int lvl;

    SECURED_ARCH_COMMAND;

    if( !arg || arg=="" )
        return notify_fail("指令格式﹕wizlock <巫師等級>\n");

    if( sscanf(arg, "%d", lvl)!=1 ) lvl = wiz_level(arg);
    seteuid(getuid());
    if( LOGIN_D->set_wizlock(lvl) ) {
        write("Ok.\n");
        return 1;
    } else
        return notify_fail("上線權限設定失敗。\n");
}

int help (object me)
{
        write(@HELP
指令格式: wizlock <巫師等級>
 
限制某個等級以下的使用者進入遊戲。
其中巫師等級分別為 (immortal) (wizard) (arch) (admin), 若要取
消限制, 則輸入 (player).
 
HELP
);
        return 1;
}
 
