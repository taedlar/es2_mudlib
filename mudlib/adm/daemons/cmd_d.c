/*  cmd_d.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

// Created by Annihilator (11/07/94)

#pragma save_binary

#include <origin.h>

inherit F_CLEAN_UP;

static mapping cache = ([]);
static string *cached_path = ({});

private void
create()
{
    seteuid(getuid());
}

// rehash()
//
// This function updates the storeded list of paths of a specific directory.
// When you add or remove any command(s) from /cmds (the directories where
// hooked commands are located), you need call this to update the stored
// list.

void
rehash(string dir)
{
    string *cmds, cmd;

    // Security check, don't allow just anybody to fool us by updating
    // random directories.
    if( origin()==ORIGIN_CALL_OTHER
    &&	(geteuid(previous_object())!=ROOT_UID) )
        return;

    if( dir[<1]!='/' ) dir += "/";
    if( member_array(dir, cached_path) < 0 )
	cached_path += ({ dir });

    if( file_size(dir)==-2 )
	foreach(cmd in get_dir(dir + "*.c")) {
	    if( !cache[cmd] )
		cache[cmd] = ({ dir });
	    else if( member_array(dir, cache[cmd]) < 0 )
		cache[cmd] += ({ dir });
	}
}

string
find_command(string verb, string *path)
{
    string *p, dir;

    if( !pointerp(path) ) return 0;

    /* 如果 cache 中有這個指令，立即傳回 */
    if( pointerp(p = cache[verb+".c"])
    &&	sizeof(p & path) )
        return p[0] + verb;

    /* 檢查使用者是否有尚未進入 cache 的搜尋路徑 */
    p = path - cached_path;
    if( sizeof(p) ) {
	foreach(dir in p) rehash(dir);		/* 加入 cache */
	if( pointerp(p = cache[verb+".c"])	/* 再搜尋一次 */
	&&  sizeof(p & path) )
	    return p[0] + verb;
    }

    return 0;
}
