/*  gain.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    string term, txt;
    int g;
    mapping score_g, skill_g;

    score_g = me->query_score_gain();
    me->reset_score_gain();
    skill_g = me->query_skill_gain();
    me->reset_skill_gain();

    txt = "從你上次使用 gain 指令，到現在為止，你共獲得：\n\n";
    
    txt += "經驗點數：\n";
    if( !sizeof(score_g) )
	txt += "    無。\n";
    else {
	foreach(term, g in score_g)
	    txt += sprintf("    %-16s %+d 點\n",
		to_chinese("score of " + term), score_g[term] );
    }

    txt += "\n技能點數：\n";
    if( !sizeof(skill_g) )
	txt += "    無。\n";
    else {
	foreach(term, g in skill_g)
	    txt += sprintf("    %-16s %+d 點\n",
		to_chinese(term), skill_g[term] );
    }

    write(txt);
    return 1;    
}

int help()
{
    write(@TEXT
指令格式：gain

這個指令可以用來檢驗你的人物成長，每次你的人物獲得任何經驗點數或技能點數，系
統會記錄所獲得的值，當你使用 gain 指令，會顯示這些值，然後清除這些紀錄從頭開
始，你可以在戰鬥開始前先用 gain 清除，然後在戰鬥結束後用 gain 檢視你的人物從
剛剛的戰鬥中獲的多少進步。
TEXT
    );
    return 1;
}
