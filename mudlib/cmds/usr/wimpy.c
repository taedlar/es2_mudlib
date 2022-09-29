/*  wimpy.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <statistic.h>

inherit F_CLEAN_UP;

void create() {seteuid(getuid());}

int main(object me, string arg)
{
    string stat, dir;
    int ratio;

    if( !arg )
    {
	string* stat_name = me->query_stat_name();
	string msg = "你的各項指數在低於下列比例時會自動逃命：\n";
	if( arrayp(stat_name) && sizeof(stat_name) )
	    foreach(stat in stat_name)
	    {
		ratio = me->query_stat_notify(stat);
		if( ratio > 0 )
		    msg += sprintf("  %s\t%d%%\n", to_chinese(stat), ratio);
	    }
        else
            msg += "沒有設定。\n";
        if( dir = me->query("wimpy_dir") )
            msg += "你的預設逃跑方向是 " + dir + "。\n";
        write(msg);
        return 1;
    }

    if( sscanf(arg, "%s at %d", stat, ratio) == 2 )
    {
	if( !me->query_stat_maximum(stat) )
            return notify_fail("你沒有這項指數項目。\n");

        if( (ratio < 0) || (ratio > 80) )
            return notify_fail("比例必須在 0 到 80 之間。\n");

        me->set_stat_notify(stat, ratio);
        write("Ok。\n");

        return 1;
    }
    else if( sscanf(arg, "to %s", dir) ) {
        me->set("wimpy_dir", dir);
        write("Ok。\n");
        return 1;
    }
    else if( arg=="random" ) {
        me->delete("wimpy_dir");
        write("Ok.\n");
        return 1;
    }

    return notify_fail("指令格式﹕wimpy <指數項目> at <比例>\n");

}

int help()
{
    write(@TEXT
指令格式﹕wimpy <指數項目> at <百分比>
          wimpy to <方向>
          wimpy random

這個指令讓你設定在人物的某項指數低於一個百分比的時候﹐自動逃跑。
(每個指數項目都可以設﹐只要其中一個低於你設的百分比﹐就會逃跑)

可以設定的指數項目包括﹕

HP        生命點數
gin       精
kee       氣
sen       神
food      食物
water     飲水

wimpy to <方向> 可以讓你指定逃跑時先往哪一個方向跑。
wimpy random 可以取消原先這定的逃跑方向。
TEXT
    );
    return 1;
}
