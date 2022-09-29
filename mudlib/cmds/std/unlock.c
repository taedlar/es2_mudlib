// open.c

#include <room.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	mapping doors;
	string *dirs, dir, key;
	object key_ob;
	int i;

	if( !arg ) return notify_fail("你要開什麼東西的鎖？\n");
	if( sscanf(arg, "%s with %s", arg, key) != 2 )
		return notify_fail("你要用什麼東西開鎖？\n");
	if( !objectp(key_ob = present(key, me)) )
		return notify_fail("你身上沒有這種東西。\n");

	doors = environment(me)->query_doors();
	if( mapp(doors) ) {
		dirs = keys(doors);
		if( member_array(arg, dirs)!=-1 ) dir = arg;
		else for(i=0; i<sizeof(dirs); i++)
			if( arg==doors[dirs[i]]["name"] || member_array(arg, doors[dirs[i]]["id"])!=-1 ) {
				dir = dirs[i];
				break;
			}
		if( !dir ) return notify_fail("你要開什麼東西的鎖？\n");
	
		if( environment(me)->unlock_door(dir, key_ob->query("lock")) ) {
			message_vision("$N將" + doors[dir]["name"] + "上的鎖打開。\n", me);
			return 1;
		}
	}

	return 0;
}

int help(object me)
{
        write(@HELP
指令格式 : unlock <物品或門> with <鑰匙>
 
這個指令可以讓你打開鎖著的門或容器。
 
HELP
    );
    return 1;
}
