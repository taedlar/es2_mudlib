/*  object.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

varargs int
getoid(object ob)
{
    int id;

    if (!ob) ob = previous_object();
    sscanf(file_name(ob), "%*s#%d", id);
    return id;
}

// Get the owner of a file.  Used by log_error() in master.c.
string
file_owner(string file)
{
    string name, rest, dir;

    if (file[0] != '/') file = "/" + file;
    if (sscanf(file, "/u/%s/%s/%s", dir, name, rest) == 3) {
        return name;
    }
    return 0;
}

void
destruct(object ob)
{
    string err;

    if( !ob ) return;

    err = catch(ob->remove());

    if( err )
	log_file("destruct", sprintf("[%s] %O: %s", ctime(time()), ob, err));

    if( userp(ob)
    &&	(geteuid(previous_object())!=ROOT_UID)
    &&	(geteuid(previous_object())!=geteuid(ob)) ) {
	log_file("destruct", sprintf("[%s] %O: Destructed by %s denied\n",
	    ctime(time()), ob, err));
	error("Destruction denied.\n");
    }

    efun::destruct(ob);
}
