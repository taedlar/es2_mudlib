/*  daemond.c

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

inherit F_CLEAN_UP;
inherit F_SAVE;

mapping daemons = ([ ]);

void create()
{
    seteuid(getuid());
    restore();
}

string query_save_file() { return DATA_DIR + "daemon"; }

void remove()
{
    save();
}

object query_daemon(string id)
{
    object ob;
    string file;

    if( !stringp(file = daemons[id]) )
        file = "/daemon/" + replace_string(replace_string(id, " ", "_"), ":", "/");

    if( ob = find_object(file) ) return ob;

    if( !catch(ob = load_object(file)) && objectp(ob) ) return ob;
    // Delete the daemon entry to prevent later attemption of loading. Once
    // the daemon is loaded successfully, it is the skill daemon's duty
    // to register itself again.
    else map_delete(daemons, id);

    return this_object();
}

mapping query_daemons() { return daemons; }

int register_race_daemon(string race)
{
    if( !previous_object() || clonep(previous_object()) ) return 0;
    
    if( !undefinedp(daemons["race:"+race])
    &&	base_name(previous_object()) != daemons["race:"+race]
    &&	geteuid(previous_object()) != MUDLIB_UID
    &&	geteuid(previous_object()) != DOMAIN_UID )
	error("DAEMON_D: registration conflict, and access denied.\n");

    daemons["race:"+race] = base_name(previous_object());
    return save();
}

int register_class_daemon(string class_name)
{
    if( !previous_object() || clonep(previous_object()) ) return 0;

    if( !undefinedp(daemons["class:"+class_name])
    &&	base_name(previous_object()) != daemons["class:"+class_name]
    &&	geteuid(previous_object()) != MUDLIB_UID
    &&	geteuid(previous_object()) != DOMAIN_UID )
	error("DAEMON_D: registration conflict, and access denied.\n");

    daemons["class:"+class_name] = base_name(previous_object());
    return save();
}

int register_condition_daemon(string condition)
{
    if( !previous_object() || clonep(previous_object()) ) return 0;

    if( !undefinedp(daemons["condition:"+condition])
    &&	base_name(previous_object()) != daemons["condition:"+condition]
    &&	geteuid(previous_object()) != MUDLIB_UID
    &&	geteuid(previous_object()) != DOMAIN_UID )
	error("DAEMON_D: registration conflict, and access denied.\n");

    daemons["condition:"+condition] = base_name(previous_object());
    return save();
}

int register_object_daemon(string domain)
{
    if( !previous_object() || clonep(previous_object()) ) return 0;

    if( !undefinedp(daemons["domain:"+domain])
    &&	base_name(previous_object()) != daemons["domain:"+domain]
    &&	geteuid(previous_object()) != MUDLIB_UID
    &&	geteuid(previous_object()) != DOMAIN_UID )
	error("DAEMON_D: registration conflict, and access denied.\n");

    daemons["domain:"+domain] = base_name(previous_object());
    return save();
}

int register_skill_daemon(string skill)
{
    if( !previous_object() || clonep(previous_object()) ) return 0;

    if( !undefinedp(daemons["skill:"+skill])
    &&	base_name(previous_object()) != daemons["skill:"+skill]
    &&	geteuid(previous_object()) != MUDLIB_UID
    &&	geteuid(previous_object()) != DOMAIN_UID )
	error("DAEMON_D: registration conflict, and access denied.\n");

    daemons["skill:"+skill] = base_name(previous_object());
    return save();
}

int unregister_daemon(string d)
{
    if( previous_object()
    &&	base_name(previous_object()) != daemons[d]
    &&	geteuid(previous_object()) != ROOT_UID )
	return 0;

    map_delete( daemons, d );
    return 1;
}
