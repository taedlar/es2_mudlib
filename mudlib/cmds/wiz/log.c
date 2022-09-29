// log.c

inherit F_CLEAN_UP;

int main(object me, string msg)
{
	string *lines;

	if( !msg || msg=="" ) return 0;

	lines = explode(msg, "\n");
	for(int i=0; i<sizeof(lines); i++)
		lines[i] = "    " + lines[i];
	msg = implode(lines, "\n") + "\n";

	seteuid(geteuid(this_player(1)));
	write_file(me->query("cwd") + ".ChangeLog",
		"\n" + ctime(time()) + " By " + capitalize(getuid(me)) + "\n\n" + msg);
	write("Ok.\n");
	return 1;
}

int help(object me)
{
	write(@Help
指令格式: log <msg>

將所做過的改變記錄在該目錄下的 .ChangeLog 裡
如果所要超過的一行, 可用 to log 來記錄。
Help
	);
	return 1;
}
