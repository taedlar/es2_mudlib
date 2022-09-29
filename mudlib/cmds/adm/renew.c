// renew.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	string file;
	object ob;
	int cnt = 0;

	seteuid(getuid(this_player(1)));
	file = resolve_path(me->query("cwd"), arg);
	foreach(ob in objects( (: inherits, file :) )) {
		if( !userp(ob) ) destruct(ob);
		cnt++;
	}
	write("object renewed: " + cnt + "\n");
	return 1;
}
