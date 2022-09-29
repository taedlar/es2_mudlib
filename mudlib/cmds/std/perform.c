// cast.c

#include <skill.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object weapon;
    string martial_skill, act, trg;
    object target;

    if( me->is_busy() )
        return notify_fail("你現在沒有空﹗\n");
    
    if( environment(me)->query("no_fight") )
	return notify_fail("這裡不准戰鬥。\n");

    if( !me->is_fighting())
	return notify_fail("你必須在戰鬥中才能使用絕招！\n");

    if( !arg ) return notify_fail("指令格式﹕perform <技能:絕招> on <目標>\n");
    
    if( sscanf( arg, "%s:%s on %s", martial_skill, act, trg) != 3
    &&	sscanf( arg, "%s:%s", martial_skill, act) != 2 )
        return notify_fail("指令格式﹕perform <技能.絕招> [on <目標>]\n");
    
    if( trg ) {
        target = present(trg, environment(me));
        if( !target ) return notify_fail("你要對誰使用招式？\n");
        if( !living(target) ) return notify_fail("你可以直接宰了這個昏迷不醒的傢伙，不需要用絕招。\n");
    }

    if( !me->query_temp("weapon/" + martial_skill ) 
        && (martial_skill != "unarmed") )
        return notify_fail("你必須裝備適當的武器來使出絕招。\n");

        notify_fail("你請先用 enable 指令選擇你要使用的技能。\n");

    if( (martial_skill != me->skill_mapped( martial_skill ))
    &&	(me->skill_mapped(martial_skill) != 0))
        return SKILL_D(me->skill_mapped(martial_skill))->perform_action(me, act, target);

    return 0;

}

int help (object me)
{
    write(@HELP
指令格式﹕perform <技能:絕招> [on <施招對象>]

在戰鬥中使用所學技能的絕招來和施招對象對戰，絕招可以參考
個別的技能所提供的HELP。

例如： perform sword:snowblade on guard

就是指定所 enable 的劍法的 snowblade 這招來對 guard 施展。

在你使用某一個技能的絕招之前﹐你必須先用 enable 指令來指定你要使用的技能。
 
HELP
    );
    return 1;
}
