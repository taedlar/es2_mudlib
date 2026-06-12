/*---
description: Cursor support for input prompt.
author: Annihilator <taedlar@gmail.com>
---*/

#include <ansi.h>

/*
 * Translate an input key (get_char) into an option using the cursor contexts in args.
 * Return the translated option, or 0 if the input is not a selection key.
 */
mixed cursor_translate (string input, mapping args) {
    int cur = args["cursor"];
    int num_options = args["options"].len();
    switch (input) {
        case " ":
        case "\r":
        case "\n":
            return args["options"][cur]; /* translated */
        case KEY_UP:
            args["cursor"] = (cur - 1 + num_options) % num_options;
            break;
        case KEY_DOWN:
            args["cursor"] = (cur + 1) % num_options;
            break;
    }
    return 0; /* not translated */
}
