/*
 *  Package: Generic object clean-up
 */

int clean_up() {
    object inv;
    int i;

    // Never cleanup an interactive user.
    if (interactive(this_object()))
        return 1;

    // If we are contained in something, let environment do the clean
    // up instead of making recursive call. This will prevent clean-up
    // time lag.
    if (environment())
        return 1;

    // If we have any interactive user in our inventory, don't clean up.
    foreach (inv in all_inventory()) {
        if (interactive(inv))
            return 1;
    }

    // If we reach here, this is a room or dangling object not accessed by
    // anyone for a while. Clean it up.
    destruct (this_object());
    return 0;
}

