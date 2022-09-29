/*  estimate.c

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
    object ob = me;
    int my_ability, my_defense, my_strength;
    int hir_ability, hir_defense, hir_strength;
    int cmp_ability, cmp_defense, cmp_strength;
    mapping weapon;
    string msg;

    if( arg ) {
	ob = present(arg, environment(me));
	if( !ob || !ob->is_character() || !ob->visible(me) )
	    return notify_fail("這裡沒有 " + arg + " 這個人。\n");
    }

    // Calculate my attack ability

    my_ability = me->query_ability("attack");
    if( mapp(weapon = me->query_temp("weapon")) && sizeof(weapon) ) {
	int v, best = 0;
	string sk;
	object foo;

	foreach(sk, foo in weapon)
	    if( (v=me->query_skill(sk)) > best ) best = v;
	my_ability += best;
    }
    else my_ability += me->query_skill("unarmed");
    if( ! my_ability ) my_ability = 1;

    // Calculate my defense ability

    my_defense = me->query_ability("defense");
    if( me->skill_mapped("dodge") ) my_defense += me->query_skill("dodge");
    if( ! my_defense ) my_defense = 1;

    // Calculate my attack strength

    my_strength = me->query_strength("attack");
    if( me->skill_mapped("force") != "force" )
	my_strength += me->query_skill("force") * me->query_stat("kee") * 4;
    if( ! my_strength ) my_strength = 1;

    if( ob==me ) {
	printf("你評估自己在武功\招數上約有 %d 的程度。\n", my_ability);
	printf("守禦方面應該有 %d 的水準。\n", my_defense);
	printf("全力一擊的話能有 %.2f 公斤左右的力道。\n", my_strength / 1000.0);
	return 1;
    }

    // Calculate target attack ability

    hir_ability = ob->query_ability("attack");
    if( mapp(weapon = ob->query_temp("weapon")) && sizeof(weapon) ) {
	int v, best = 0;
	string sk;
	object foo;

	foreach(sk, foo in weapon)
	    if( (v=ob->query_skill(sk)) > best ) best = v;
	hir_ability += best;
    }
    else hir_ability += ob->query_skill("unarmed");

    // Calculate my defense ability

    hir_defense = ob->query_ability("defense");
    if( ob->skill_mapped("dodge") ) hir_defense += ob->query_skill("dodge");

    // Calculate target attack strength

    hir_strength = ob->query_strength("attack");
    if( ob->skill_mapped("force") != "force" )
        hir_strength += ob->query_skill("force") * ob->query_stat("kee") * 4;

    // Give remarks.

    if( my_ability < 1 ) my_ability = 1;
    if( my_strength < 1 ) my_strength = 1;
    if( my_defense < 1 ) my_defense = 1;

    cmp_ability  = (my_ability - hir_ability) * 100 / my_ability;
    cmp_strength = (my_strength - hir_strength) * 100 / my_strength;
    cmp_defense  = (my_defense - hir_defense) * 100 / my_defense;

    msg = "你評估" + ob->name() + "的武功\在招數上";

    if( cmp_ability < -150 )
        msg += "遠勝於你，";
    else if( cmp_ability < -80 )
        msg += "高出你甚多，";
    else if( cmp_ability < -40 )
        msg += "高出你一截，";
    else if( cmp_ability < -10 )
        msg += "比你略勝一籌，";
    else if( cmp_ability < 10 )
        msg += "與你只在伯仲之間，";
    else if( cmp_ability < 40 )
        msg += "略遜你一籌，";
    else if( cmp_ability < 80 )
        msg += "和你相比頗有不如，";
    else if( cmp_ability < 150 )
        msg += "和你相比差著老大一截，";
    else
        msg += "對你而言毫不構成威脅，";

    msg += "在守禦方面";

    if( cmp_defense < -150 )
        msg += "遠勝於你，";
    else if( cmp_defense < -80 )
        msg += "高出你甚多，";
    else if( cmp_defense < -40 )
        msg += "高出你一截，";
    else if( cmp_defense < -10 )
        msg += "比你略勝一籌，";
    else if( cmp_defense < 10 )
        msg += "與你只在伯仲之間，";
    else if( cmp_defense < 40 )
        msg += "略遜你一籌，";
    else if( cmp_defense < 80 )
        msg += "和你相比頗有不如，";
    else if( cmp_defense < 150 )
        msg += "和你相比差著老大一截，";
    else
        msg += "對你而言毫不構成威脅，";

    if( cmp_defense * cmp_strength > 0 )
        msg += "而且在力道上也";
    else
        msg += "但是在力道上卻";

    if( cmp_strength < -150 )
        msg += "遠勝於你。\n";
    else if( cmp_strength < -80 )
        msg += "強過你甚多。\n";
    else if( cmp_strength < -40 )
        msg += "強過你一大截。\n";
    else if( cmp_strength < -10 )
        msg += "比你略勝一籌。\n";
    else if( cmp_strength < 10 )
        msg += "與你在伯仲之間。\n";
    else if( cmp_strength < 40 )
        msg += "略遜你一籌。\n";
    else if( cmp_strength < 80 )
        msg += "頗不如你。\n";
    else if( cmp_strength < 150 )
        msg += "差著你老大一截。\n";
    else
        msg += "對你而言不值一訕。\n";

    write(msg);
    return 1;
}

int help()
{
    write(@TEXT
指令格式：estimate [<對象>]

這個指令讓你大略評估一個對手的實力，如果不加對象，則是評估自己的武藝。
TEXT
    );
    return 1;
}
