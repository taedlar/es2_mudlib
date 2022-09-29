// cat.c

#include "/doc/help.h"

inherit F_CLEAN_UP;
int help(object me);

int main(object me, string arg)
{
	string file, text;

	seteuid(geteuid(this_player(1)));

    if (!arg) return notify_fail("你想要顯示那個檔案?\n");
	file = resolve_path(me->query("cwd"), arg);
	if( file_size(file)<0 ) return notify_fail("沒有這個檔案。\n");
	if( !text = read_file(file) ) text = "";
	write(text);
	return 1;
}

int help(object me)
{
write(@HELP
指令格式 : cat <檔案>

此指令可讓你(妳)直接讀取某檔案之程式。
HELP
    );
    return 1;
}
