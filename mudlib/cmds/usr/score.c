/*  score.c - display informations of a user

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <ansi.h>
#include <combat.h>

#define	SCORE_COLUMN	2

inherit F_CLEAN_UP;

string bar_string
	= "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■";
string blank_string
	= "□□□□□□□□□□□□□□□□□□□□□□□□□□□□□";

string display_attr(int gift, int value);
string status_color(int current, int max);
string tribar_graph(int val, int eff, int max);

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    object ob;
    mapping score;
    string line, tmp, lform;
    int i;

    if(!arg) ob = me;
    else {
	if( !wizardp(me) ) return notify_fail("只有巫師能察看別人的狀態。\n");
	ob = present(arg, environment(me));
	if (!ob) ob = find_player(arg);
	if (!ob) ob = find_living(arg);
	if (!ob) return notify_fail("你要察看誰的狀態﹖\n");
/*  將check wiz 移至前面 -dragoon
	if (!wizardp(me) && !ob->accept_info(me, "score") )
	    return notify_fail("只有巫師能察看別人的狀態。\n");
*/
    }

    line = sprintf( HIW "【 %s 】" NOR "%s\n\n", ob->rank(), ob->short(1) );
    line += break_chinese_string(
	sprintf(" %s是一%s%s歲的 %2d 級%s%s%s﹐出生於%s，目前的業力是 %d 點。",
		ob==me ? gender_self(ob) : gender_pronoun(ob),
		(tmp = ob->query("unit")) ? tmp : "個",
		chinese_number(ob->query("age")),
		ob->query_level(),
		(tmp = ob->query("gender")) ? to_chinese(tmp) : "",
		to_chinese(ob->query_race()),
		ob->query("humanoid") ? ob->rank(0, 1) : "",
		CHINESE_D->chinese_date(((int)ob->query("birthday") - 14*365*24*60) * 60),
		ob->link() ? ob->link()->query("karma") : 0 ),
        	68, " ") + "\n\n";

    if( wizardp(me) || (int)me->query_level() > 1 ) {
        line = sprintf(
            "%s 膂力 %s   膽識 %s   悟性 %s   靈性 %s\n"
              " 定力 %s   機敏 %s   根骨 %s   慧根 %s\n\n", line,
            display_attr(ob->query_attr("str",1), ob->query_attr("str")),
            display_attr(ob->query_attr("cor",1), ob->query_attr("cor")),
            display_attr(ob->query_attr("int",1), ob->query_attr("int")),
            display_attr(ob->query_attr("spi",1), ob->query_attr("spi")),
            display_attr(ob->query_attr("cps",1), ob->query_attr("cps")),
            display_attr(ob->query_attr("dex",1), ob->query_attr("dex")),
            display_attr(ob->query_attr("con",1), ob->query_attr("con")),
            display_attr(ob->query_attr("wis",1), ob->query_attr("wis")));
    }

    line = sprintf("%s 形體 %s%4d/%4d  " NOR HIY "%s\n\n" NOR, line,
        status_color(ob->query_stat("HP"), ob->query_stat_maximum("HP")),
        ob->query_stat("HP"), ob->query_stat_maximum("HP"),
        tribar_graph(ob->query_stat("HP"), ob->query_stat_effective("HP"), ob->query_stat_maximum("HP")) );

    if( ob->query_stat_maximum("gin") )
    line = sprintf("%s 精   %s%4d/%4d  " NOR GRN "%s\n" NOR, line,
	status_color(ob->query_stat("gin"), ob->query_stat_maximum("gin")),
	ob->query_stat("gin"),    ob->query_stat_maximum("gin"),
	tribar_graph(ob->query_stat("gin"),    ob->query_stat_effective("gin"), ob->query_stat_maximum("gin")) );
    if( ob->query_stat_maximum("kee") )
    line = sprintf("%s 氣   %s%4d/%4d  " HIR "%s\n" NOR, line,
	status_color(ob->query_stat("kee"), ob->query_stat_maximum("kee")),
	ob->query_stat("kee"), ob->query_stat_maximum("kee"),
	tribar_graph(ob->query_stat("kee"),    ob->query_stat_effective("kee"), ob->query_stat_maximum("kee")) );
    if( ob->query_stat_maximum("sen") )
    line = sprintf("%s 神   %s%4d/%4d  " HIB "%s\n" NOR, line,
	status_color(ob->query_stat("sen"), ob->query_stat_maximum("sen")),
	ob->query_stat("sen"), ob->query_stat_maximum("sen"),
	tribar_graph(ob->query_stat("sen"),    ob->query_stat_effective("sen"), ob->query_stat_maximum("sen")) );

    line = sprintf("%s\n 食物 %s%4d/%4d" NOR "\t\t飲水 %s%4d/%4d" NOR "\t\t疲勞 %s%4d/%4d\n" NOR, line,
	status_color(ob->query_stat("food"), ob->query_stat_maximum("food")),
	ob->query_stat("food"), ob->query_stat_maximum("food"),
	status_color(ob->query_stat("water"), ob->query_stat_maximum("water")),
	ob->query_stat("water"), ob->query_stat_maximum("water"),
	status_color(ob->query_stat_maximum("fatigue") - ob->query_stat("fatigue"), ob->query_stat_maximum("fatigue")),
	ob->query_stat("fatigue"), ob->query_stat_maximum("fatigue") );

    score = ob->query("score");
    if( mapp(score) )
    {
	string c;
	int xp;

	line += "\n 人物經驗/評價﹕\n   ";
	i = 0;
	foreach(c, xp in score)
	{
	    int xp_t;

	    if( xp >= 0 && (xp_t = ob->query_target_score(c) - xp) > 0 ) 
		line = sprintf("%s" HIM "%-8s %8d %-9s " NOR, line,
			to_chinese("score of " + c), xp, "[" + xp_t + "]");
	    else
		line = sprintf("%s%-8s %8d %9s ", line,
			to_chinese("score of " + c), xp, "");
	    if( (i++)%SCORE_COLUMN == SCORE_COLUMN-1 ) line += "\n   ";
	}
	if( i%SCORE_COLUMN != SCORE_COLUMN-1 ) line += "\n";
	line += "\n";
    }

    write(line);
    return 1;
}

string
display_attr(int gift, int value)
{
    if( value > gift )
        return sprintf( HIC "%3d%5s" NOR, value, "(+" + (value - gift) + ")" );
    else if( value < gift )
        return sprintf( HIB "%3d%5s" NOR, value, "(" + (value - gift) + ")" );
    else
        return sprintf( CYN "%3d     " NOR, value);
}

string
status_color(int current, int max)
{
    if( (max<=0) || (current > max) )
	return HIC;		/* > 100%	青色 */

    switch(current*10/max) {
    case 10: case 9:		/* 100% - 90%	綠色 */
	return HIG;
    case 8: case 7: case 6:	/* 90% - 60%	黃色 */
	return HIY;
    case 5: case 4: case 3:	/* 60% - 30%	暗黃色 */
	return YEL;
    case 2: case 1:		/* 30% - 10%	紅色 */
	return HIR;
    default:			/* < 10%	暗紅色 */
	return RED;
    }
}

string
tribar_graph(int val, int eff, int max)
{
    int n_filled, n_empty;
    string bar = "";

    if( (max <= 0) || (val <= 0) || (eff <= 0) ) return "";

    n_filled = val * 25 / max;
    n_empty = eff * 25 / max - n_filled;

    /* 如果狀態值與目前值都非常低(但不是 0)，顯示一個空格 */
    if( ! n_filled && ! n_empty ) n_empty = 1;

    if( n_filled ) bar += bar_string[0..n_filled*2-1];
    if( n_empty ) bar += blank_string[0..n_empty*2-1];

    return bar;
}

int
help(object me)
{
        write(@HELP
指令格式 : score
           score <對象名稱>                   (巫師專用)
 
這個指令可以顯示你(妳)或指定對象(含怪物)的基本資料。
基本資料的設定請參閱 'help setup'。
 
其他相關指令﹕hp
HELP
    );
    return 1;
}
