// tail.c

inherit F_CLEAN_UP;

int help(object me);

int main(object me, string arg)
{
	string file;

	seteuid(geteuid(this_player(1)));
    if (!arg) return help(me);
	file = resolve_path(me->query("cwd"), arg);
	if( file_size(file)<0 ) return notify_fail("沒有這個檔案。\n");
	tail(file);
	return 1;
}

int help(object me)
{
write(@HELP
指令格式 : tail <檔案>

此指令可讓你(妳)直接讀取某檔案最末尾的幾行。
HELP
    );
    return 1;
}
