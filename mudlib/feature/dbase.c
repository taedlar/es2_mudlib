/*  dbase.c - database feature

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/


#include <dbase.h>
#include <function.h>

//inherit F_TREEMAP;

mapping dbase;
static mapping tmp_dbase;

// The default_ob provides the default values of the dbase. It is set to
// be master copy of an object.
static mixed default_ob;

mixed query_default_object() { return default_ob; }

void
set_default_object(mixed ob)
{
    if( !geteuid() ) seteuid(getuid());
    if( ob==this_object()
    ||  ob==file_name(this_object()) )
        return;
    default_ob = ob;
}

mixed
set(string prop, mixed data)
{
    mixed cont;
    int r;

    if( !mapp(dbase) ) dbase = ([]);

    while( (r = strsrch(prop, '/', -1)) > 0 )
    {
	cont = match_path( dbase, prop[0..r-1] );
	if( mapp(cont) ) return cont[prop[r+1..]] = data;
	data = ([ prop[r+1..]: data ]);
	prop = prop[0..r-1];
    }

    return dbase[prop] = data;
}

varargs mixed
query(string prop, int raw)
{
    int fp;
    mixed data;

    if( !mapp(dbase) ) return 0;

    data = match_path(dbase, prop);

    if( raw ) return data;

    if( undefinedp(data) && default_ob )
        data = default_ob->query(prop, 1);

    if( (fp = functionp(data)) & FP_OWNER_DESTED ) {
        if( fp & FP_NOT_BINDABLE ) {
            delete(prop);
            return 0;
        }
        data = set(prop, bind(data, this_object()));
    }

    return evaluate( data, this_object() );
}

void
delete(string prop)
{
    mixed cont;
    int r;

    if( !mapp(dbase) ) return;

    if( (r = strsrch(prop, '/', -1)) > 0 ) {
	cont = match_path(dbase, prop[0..r-1]);
	if( mapp(cont) )
	    map_delete(cont, prop[r+1..]);
	return 0;
    }

    map_delete(dbase, prop);
}

mixed
add(string prop, mixed data)
{
    mixed old;

    if( !mapp(dbase) || !(old = query(prop, 1)) )
        return set(prop, data);

    if( functionp(old) ) return old;

    return set(prop, old + data);
}

mapping
query_entire_dbase()
{
    object ob;

    foreach(ob in previous_object(-1)) {
	if( (geteuid(ob) == ROOT_UID)
	||  (geteuid(ob) == getuid(this_object())) 
	||  master()->valid_write( base_name(this_object()), ob, "write_file" ) )
	    continue;
	error("Permission denied.\n");
    }
    return dbase;
}

mixed
set_temp(string prop, mixed data)
{
    mixed cont;
    int r;

    if( !mapp(tmp_dbase) ) tmp_dbase = ([]);

    while( (r = strsrch(prop, '/', -1)) > 0 )
    {
	cont = match_path( tmp_dbase, prop[0..r-1] );
	if( mapp(cont) ) return cont[prop[r+1..]] = data;
	data = ([ prop[r+1..]: data ]);
	prop = prop[0..r-1];
    }

    return tmp_dbase[prop] = data;
}

varargs mixed
query_temp(string prop, int raw)
{
    mixed data;

    if( !mapp(tmp_dbase) ) return 0;
    data = match_path(tmp_dbase, prop);
    return raw ? data : evaluate(data, this_object());
}

void
delete_temp(string prop)
{
    mixed cont;
    int r;

    if( !mapp(tmp_dbase) ) return;

    if( (r = strsrch(prop, '/', -1)) > 0 ) {
	cont = match_path(tmp_dbase, prop[0..r-1]);
	if( mapp(cont) )
	    map_delete(cont, prop[r+1..]);
	return 0;
    }

    map_delete(tmp_dbase, prop);
}

mixed
add_temp(string prop, mixed data)
{
    mixed old;

    if( !mapp(tmp_dbase) || !(old = query_temp(prop, 1)) )
	return set_temp(prop, data);

    if( functionp(old) ) return old;

    return set_temp(prop, old + data);
}

mapping
query_entire_temp_dbase()
{
    return tmp_dbase;
}

void
clear_temp_dbase()
{
    tmp_dbase = ([]);
}
