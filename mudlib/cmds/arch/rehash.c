// rehash.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
        seteuid(geteuid(this_player(1)));
	if( !arg )
		return notify_fail("指令格式﹕rehash <路徑>\n");

	if( arg[strlen(arg)-1]!='/' ) arg += "/";
	if(  file_size(arg)!=-2 )
		return notify_fail("沒有這個目錄﹗\n");

	write("更新指令表﹕" + arg + "\n");
	seteuid(getuid());
	COMMAND_D->rehash(arg);
	return 1;
}

int help(object me)
{
	write(@HELP
指令格式﹕rehash <目錄名稱>

這個指令會更新系統內部的指令表﹐如果你新增或刪除了某個目錄下的指令﹐必須用
這個指令更新該目錄才能使用。
HELP
	);
	return 1;
}
