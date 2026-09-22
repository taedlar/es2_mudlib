// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 1994-2026 Annihilator <taedlar@gmail.com>

#include <ansi.h>
#include <command.h>

inherit F_CLEAN_UP;

int main (object me, string arg) {
    string wiz_status;
    object *user, link_ob;
    int i;

    SECURED_ADMIN_COMMAND;

    mapping opts = ([
        "prompt": "你確定要執行遊戲關閉程序嗎 (y/n)? ",
        "options": ({ "No", "Yes" }),
        "cursor": 0
    ]);
    get_char ("confirm_shutdown", 1, opts);

    return 1;
}

void confirm_shutdown (string arg, mapping opts) {
    string answer = cursor_translate (arg, opts);
    switch (answer ? answer : arg) {
    case "Yes": case "Y": case "y":
        seteuid (geteuid(this_player(1)));
        message ("system", "遊戲關閉程序執行中﹐請稍候幾分鐘再嘗試連線。\n", users() );

        foreach (object user in users()) {
            if (!user.environment())
                continue;
            user->save();
        }

        write ("\r" CLR "\n"); // clear the line before shutdown
        shutdown();
        break;
    case "No": case "N": case "n":
        write ("\r" CLR "\n"); // clear the line
        break;
    default:
        get_char ("confirm_shutdown", 1, opts);
        break;
    }
}

int help (object me) {
    write (@HELP
指令格式: shutdown
 
遊戲關閉程序。
HELP
);
    return 1;
}
 

