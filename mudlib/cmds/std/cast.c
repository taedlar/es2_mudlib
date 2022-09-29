/*  cast - the spell-casting command

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

int main(object me, string arg)
{
    string spells, spl, trg;
    object target;
        
    if( me->query("life_form") == "ghost" )
        return notify_fail("人死了就一了百了, 快去找城隍復活吧!!\n");

    if( me->is_busy() ) return notify_fail("你現在沒有空！\n");

    if( !arg ) return notify_fail("指令格式﹕cast <咒文名稱> [on <施咒對象>]\n");

    if( sscanf(arg, "%s on %s", spl, trg)==2 ) {
	target = present(trg, environment(me));
	if( !target ) target = present(trg, me);
	if( !target ) return notify_fail("這裡沒有 " + trg + "。\n");

	// add by dragoon, modified by annihilator
	if( userp(target) && !interactive(target) ) {
	     write("你不能對斷線中的人施法。\n");
	     return 1;
	}
    } else {
	spl = arg;
	target = 0;
    }

    if( environment(me)->query("no_magic")
    && !me->is_fighting(target)
    && !target->query("unprotect_mark") ) {
	    tell_object(me, "這裡不准戰鬥。\n");
	    return 1;
    }

    spl = replace_string( spl, " ", "_");

    notify_fail("你請先用 enable 指令選擇你要使用的咒文系。\n");
    if( stringp(spells = me->skill_mapped("spells")) )
	return (int)SKILL_D(spells)->cast_spell(me, spl, target);

    return 0;
}

int help (object me)
{
    write(@HELP
指令格式﹕cast <咒文名稱> [on <施咒對象>]

施展法術，你必需要指定<咒文名稱>，<施咒對象>若已處於戰門狀態則可省略。

在你使用某一個咒文之前，你必須先用 enable 指令來指定你要使用的咒文系，並
使用的該咒文系中的咒文，咒文名稱可參考 help taoism。

大多數的法術從你開始吟誦咒文，到完成法術會需要一段時間。這段時間施法者會
無法移動或攻擊。
HELP
    );
    return 1;
}
