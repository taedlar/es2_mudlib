/*---
description: Input prompt for get_char() and input_to() carryover mapping.
author: Annihilator <taedlar@gmail.com>
---*/

#include <ansi.h>

/* [NEOLITH-EXTENSION] Show options menu */
void input_prompt (mixed func, int flags, mixed data) {
    if (mapp(data) && arrayp(data["options"]) && intp(data["cursor"])) {
        string prompt = data["prompt"] || "你的選擇：";
        int pos = 0;
        write("\r" CLR "\n"); // placeholder for prompt
        foreach (string opt in data["options"]) {
            if (pos == data["cursor"])
                write("-> " BLK BWHT);
            else
                write("   ");
            write (" " + opt + " ");
            if (pos == data["cursor"])
                write (NOR);
            if (functionp(data["option_hints"]))
                evaluate (data["option_hints"], opt);
            write("\n");
            pos++;
        }
        write (CUU (data["options"].len() + 1) + "\r" + prompt);
    }
}
