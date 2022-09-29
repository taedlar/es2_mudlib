// home.c

#include "/doc/help.h"

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	string msg;

	seteuid(geteuid(this_player(1)));

	if( file_size(user_path(geteuid(me)) + "workroom.c") <= 0 ){
		message_vision("$N向眾人揮揮手, 回家去了。\n", me);
		me->move("/adm/guild/guildhall");
		message("tell_room", me->name() + "回到了自己的家中。\n", environment(me), me);
		return 1;
	}

	if( stringp(msg = me->query("env/msg_home")) )
		message_vision(msg + "\n", me);
	else
		message_vision("$N向眾人揮揮手, 回家去了。\n", me);
	me->move(user_path(geteuid(me)) + "workroom");
	message("tell_room", me->name() + "回到了自己的家中。\n", environment(me), me);
	return 1;
}

int help(object me)
{
  write(@HELP
指令格式 : home

利用此一指令可直接回到自己的工作室, 若 home directory 中無 workroom.c 檔, 
則回到 wiz guild 。 如果你 workroom.c 中有 'msg_home' 這個設定, 則在場的人
都會看到那個訊息。
HELP
    );
    return 1;
}
