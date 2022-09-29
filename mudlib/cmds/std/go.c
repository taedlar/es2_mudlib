/*  go.c - character move command

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#pragma optimize
#pragma save_binary

inherit F_CLEAN_UP;

int do_go(object me, object dest, string dir);

mapping default_dirs = ([
    "north":		"北",
    "south":		"南",
    "east":		"東",
    "west":		"西",
    "northup":		"北邊",
    "southup":		"南邊",
    "eastup":		"東邊",
    "westup":		"西邊",
    "northdown":	"北邊",
    "southdown":	"南邊",
    "eastdown":		"東邊",
    "westdown":		"西邊",
    "northeast":	"東北",
    "northwest":	"西北",
    "southeast":	"東南",
    "southwest":	"西南",
    "up":		"上",
    "down":		"下",
    "out":		"外",
]);

void create()
{
    /* 因為 go 指令可能會載入其他物件，所以需要有 euid */
    seteuid(getuid());
}

int main(object me, string arg)
{
    mixed dest;
    object env, obj;

    if( !arg ) return notify_fail("你要往哪個方向走﹖\n");

    if( me->is_busy() ) {
	write("要移動請你先用 halt 終止你正在做的事。\n");
	return 1;
    }

    if( me->over_encumbranced() ) {
	write("你的負荷過重﹐動彈不得。\n");
	return 1;
    }

    env = environment(me);
    if( !env ) return notify_fail("你哪裡也去不了。\n");

    if( arg=="escape" ) {
	mapping exits = env->query("exits");
	string *dirs;
	if( mapp(exits) ) {
	    dirs = keys(exits);
	    arg = dirs[random(sizeof(dirs))];
	}
	else {
	    write("你已經無路可逃了！\n");
	    return 1;
	}
    }

    if( !(dest = env->query("exits/" + arg)) ) {
	write("這個方向沒有出路。\n");
	return 1;
    }

    // Load destination room and check its existence.
    if( !objectp(obj = stringp(dest) ? load_object(dest) : dest)) {
	write("這一個方向的出口有問題﹐請通知巫師處理。\n");
	return 1;
    }
    
    // Check for mudlib apply valid_leave()
    if( function_exists("valid_leave", env) && !env->valid_leave(me, arg) )
	return 0;

    return do_go( me, obj, arg );
}

int do_go(object me, object obj, string dir)
{
    string min, mout, dir_name;
    object old_env;

    if( !me || !obj ) return 0;

    if( undefinedp(dir_name = default_dirs[dir]) ) dir_name = dir;

    if( me->is_fighting() ) {
	object *enemy, ob;
	int intimidate, my_energy;

	mout = "往" + dir_name + "落荒而逃了。\n";
	min = "跌跌撞撞地跑了過來﹐模樣有些狼狽。\n";

	enemy = me->query_enemy() - ({ 0 });
	intimidate = me->query_ability("intimidate");
	my_energy = me->query_stat("gin");
	foreach(ob in enemy)
	{
	    if( userp(ob) || !living(ob) ) continue;
	    if( environment(ob) != environment(me) ) continue;
	    if( ! me->visible(ob) ) continue;
	    if( ob->query_ability("intimidate") > intimidate
	    &&	random(ob->query_stat("gin")) > my_energy ) {
		tell_object(me, "你被" + ob->name() + "擋住了！\n");
		me->start_busy(1);
		return 0;
	    }
	}
    } else {
        mout = "往" + dir_name + "離開。\n";
        min = "走了過來。\n";
    }

    old_env = environment(me);
    message( "vision", me->name() + mout, old_env, me );
    if( me->move(obj) && environment(me)==obj ) {
        message( "vision", me->name() + min, obj, me );
        me->set_temp("pending", 0);
        all_inventory(old_env)->follow_me(me, dir);
        return 1;
    }

    return 0;
}

int help(object me)
{
    write(@HELP
指令格式 : go <方向>
 
讓你往指定的方向移動。
 
HELP
    );
    return 1;
}
