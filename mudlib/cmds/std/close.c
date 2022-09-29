// close.c

#include <room.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	mapping doors;
	string *dirs, dir;
	int i;

	if( me->is_busy() ) return notify_fail("你現在沒有空﹗\n");
	if( !arg ) return notify_fail("你要關閉什麼﹖\n");

	doors = environment(me)->query_doors();
	if( mapp(doors) ) {
		dirs = keys(doors);
		if( member_array(arg, dirs)!=-1 ) dir = arg;
		else for(i=0; i<sizeof(dirs); i++)
			if( arg==doors[dirs[i]]["name"] || member_array(arg, doors[dirs[i]]["id"])!=-1 ) {
				dir = dirs[i];
				break;
			}
		if( !dir ) return notify_fail("你要關閉什麼﹖\n");
	
		if( environment(me)->close_door(dir) ) {
			message_vision("$N將" + doors[dir]["name"] + "關上。\n", me);
			return 1;
		}
	}

	return 0;
}

int help(object me)
{
        write(@HELP
指令格式 : close <door> 或 <方向>
 
這個指令可以讓你關上門.
 
HELP
    );
    return 1;
}
