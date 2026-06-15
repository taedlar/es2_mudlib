/*---
description: Cursor support for input prompt.
author: Annihilator <taedlar@gmail.com>
---*/

#include <ansi.h>

/*
 * Translate an input key (get_char) into an option using the cursor contexts in args.
 * Return the translated option, or 0 if the input is not a selection key.
 *
 * If an option is in the form of "X) Option", and the user presses space or enter, it
 * will be translated to "X" as a shortcut.
 *
 * This function usually works together with input_prompt() in feature/user/input.c,
 * which displays the options and updates the cursor position in args.
 */
mixed cursor_translate (string input, mapping args) {
    int cur = args["cursor"];
    int num_options = args["options"].len();
    string cur_option = args["options"][cur];
    switch (input) {
        case " ":
        case "\r":
        case "\n":
            if (cur_option.len() > 1 && cur_option[1] == ')')
                return cur_option[0..0]; /* shortcut for numbered option or hotkey */
            return cur_option; /* translated */
        case KEY_UP:
            args["cursor"] = (cur - 1 + num_options) % num_options;
            break;
        case KEY_DOWN:
            args["cursor"] = (cur + 1) % num_options;
            break;
    }
    return 0; /* not translated */
}
