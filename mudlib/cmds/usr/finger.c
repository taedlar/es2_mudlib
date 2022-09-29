/*  finger.c - inquiry other users' information

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

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    SECURED_COMMAND;

    if( !arg ) {
	if( !wizardp(me) )
	    return notify_fail("你要查詢哪位使用者的資料？\n");
	me->start_more( FINGER_D->finger_all() );
    } else
	write( FINGER_D->finger_user(arg) );

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式：finger [<使用者姓名>]
 
這個指令可顯示某個玩家的連線，權限等資料，如果你是巫師，則不指定使用者姓名
會顯示出所有正在線上之玩家的連線資料。
 
其他相關指令：who
HELP
    );
    return 1;
}
 
