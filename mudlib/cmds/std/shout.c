// shout.c

#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	if( !wizardp(me) )
		return notify_fail("目前暫不開放玩家 shout。\n");
	if (!arg) return notify_fail("你想要大叫什麼?\n");
	shout( me->name() + "縱聲長嘯﹕" + HIY + arg + NOR + "\n");
	write("你縱聲長嘯﹕" + HIY + arg + NOR + "\n");
	return 1;
}
int help(object me)
{
  write(@HELP
指令格式 : shout <訊息>
 
這個指令讓你將 <訊息> 傳送出去, 所有正在線上未昏的人都會聽見你的話。目前只
有巫師可用。
 
see also : tune
HELP
    );
    return 1;
}
 
