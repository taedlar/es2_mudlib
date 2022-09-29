// say.c

#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object *ob;
	if (!arg) return notify_fail("你要說什麼﹖\n");

	write("你說道﹕" + HIG + arg + "\n" NOR);
	message("sound", me->name() + "說道﹕" + HIG + arg + "\n" NOR,
		environment(me), me);

	// The mudlib interface of say
	ob = all_inventory(environment(me)) - ({ me });
	ob->relay_say(me, arg);

	return 1;
}

int help (object me)
{
	write(@HELP
指令格式: say <訊息>
 
說話﹐所有跟你在同一個房間的人都會聽到你說的話。
 
註: 本指令可用 ' 取代.
 
HELP
	);
	return 1;
}
