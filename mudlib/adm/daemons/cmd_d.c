// vim: set ts=4 sw=4 syntax=lpc

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

    // vim: set ts=4 sw=4 syntax=lpc
    if( pointerp(p = cache[verb+".c"])
    &&	sizeof(p & path) )
        return p[0] + verb;

    // vim: set ts=4 sw=4 syntax=lpc
    p = path - cached_path;
    if( sizeof(p) ) {
	foreach(dir in p) rehash(dir);		// vim: set ts=4 sw=4 syntax=lpc
	if( pointerp(p = cache[verb+".c"])	// vim: set ts=4 sw=4 syntax=lpc
	&&  sizeof(p & path) )
	    return p[0] + verb;
    }

    return 0;
}

