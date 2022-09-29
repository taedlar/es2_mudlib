// disassemble.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	string target, file, f;
	object ob;

        seteuid(geteuid(this_player(1)));
	if( !arg || sscanf(arg, "%s %s", target, file) != 2 )
		return notify_fail("指令格式﹕disassemble <物件> <輸出檔名>\n");
	
	if( !ob ) ob = present(target, me);
	if( !ob ) ob = present(target, environment(me));
	if( !ob ) {
		f = resolve_path(me->query("cwd"), target);
		if( file_size(f + ".c") > 0 ) {
			f->foo();
			ob = find_object(f);
		}
	}

	if( !ob ) return notify_fail("你要反組譯哪一個物件﹖\n");

	f = resolve_path(me->query("cwd"), file);
	write("反組譯輸出至 " + f + "。\n");
	dump_prog(ob, 3, f);
	write("Ok.\n");

	return 1;
}
