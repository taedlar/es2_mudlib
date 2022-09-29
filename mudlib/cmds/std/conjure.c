// conjure.c

#include <skill.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	string magic, spl, trg;
	object target;
	

	if( me->is_busy() )
		return notify_fail("你現在沒有空﹗\n");

	if( environment(me)->query("no_magic") )
		return notify_fail("這裡不准施展法術。\n");

	if( !arg ) return notify_fail("指令格式﹕conjure <法術> [on <目標>]\n");
	if( sscanf(arg, "%s on %s", spl, trg)==2 ) {
		target = present(trg, environment(me));
		if( !target ) target = present(trg, me);
		if( !target ) return notify_fail("這裡沒有 " + trg + "。\n");
	} else {
		spl = arg;
		target = 0;
	}

	spl = replace_string( spl, " ", "_");

	notify_fail("你請先用 enable 指令選擇你要使用的法術系。\n");
	if( stringp(magic = me->skill_mapped("magic")) )
		return (int)SKILL_D(magic)->conjure_magic(me, spl, target);
		
	return 0;
}

int help (object me)
{
        write(@HELP
指令格式﹕conjure <法術名稱> [on <對象>]
 
施法﹐你必需要指定<法術名稱>﹐<施咒對象>則可有可無。
在你使用某一個法術之前﹐你必須先用 enable 指令來指定
你要使用的法術系。可使用的法術系參考 help alchemy。
 
HELP
        );
        return 1;
}
