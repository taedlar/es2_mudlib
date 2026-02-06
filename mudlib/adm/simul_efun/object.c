

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

