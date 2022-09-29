/*  exert.c

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

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    int ratio;
    string f, dest;
    object target;

    if( !(f = me->skill_mapped("force")) )
        return notify_fail("你現在並沒有使用任何內功\。\n");
    if( f=="force" )
        return notify_fail("你必須先 enable 一種內功\。\n");

    if( !arg ) {
        ratio = me->query("force_ratio");
        if( !ratio ) ratio = 75;
        printf("你在作戰時將使出%s成%s的功\力。\n",
            chinese_number(ratio/10), (ratio%10==0 ? "" : chinese_number(ratio%10)));
        return 1;
    }

    if( sscanf(arg, "%d", ratio)==1 ) {
        if( ratio < 10 || ratio > 100 )
            return notify_fail("你能控制的範圍是在一成到十成功\力之間。\n");
        me->set("force_ratio", ratio);
        write("Ok.\n");
        return 1;
    }

    if( me->is_busy() )
        return notify_fail("你必須先用 halt 停止你正在做的事﹗\n");

    if( sscanf(arg, "%s on %s", arg, dest)==2 ) {
        target = present(dest, environment(me));
//        if( !target || !target->is_target() )
//            return notify_fail("這裡沒有這個人。\n");
// edit by ueiren .....cant find is_target()
	if( !target ) target = present( dest, me );
	if( !target ) return notify_fail("這裡沒有這個人。\n");
    }
    notify_fail("你所使用的內功\沒有這種功\能。\n");
    return SKILL_D(me->skill_mapped("force"))->exert_function(me, arg, target);
}

int help(object me)
{
    write(@TEXT
指令格式﹕exert [<功能> [on <目標>]]
          exert <功力百分比(10-100)>
          exert

使用內功的某些功能﹐或設定在戰鬥中要使出幾成功力﹐exert 100 表示你將使出全力
戰鬥﹐當你技能不高的時候你對功力的控制將比較不能收發自如﹐使出全力戰鬥將使你
運用內功提昇戰鬥時的攻擊命中率、破壞力、與防禦力﹐但是相對的力氣的消耗也相當
大﹐面對較弱的敵人時你可以選擇使用五、六成功力以節省體力。

不加任何參數時會顯示你現在的設定。

使用內功技能時所用的功能隨你學習的內功而異﹐通常你可以用 exert <內功名稱> 來
修煉內功﹐不過有些特殊內功的修煉方法可能會有不同。

TEXT
    );
    return 1;
}
