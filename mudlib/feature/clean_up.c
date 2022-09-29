/*  clean_up.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

int clean_up()
{
    object inv;
    int i;

    // Never cleanup an interactive user.
    if( interactive(this_object()) ) return 1;

    // If we are contained in something, let environment do the clean
    // up instead of making recursive call. This will prevent clean-up
    // time lag.
    if( environment() ) return 1;

    // If we have any interactive user in our inventory, don't clean up.
    foreach(inv in all_inventory())
        if( interactive(inv) ) return 1;

    destruct(this_object());
    return 0;
}
