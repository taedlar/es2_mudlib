// where.c
// 06-14-95

inherit F_CLEAN_UP;

int main(object me, string str)
{
	object ob, where, *ob_list;
	string msg="", dir_str;
	int i;

	if (!str) {
		ob_list = sort_array( users(), (:
			strcmp( environment($1) ? file_name(environment($1)) : "",
					environment($2) ? file_name(environment($2)) : "" )
		:) );
		msg = sprintf("%-12s %-12s %-3s %s\n", "姓名","ID","發呆","地點");
		foreach(ob in ob_list) {
			if( !environment(ob) || !ob->visible(this_player()) )
				continue;
			msg += sprintf("%-12s %-12s %-3d %s\n", ob->name(), ob->query_id(),
				(int)query_idle(ob)/60,
				file_name(environment(ob))
			);
		}
		me->start_more(msg);
		return 1;
	}

	ob = find_player(str);
	if( !ob ) ob = find_living(str);
	if( !ob ) ob = LOGIN_D->find_body(str);
	if( ob && ob->visible(me) ) {
		where = environment(ob);
		if (!where) return notify_fail("這個人不知道在那裡耶...\n");
		printf("%s(%s) 現在在 %s(%s), %s\n",
			ob->name(),
			ob->query("id"),
			where->query("short"),
			file_name(where),
			ob->link() ? ( living(ob) ? "發呆 " + query_idle(ob)/60 + " 分鐘" : "昏倒中") : (userp(ob) ? "斷線中":"NPC 類"), 
		);
		return 1;
	}

	// locate the children of object
	dir_str = resolve_path(me->query("cwd"), str);
	if( sizeof(ob_list = children(dir_str)) ) {
		for(i=0; i<sizeof(ob_list); i++) {
			if( !ob = environment(ob_list[i]) ) continue;
			msg += sprintf("%O\n", ob );
			// show until ob is room
			while( ob = environment(ob) )
				msg += sprintf("%O\n", ob );
			msg += "\n";
		}
		msg += "Object " + dir_str + ".c located.\n";
		this_player()->start_more(msg);
		return 1;
	}
	if (!ob) return notify_fail("現在沒 " + str + " 這個人物 (include ppl and npc), 也沒有 " + dir_str + ".c 的存在.\n");
}

int help(object me)
{
	write(@HELP
指令格式: where <參數>

參數可為:
無		列出所有 users 所在位置。
grain		找使用者 "grain" . 若無, 則列出全名為 "grain" 之 npc位置, 且會
		秀出 "發呆", "昏倒", "斷線", "NPC" 等狀態顯示。
kao yeu		列出全名 ( set_name 中第一個 string ), 為前述之 npc位置。
/obj/bottle	列出所有檔名為前述之物件位置, 且會一層層秀出, 直到所處環境為
		room 為止。
bottle		同上行, 但只搜尋當前目錄 (current directory)。
bottle.c	同上行。

HELP
	);
	return 1;
}
