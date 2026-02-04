// vim: set ts=4 sw=4 syntax=lpc

// Created by Annihilator (07/06/1995)

inherit F_CLEAN_UP;

void create()
{
    seteuid(getuid());
}

// This function is called by master object to return the "virtual" object
// of <file> when dirver failed to find such a file.
object compile_object(string file)
{
    string server, arg;

    if( sscanf(file, "%s:%s", server, arg) != 2 )
        return 0;

    return OBJECT_D(server)->virtual_create(arg);
}

