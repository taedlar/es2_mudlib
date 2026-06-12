/*---
description: Save and restore LPC objects.
author: Annihilator <taedlar@gmail.com>
---*/

int save() {
    string file;

    // always use the UID for security check, never trust the euid of the object.
    seteuid (getuid());
    if (geteuid() == "guest")
        return 0;

    if (stringp(file = this_object()->query_save_file())) {
        assure_file(file);
        return save_object (file);
    }
    return 0;
}

int restore() {
    string file;

    seteuid (getuid());
    if (stringp (file = this_object()->query_save_file()))
        return restore_object (file, 1);
    return 0;
}
