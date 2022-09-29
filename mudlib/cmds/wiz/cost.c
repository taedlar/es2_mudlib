// cost.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	mapping before, after;
	int stime, usertime, eval_cost;

	if( me != this_player(1) ) me = this_player(1);

	seteuid(getuid());
   	if(!arg) return notify_fail("指令格式﹕cost <指令> [<參數> ....]\n");

	before = rusage();
	eval_cost = eval_cost();
	me->force_me(arg);
	eval_cost -= eval_cost();
	after = rusage();
	usertime = after["utime"] - before["utime"];
	stime = after["stime"] - before["stime"];
	printf("\n系統時間﹕%d 微秒\n使用者時間﹕%d 微秒\n執行指令數﹕%d\n",
		stime, usertime, eval_cost );
		
	return 1;
}

int help()
{
	write( @TEXT
指令格式﹕cost <指令> [<參數> .... ]

這個指令讓你測量另一個指令執行所需的時間。
TEXT
	);
	return 1 ;
}
