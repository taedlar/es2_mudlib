/*  acupuncture.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <skill.h>

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string cauterization, spl, trg;
    object target, weapon;
       
    weapon = me->query_temp("weapon/needle");

    if( me->query("life_form") == "ghost" )
        return notify_fail("人死了就一了百了, 快去找城隍復活吧!!\n");

    if(!weapon) return notify_fail("你必須裝備針類醫具才可使用針灸之術。\n");
        
    if( me->is_busy() )
        return notify_fail("你現在沒有空﹗\n");

    if( !arg ) return notify_fail("指令格式﹕acupuncture <灸術> [to <目標>]\n");
    if( sscanf(arg, "%s to %s", spl, trg)==2 ) {
        target = present(trg, environment(me));
        if( !target ) target = present(trg, me);
        if( !target ) return notify_fail("這裡沒有 " + trg + "。\n");
        // add by dragoon
        if( userp(target) && !interactive(target) )
            return notify_fail("這人現在不在線上。\n");
    } else {
        spl = arg;
        target = 0;
    }

    spl = replace_string( spl, " ", "_");

    notify_fail("你請先用 enable 指令選擇你要使用的灸術系。\n");
    if( stringp(cauterization = me->skill_mapped("cauterization")) ) {
        me->set_temp("pending/hidden", 0);
        return (int)SKILL_D(cauterization)->acupuncture_cauterization(me, 
                spl, target);
    } 
    return 0;
}

int help (object me)
{
    write(@HELP
指令格式﹕acupuncture <灸術名稱> [to <對象>]
 
使用灸術﹐你必需指定<灸術名稱>﹐<施灸對象>則可有可無。
在你使用某一個灸術之前﹐你必須先用 enable 指令來指定你
要使用的灸術系, 詳細的設定請參考 help cauterization 。
 
HELP
    );
    return 1;
}
