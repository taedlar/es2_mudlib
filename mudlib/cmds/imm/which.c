// which.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	string f;

	if( !arg ) return notify_fail("指令格式﹕which <指令>\n");

	if( f = me->find_command(arg) ) {
		write(f + "\n");
		return 1;	
	}
	write("你的指令路徑中沒有這個指令﹐請試試 localcmds 這個指令。\n");
	return 1;
}

int help(object me)
{
	write(@Help
指令格式: which <指令>

找出指令所在的目錄。

Help
	);
	return 1;
}
