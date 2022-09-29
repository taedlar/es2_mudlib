/*  update.c - compile/load an object

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <command.h>

inherit F_CLEAN_UP;

int update_player(object me);

int main(object me, string file)
{
    int i;
    object obj, *inv;
    string err;

    SECURED_WIZARD_COMMAND;

    seteuid(geteuid(me));

    if( !file ) file = me->query("cwf");
    if( !file ) return notify_fail("你要重新編譯什麼檔案﹖\n");

    // We check if the file string with file_size directly here is
    // important because the resolve_path may be buggy that never
    // return correct path, so we preserve the chance to update it
    // by typing its full-name here.

    if( file_size(file) < 0 )
    {
	switch(file)
	{
	case "here" : file = file_name(environment(me)); break;
	case "me" : return update_player(me);
	default:
	    if( (obj = present(file, environment(me)))
	    &&	interactive(obj) )
		return update_player(obj);
	}

	file = resolve_path(me->query("cwd"), file);

	if( file_size(file) < 0 && file[<2..<1] != ".c" ) file += ".c";
	if( file_size(file) < 0 )	/* -1: not exist, -2: directory */
	    return notify_fail("沒有 " + file + " 這個檔案。\n");
    }

    if( file==__FILE__ ) {
	write("下一次使用 update 指令將會重新編譯 update 指令。\n");
	destruct(this_object());
	return 1;
    }

    me->set("cwf", file);

    if (obj = find_object(file)) {
	if( obj==environment(me) ) {
	    if( file_name(obj)==VOID_OB )
		return notify_fail("你不能在 VOID_OB 裡重新編譯 VOID_OB。\n");
	    inv = all_inventory(obj);
	    i = sizeof(inv);
	    while(i--)
		if( userp(inv[i]) ) {
		    inv[i]->set_temp("last_location", file);
		    inv[i]->move(VOID_OB, 1);
		}
		else inv[i] = 0;
	}
	destruct(obj);
    }

    if (obj) return notify_fail("無法清除舊程式碼。\n");

    log_file("static/UPDATE",
        sprintf("%s(%s) 編譯 %s at %s\n",
                                me->name(1), geteuid(me), file,ctime(time()) ) );
    write("重新編譯 " + file + " ...\n");
    err = catch(obj = load_object(file));

    if(!err) {
	write("成功。\n");
	i = sizeof(inv);
	while(i--)
	    if( inv[i] && userp(inv[i]) ) inv[i]->move(obj, 1);
    } else
	write("編譯失敗﹕" + err);
                
    return 1;
}

int update_player(object me)
{
    object env, link_ob, obj;

    env = environment(me);

    link_ob = me->link();
    if( !link_ob ) {
	// If update a linkdead player, simply destruct it.
	me->save();
	seteuid(getuid());
	destruct(me);
	return 1;
    }

    seteuid(getuid());
    if( !(obj = LOGIN_D->make_body(link_ob)) )
	return notify_fail("無法複製新的身體物件﹗\n");

    exec(link_ob, me);

    // Make a careful check before copying body.
    if( (string)me->query_save_file() == (string)obj->query_save_file() ) {
	me->save();
	obj->restore();
    }
    seteuid(getuid());
    destruct(me);

    LOGIN_D->enter_world(link_ob, obj, 1);

    write("身體更新完畢。\n\n");
    obj->move(env);
    obj->write_prompt();

    return 1;
}

int help(object me)
{
  write(@HELP
指令格式：update <檔名|here|me|玩家名>
 
這個指令可以更新檔案, 並將新檔的內容載入記憶體內. 若目標為
'here' 則更新所在環境. 若目標為 'me' 則更新自己的人物. 若目
標為玩家則可更新玩家物件.
 
HELP
    );
    return 1;
}
