/*  semote.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include "/doc/help.h"

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    string *e, msg;
    int i;

    if( arg ) {
        mapping emote;
        if( !mapp(emote = EMOTE_D->query_emote(arg)) )
            return notify_fail("沒有這個 emote。\n");
        printf("上次修改﹕%s\n", emote["updated"]);
        printf("─────────────\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
            emote["myself"], emote["others"], emote["myself_self"],
            emote["others_self"], emote["myself_target"], emote["target"],
            emote["others_target"] );
        return 1;
    }

    e = sort_array(EMOTE_D->query_all_emote(), 1);

    msg = "";
    for(i=0; i<sizeof(e); i++)
        msg += sprintf("%-15s%s", e[i], (i%5==4)?"\n": "");
    write(msg + "\n");
    return 1;
}

int help(object me)
{
  write(@HELP
指令格式 : semote [<emote>]

這個指令可以列出目前所能使用的 emote﹐你也可以指定某一個 emote 來顯示
做這個動作時會有什麼訊息。
HELP
    );
    return 1;
}
