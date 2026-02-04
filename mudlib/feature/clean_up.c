// vim: set ts=4 sw=4 syntax=lpc

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

