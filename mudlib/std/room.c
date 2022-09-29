/*  room.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#pragma save_binary

#include <ansi.h>
#include <dbase.h>
#include <room.h>

inherit F_DBASE;
inherit F_CLEAN_UP;

static mapping doors;
static mapping guards;

int receive_object(object ob, int from_inventory) { return 1; }

// clean_up()
//
// We override the clean_up() here to provide that the room won't got
// destructed when there is NPC(s) created in this room was wandering
// outside, and create another copy of the same NPC when next time
// the room is loaded.

int clean_up(int inherit_flag)
{
    mapping items;
    string file;
    object ob;

    // Don't clean up if any NPCs we created were wandering somewhere.
    // This may introduce a potential deadlock that when room-A holds
    // an NPC from room-B while room-B holds another NPC from room-A.
    // Anyway, such condition is really rare and its only fault is leaving
    // the room not cleaned up when deadlock exists. So, we just merely
    // ignore it :P                                 - Annihilator

    if( mapp(items = query_temp("objects")) )
	foreach(file, ob in items) {
	    if( objectp(ob)
	    &&	ob->is_character()
	    &&	environment(ob)!=this_object() )
		return 1;
	}

    return ::clean_up(inherit_flag);
}

void remove()
{
    mapping items;
    string file;
    object ob;
    int cnt = 0;

    if( !mapp(items = query_temp("objects")) ) return;

    // Destroy wandering NPC created by this room as well.
    foreach(file, ob in items) {
	if( objectp(ob)
	&&  ob->is_character()
	&&  environment(ob)!=this_object() ) {
	    message("vision", "一陣強烈的閃光忽然出現﹐吞沒了" + ob->name() + "。\n",
		environment(ob));
	    destruct(ob);
	    cnt++;
	}
    }
    if( cnt && this_player() )
	write("WARNNING: " + cnt + " wandering NPC(s) created by this room are forced destructed.\n");
}

object
make_inventory(string file)
{
    object ob;

    ob = new(file);

    // Support for uniqueness
    if( ob->violate_unique() ) ob = ob->create_replica();
    if( !ob ) return 0;

    ob->move(this_object());

    return ob;
}

void reset()
{
    mapping ob_list, ob;
    string file;
    int amount;
    object obj, *inv;

    ob_list = query("objects");
    if( mapp(ob_list) )
    {
	if( !mapp(ob = query_temp("objects")) ) ob = ([]);

	foreach(file, amount in ob_list)
	{	/* 檢查房間需要載入的物件數量是否足夠 */
	    if( amount==1 ) {
		if( !objectp(ob[file]) ) ob[file] = make_inventory(file);
	    } else {
		int i;
		for(i=amount-1; i>=0; i--) {
		    if( objectp(ob[file + " " + i]) ) continue;
		    ob[file + " " + i] = make_inventory(file);
		}
	    }
	}
	set_temp("objects", ob);
    }

    inv = all_inventory(this_object());
    foreach(obj in inv)
	if( interactive(obj) || !clonep(obj) ) return;

    // Reset resource value
    if( mapp(query("resource")) )
	set_temp("resource", copy(query("resource")) );
}

// Redirect item_desc of the door to this function in default.
string look_door(string dir)
{
    if( !mapp(doors) || undefinedp(doors[dir]) || doors[dir]["status"]&DOOR_HIDDEN)
	return "你要看什麼﹖\n";
    if( doors[dir]["status"] & DOOR_CLOSED )
	return "這個" + doors[dir]["name"] + "是關著的。\n";
    else
	return 0;
//	return "這個" + doors[dir]["name"] + "是開著的。\n";
}

varargs int
open_door(string dir, int from_other_side)
{
    mapping exits;
    object ob;

    if( !mapp(doors) || undefinedp(doors[dir])
    || (!from_other_side && doors[dir]["status"]&DOOR_HIDDEN))
	return notify_fail("這個方向沒有門。\n");

    if( !from_other_side && !(doors[dir]["status"] & DOOR_CLOSED) )
	return notify_fail( doors[dir]["name"] + "已經是開著的了。\n");

    if( !from_other_side && doors[dir]["status"] & DOOR_LOCKED )
	return notify_fail( doors[dir]["name"] + "是鎖著的。\n");

    exits = query("exits");
    if( !mapp(exits) || undefinedp(exits[dir]) )
	error("Room: open_door: attempt to open a door with out an exit.\n");

    if( from_other_side ) {
	message("vision", "有人從另一邊將" + doors[dir]["name"] + "打開了。\n", this_object());
    } else if( objectp(ob = find_object(exits[dir])) ) {
	if( !ob->open_door(doors[dir]["other_side_dir"], 1) )
	    return notify_fail("門的另一邊好像卡住了。\n");
    }

    doors[dir]["status"] &= (~DOOR_CLOSED);
    return 1;
}

varargs int
close_door(string dir, int from_other_side)
{
    mapping exits;
    object ob;

    if( !mapp(doors) || undefinedp(doors[dir]
    ||    (!from_other_side && doors[dir]["status"]&DOOR_HIDDEN)) )
	return notify_fail("這個方向沒有門。\n");

    if( (doors[dir]["status"] & DOOR_CLOSED) )
	return notify_fail( doors[dir]["name"] + "已經是關著的了。\n");

    exits = query("exits");
    if( !mapp(exits) || undefinedp(exits[dir]) )
	error("Room: close_door: attempt to open a door with out an exit.\n");

    if( from_other_side )
	message("vision", "有人從另一邊將" + doors[dir]["name"] + "關上了。\n", this_object());
    else if( objectp(ob = find_object(exits[dir])) ) {
	if( !ob->close_door(doors[dir]["other_side_dir"], 1) ) return 0;
    }

    doors[dir]["status"] |= DOOR_CLOSED;
    return 1;
}


varargs int
lock_door(string dir, string key, int from_other_side)
{
    object ob;
    mapping exits;

    if( !mapp(doors) || undefinedp(doors[dir]) )
	return notify_fail("這個方向沒有門。\n");
    if( !doors[dir]["lock"] ) return notify_fail("這個"+ doors[dir]["name"] 
	+ "上面沒有鎖。\n");
    if( doors[dir]["lock"] != key )
	return notify_fail("你的鑰匙不對。\n");

    if( doors[dir]["status"] & DOOR_LOCKED ) 
	return notify_fail(doors[dir]["name"] + "已經鎖上了。\n");

    exits = query("exits");
    if( !mapp(exits) || undefinedp(exits[dir]) )
	error("Room: lock_door: attempt to lock a door with out an exit.\n");

    if( from_other_side )
	message("vision", "你聽到" + doors[dir]["name"] + "發出「喀」地一聲。\n",
	this_object());
    else if( objectp(ob = find_object(exits[dir])) )
	if( !ob->lock_door(doors[dir]["other_side_dir"], key, 1) ) return 0;

    doors[dir]["status"] |= DOOR_LOCKED;
    return 1;
}

varargs int
unlock_door(string dir, string key, int from_other_side)
{
    object ob;
    mapping exits;

    if( !mapp(doors) || undefinedp(doors[dir]) )
	return notify_fail("這個方向沒有門。\n");
    if( !doors[dir]["lock"] ) return notify_fail("這個"+ doors[dir]["name"] 
	+ "上面沒有鎖。\n");
    if( doors[dir]["lock"] != key )
	return notify_fail("你的鑰匙不對。\n");

    if( !(doors[dir]["status"] & DOOR_LOCKED) ) 
	return notify_fail(doors[dir]["name"] + "並沒有上鎖。\n");

    exits = query("exits");
    if( !mapp(exits) || undefinedp(exits[dir]) )
	error("Room: unlock_door: attempt to unlock a door with out an exit.\n");

    if( from_other_side )
	message("vision", "你聽到" + doors[dir]["name"] + "發出「喀」地一聲。\n",
	this_object());
    else if( objectp(ob = find_object(exits[dir])) )
	if( !ob->unlock_door(doors[dir]["other_side_dir"], key, 1) ) return 0;

    doors[dir]["status"] &= ~DOOR_LOCKED;
    return 1;
}

#define CONSIST_DOOR_STATUS (DOOR_CLOSED|DOOR_LOCKED)
int
check_door(string dir, mapping door)
{
    // If we have no responding door, assume it is correct.
    if( !mapp(doors) || undefinedp(doors[dir]) ) return 1;

    door["status"] &= (~CONSIST_DOOR_STATUS);
    door["status"] |= (doors[dir]["status"] & CONSIST_DOOR_STATUS);
    return 1;
}

varargs void
create_door(string dir, mixed data, string other_side_dir, int status)
{
    mapping d, item_desc;
    object ob;
    string exit;

    if( !stringp(exit = query("exits/" + dir)) )
	error("Room: create_door: attempt to create a door without exit.\n");

    // String mode.
    if( stringp(data) ) {
	d = allocate_mapping(4);
	d["name"] = data;
	d["id"] = ({ dir, data, "door" });
	d["other_side_dir"] = other_side_dir;
	d["status"] = status;
    // Compelete specification mode.
    } else if( mapp(data) )
	d = data;
    else
	error("Create_door: Invalid door data, string or mapping expected.\n");

    set("detail/" + dir, (: look_door, dir :) );

    if( objectp(ob = find_object(exit)) ) {
	if( !ob->check_door(other_side_dir, d) )
	    return;
    }

    // Add the door.
    if( !mapp(doors) ) doors = ([ dir: d ]);
    else doors[dir] = d;
}

mapping query_doors() { return doors; }

mixed
query_door(string dir, string prop)
{
    if( !mapp(doors) || undefinedp(doors[dir]) ) return 0;
    else return doors[dir][prop];
}

mixed
set_door(string dir, string prop, mixed data)
{
    if( !mapp(doors) || undefinedp(doors[dir]) ) return 0;
    else return doors[dir][prop] = data;
}

int
valid_leave(object me, string dir)
{
    int i, move_skill;
    string my_name;
    object guard, *ob;

    if( mapp(doors) && !undefinedp(doors[dir]) ) {
	if( doors[dir]["status"] & DOOR_CLOSED ) {
	    if( doors[dir]["status"]& DOOR_HIDDEN ) return notify_fail("這個方向沒有出路。\n");
	    return notify_fail("你必須先把" + doors[dir]["name"] + "打開﹗\n");
	}
	// if ( door[dir]["status"] & DOOR_HAS_TRAP ) ....
    }
    return 1;
}

varargs int
do_look(object me, string arg)
{
    int i;
    object *inv, ob;
    mapping exits;
    string str, *dirs;

    // Look specific object in the room.
    if( arg ) {
	if( str = query("detail/" + arg) ) {
	    write(str);
	    return 1;
	}
	if( strsrch(query("long"), arg) >= 0 )
	    return notify_fail("你看不出這裡的" + arg + "有什麼特別的。\n");
	return notify_fail("你要看什麼﹖\n");
    }

    if( previous_object() && previous_object()->query("option/BRIEF_ROOM") )
	str = query("short") + "，";
    else
	str = sprintf( "%s - %s\n    %s%s    ",
	    query("short"),
	    wizardp(me)? file_name(this_object()) : "",
	    query("long"),
	    query("outdoors") ? NATURE_D->outdoor_room_description() : "" );

    if( mapp(exits = query("exits")) )
	dirs = keys(exits);
    
    // Check for exits with door.
    if( mapp(doors) )
	dirs = filter(dirs, (: undefinedp(doors[$1]) || (doors[$1]["status"] & DOOR_CLOSED)==0 :));

    if( sizeof(dirs)==0 )
	str += "這裡沒有任何明顯的出路。\n";
    else if( sizeof(dirs)==1 )
	str += "這裡唯一的出口是 " BOLD + dirs[0] + NOR "。\n";
    else
	str += sprintf("這裡明顯的出口是 " BOLD "%s" NOR " 和 " BOLD "%s" NOR "。\n",
	    implode(dirs[0..<2], "、"), dirs[<1]);

    inv = all_inventory(this_object()) - ({ me });
    foreach(ob in inv) {
	if( !ob->visible(me) ) continue;
	str = sprintf("%s  %s\n", str, ob->short() );
    }

    message("vision", str, me);
    return 1;
}

void
init()
{
    int explore_id;
    if( !undefinedp(explore_id = query("site_explore")) ) {
	if( !this_player()->recognize(explore_id, 1) ) {
	    this_player()->gain_score("survive", 100);
	    this_player()->gain_score("explorer fame", 1);
	}
    }
}

void
setup()
{
    seteuid(getuid());
    this_object()->reset();
}
