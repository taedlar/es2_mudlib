// reclaim.c

int main(object me, string arg)
{
	int mem;

	write("整理結果﹐共清除 " + reclaim_objects() + " 個變數。\n");
	return 1;
}

int help(object me)
{
write(@HELP
指令格式 : reclaim

用處 : 將存在於記憶體裡無用的變數清除掉, 以減少記憶體的使用。
HELP
     );
     return 1;
}
