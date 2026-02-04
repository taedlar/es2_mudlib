// vim: set ts=4 sw=4 syntax=lpc

// inherit F_CLEAN_UP;
// Don't want this command be cleaned up while typing long string.

int main(object me, string arg)
{
    me->edit( (: call_other, __FILE__, "do_to", me, arg :) );
    return 1;
}

void do_to(object me, string arg, string str)
{
    seteuid(getuid());
    if(arg) me->force_me(arg + " " + str);
    else me->force_me(str);
}

int help(object me)
{
write(@HELP
指令格式 : to <指令>

可以一次輸出多行以上的文字, <指令>通常是輸出訊息的指令如
say , tell , describe , chat , reple ........等。
範例 : 
 to say
 結束離開用 '.'﹐取消輸入用 '~q'﹐使用內建列編輯器用 '~e'。
 ─────────────────────────────
 東方神教教主 : 安尼席洛特爾
 教主大夫人暨發呆部部長 : Elon
 教主小老婆暨摸魚部部長 : 夢憶柔
 大弟子暨打混部部長 : 風火血舞
 .

輸出到螢幕上的結果則是 :

你說道﹕東方神教教主 : 安尼席洛特爾
教主大夫人暨發呆部部長 : Elon
教主小老婆暨摸魚部部長 : 夢憶柔
大弟子暨打混部部長 : 風火血舞
HELP
    );
    return 1;
}


