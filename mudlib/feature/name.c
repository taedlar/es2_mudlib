/*  name.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <ansi.h>
#include <dbase.h>

static string *my_id;

int
visible(object ob)
{
    return 1;
}

void
set_name(string name, mixed *id_list ...)
{
    mixed id;

    set("name", name);

    if( sizeof(id_list)==1 && arrayp(id_list[0]) ) {
	set("id", id_list[0][0]);
	my_id = id_list[0];
    } else {
	set("id", id_list[0]);
	my_id = id_list;
    }
}

string
query_id()
{
    return query("id");
}

int
id(string str)
{
    return pointerp(my_id) && (member_array(str, my_id) >= 0);
}

string*
parse_command_id_list()
{
    return my_id;
}

varargs string
name(int raw)
{
    string str;

    if( stringp(str = query("name")) ) return str;
    return file_name(this_object());
}

varargs string
short(int raw)
{
    string str;

    if( !stringp(str = query("short", 1)) ) str = name(1);

    if( this_player()
    &&	!this_player()->query("option/BRIEF_SHORT") )
	str += "(" + capitalize(raw ? query("id") : this_object()->query_id()) + ")";

    return str;
}

varargs string
long(int raw)
{
    string str;

    if( stringp(str = query("long")) ) return str;
    return this_object()->name(raw) + "看起來沒有什麼特別。\n";
}

varargs string
rank(string politeness, int raw)
{
    return name(raw);
}
