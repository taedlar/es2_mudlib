// vim: set ts=4 sw=4 syntax=lpc

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string *tuned_ch;

    tuned_ch = me->link()->query("channels");
    if( !arg ) {
        if( !pointerp(tuned_ch) || !sizeof(tuned_ch) )
            write("你現在並沒有收聽任何頻道。\n");
        else
            write("你現在收聽的頻道﹕" + implode(tuned_ch, ", ") + "。\n");
        return 1;
    }

    if( pointerp(tuned_ch) && member_array(arg, tuned_ch)!=-1 ) {
        write("關閉 " + arg + " 頻道。\n");
        tuned_ch -= ({ arg });
        me->link()->set("channels", tuned_ch);
        return 1;
    } else {
        write("要打開某個頻道只要用該頻道說話即可。\n");
        return 1;
    }
}

int help(object me)
{
    write(@HELP
指令格式﹕tune [<頻道名稱>]

這個指令讓你選擇是否要收聽某一頻道的訊息﹐如果沒有指定頻道名稱﹐就會列出
你目前收聽中的頻道﹐如果指定了頻道﹐原來收聽中的就會關掉﹐反之打開。

如果你對一個沒有收聽中的頻道講話﹐會自動將它打開。
要對一個頻道講話﹐只要用﹕

<頻道名稱> <訊息> ....

例子﹕
  chat hello everyone!
  
你可以用 help channels 查詢目前有哪些頻道可以收聽。

see also : shout
HELP
    );
    return 1;
}


