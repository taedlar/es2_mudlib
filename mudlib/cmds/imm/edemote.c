/*  edemote.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <command.h>

inherit F_CLEAN_UP;

int in_use;

int main(object me, string arg)
{
    mapping emote;

    SECURED_COMMAND;

    if( !arg ) return notify_fail("你要編輯什麼 emote﹖\n");

    if( sscanf(arg, "-d %s", arg) ) {
        write("刪除 emote﹕" + arg + "\n");
        EMOTE_D->delete_emote(arg);
        return 1;
    }

    if( sscanf(arg, "-p %s", arg) ) {
        emote = EMOTE_D->query_emote(arg);
        printf("上次修改﹕%s\n", emote["updated"]);
        printf("─────────────\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
            emote["myself"], emote["others"], emote["myself_self"],
            emote["others_self"], emote["myself_target"], emote["target"],
            emote["others_target"] );
        return 1;
    }

    emote = EMOTE_D->query_emote(arg);
    emote = (["updated":geteuid(me)]);

    write("編輯 emote﹕" + arg + "\n");
    write("訊息可以有好幾行﹐用 . 表示結束。\n");
    write("訊息中可使用的參數有以下幾種﹕\n");
    write("  $N  自己的名字。\n");
    write("  $n  使用對象的名字。\n");
    write("  $P  自己的人稱代名詞﹐如你、妳、他、她、牠、它。\n");
    write("  $p  使用對象的人稱代名詞﹐如你、妳、他、她、牠、它。\n");
    write("────────────────────────────────────\n");
    write("不指定對象使用這個 emote 時﹐你自己看到的訊息﹕\n->");
    input_to("get_msg_myself", emote, arg);
    return 1;
}


private int
get_msg_myself(string msg, mapping emote, string pattern)
{
    if (msg==".") {
        if( !undefinedp(emote["myself"]) ) emote["myself"] += "\n";
        write("不指定對象使用這個 emote 時﹐其他人看到的訊息﹕\n->");
        input_to("get_msg_others", emote, pattern);
        return 1;
    }
    if( !undefinedp(emote["myself"]) )
        emote["myself"] += msg + "\n";
    else emote["myself"] = msg;
    write("->");
    input_to("get_msg_myself", emote, pattern);
    return 1;
}

private int
get_msg_others(string msg, mapping emote, string pattern)
{
    if (msg==".") {
        if( !undefinedp(emote["others"]) ) emote["others"] += "\n";
        write("對自己使用這個 emote 時﹐自己看到的訊息﹕\n->");
        input_to("get_msg_myself_self", emote, pattern);
        return 1;
    }
    if( !undefinedp(emote["others"]) )
        emote["others"] += msg + "\n";
    else emote["others"] = msg;
    write("->");
    input_to("get_msg_others", emote, pattern);
    return 1;
}

private int
get_msg_myself_self(string msg, mapping emote, string pattern)
{
    if (msg==".") {
        if( !undefinedp(emote["myself_self"]) ) emote["myself_self"] += "\n";
        write("對自己使用這個 emote 時﹐其他人看到的訊息﹕\n->");
        input_to("get_msg_others_self", emote, pattern);
        return 1;
    }
    if( !undefinedp(emote["myself_self"]) )
        emote["myself_self"] += msg + "\n";
    else emote["myself_self"] = msg;
    write("->");
    input_to("get_msg_myself_self", emote, pattern);
    return 1;
}

private int
get_msg_others_self(string msg, mapping emote, string pattern)
{
    if (msg==".") {
        if( !undefinedp(emote["others_self"]) ) emote["others_self"] += "\n";
        write("對別人使用這個 emote 時﹐自己看到的訊息﹕\n->");
        input_to("get_msg_myself_target", emote, pattern);
        return 1;
    }
    if( !undefinedp(emote["others_self"]) )
        emote["others_self"] += msg + "\n";
    else emote["others_self"] = msg;
    write("->");
    input_to("get_msg_others_self", emote, pattern);
    return 1;
}

private int
get_msg_myself_target(string msg, mapping emote, string pattern)
{
    if (msg==".") {
        if( !undefinedp(emote["myself_target"]) ) emote["myself_target"] += "\n";
        write("對別人使用這個 emote 時﹐使用對象看到的訊息﹕\n->");
        input_to("get_msg_target", emote, pattern);
        return 1;
    }
    if( !undefinedp(emote["myself_target"]) )
        emote["myself_target"] += msg + "\n";
    else emote["myself_target"] = msg;
    write("->");
    input_to("get_msg_myself_target", emote, pattern);
    return 1;
}

private int
get_msg_target(string msg, mapping emote, string pattern)
{
    if (msg==".") {
        if( !undefinedp(emote["target"]) ) emote["target"] += "\n";
        write("對別人使用這個 emote 時﹐除你自己和使用對象外﹐其他人看到的訊息﹕\n->");
        input_to("get_msg_others_target", emote, pattern);
        return 1;
    }
    if( !undefinedp(emote["target"]) )
        emote["target"] += msg + "\n";
    else emote["target"] = msg;
    write("->");
    input_to("get_msg_target", emote, pattern);
    return 1;
}

private int
get_msg_others_target(string msg, mapping emote, string pattern)
{
    if (msg==".") {
        if( !undefinedp(emote["others_target"]) ) emote["others_target"] += "\n";
        EMOTE_D->set_emote(pattern, emote);
        write("Emote 編輯結束。\n");
        return 1;
    }
    if( !undefinedp(emote["others_target"]) )
        emote["others_target"] += msg + "\n";
    else emote["others_target"] = msg;
    write("->");
    input_to("get_msg_others_target", emote, pattern);
    return 1;
}

int help(object me)
{
    write(@HELP
指令格式：edemote [-d|-p] <emote>
 
這個指令可以修改, 刪除 emote 或列出其內容. 加上 -d 參數會刪除
指定的 emote, -p 參數則會列出指定 emote 的內容. 列出的順序與編
輯 emote 時相同.
 
輸入 emote 訊息時有三個項目: 沒有目標, 指定目標或是對自己. 若
不想有某項訊息, 則直接在空白行輸入 '.' 跳過.
 
一個 emote 訊息可以有很多行, 在空白行輸入 '.' 結束輸入該項 emote.
 
編輯 emote 時可以用以下的符號來表示:
 
$N : 自己的名字.
$n : 目標的名字.
$P : 自己的人稱代名詞.
$p : 目標的人稱代名詞.
 
HELP
    );
    return 1;
}
