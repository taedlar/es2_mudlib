/*  save.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

int save()
{
    string file;

    seteuid(getuid());
    if( stringp(file = this_object()->query_save_file()) ) {
        assure_file(file);
        return save_object(file);
    }
    return 0;
}

int restore()
{
    string file;

    seteuid(getuid());
    if( stringp(file = this_object()->query_save_file()) )
        return restore_object(file, 1);
    return 0;
}
