/*---
description: Input prompt for get_char() and input_to() carryover mapping.
author: Annihilator <taedlar@gmail.com>
---*/

#include <ansi.h>

/* [NEOLITH-EXTENSION]
 * Show options menu.
 *
 * The options menu interaction is usually initiated by get_char() with a mapping
 * argument containing "options" and "cursor", and optionally "prompt" and "option_hints".
 * The user input is then translated by cursor_translate() in simul_efun to determine
 * the selected option.
 *
 * The options menu supports a simple cursor interface where the user can use up/down keys
 * to navigate the options, and press space/enter to select. If an option is in the form
 * of "X) Option", the user can also press "X" as a shortcut to select that option.
 *
 * Example usage:
 * mapping opts = ([
 *     "prompt": "Choose your character's race:",
 *     "options": ({ "human", "elf", "dwarf" }),
 *     "option_hints": (: call_other, CHAR_D, "hint_user_race" :),
 *     "cursor": 0
 * ]);
 * get_char ("get_race", 1, opts, ob);
 *
 * NOTE: Always call get_char() with no-echo flag set to 1 when using this input prompt, so
 * that the input_prompt()'s output is not interleaved with the user's input.
 */
void input_prompt (mixed func, int flags, mixed data, mixed arg1) {
    if (mapp(data) && arrayp(data["options"]) && intp(data["cursor"])) {
        string prompt = data["prompt"] || "你的選擇：";
        int pos = 0;
        write("\r" CLR "\n"); // placeholder for prompt
        foreach (string opt in data["options"]) {
            if (pos == data["cursor"])
                write ("-> " BLK BWHT);
            else
                write ("   ");
            write (" " + opt + " ");
            if (pos == data["cursor"]) {
                write (NOR);
                if (functionp (data["option_hints"]))
                    evaluate (data["option_hints"], opt);
            }
            write ("\n");
            pos++;
        }
        write (CUU (data["options"].len() + 1) + "\r" + prompt);
    }
    else if (func == "more" && arrayp (data) && intp (arg1)) {
        int page_size = this_object()->query("pager_size") || 22;
        if (arg1 < 0)
            arg1 = 0;
        else if (arg1 > sizeof (data))
            arg1 = sizeof (data);
        // The pager has two arguments: the text array and the current line number.
        printf (
            "\n" CLR BGRN "--More-- Lines %d-%d (%d%%)  "
            BWHT "  [PgDn|Space, PgUp|b, Up|k, Down|j, Home|g, End|G, q]  " NOR,
            arg1 - page_size + 1, arg1, arg1 * 100 / sizeof (data)
        );
    }
}
