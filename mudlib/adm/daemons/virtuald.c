/*  virtuald.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

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
