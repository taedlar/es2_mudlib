/*  berserk.c - berserk command

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

inherit F_CLEAN_UP;

void do_berserk(object);

private void create() { seteuid(getuid()); }

int
main(object me, string arg)
{
    object ob;
    mapping weapon;
    string skill;

    weapon = me->query_temp("weapon");

    if( me->is_busy() )
	return notify_fail("請先用 halt 停止你正在做的事。\n");

    /* NPC 不檢查這些限制 */
    if( userp(me) )
    {
	if( me->query_stat("fatigue") >= me->query_stat_maximum("fatigue") )
	    return notify_fail("你太累了，休息一會兒吧。\n");

	if( !me->query_learn("berserk", 1) )
	    return notify_fail("你不會這項技能。\n");

	if( !me->is_fighting() )
	    return notify_fail("你要在戰鬥中才能施展狂擊亂鬥之法！\n");

	if( mapp(weapon) && sizeof(weapon) ) {
	    foreach(skill, ob in weapon)
		if( skill[0..8] != "twohanded" )
		    return notify_fail("你必須手持雙握武器才能使用這種戰術。\n");
	}

	if( me->query_stat("gin") < me->query_stat_maximum("kee")/10 )
	    return notify_fail("你的精力不夠了。\n");

	me->supplement_stat("fatigue", 3);
    }

//    message_vision(HIR "\n$N一聲怒吼，大喊：「殺～～～～～！」\n\n" NOR,
//	me);
    do_berserk(me);

    return 1;
}

void
do_berserk(object me)
{
    object ob, *enemy;
    int skill, power, cost, max_attack, max_hit, penalty, exp, kee_cost;

    /* berserk 的花費看氣的強度而定，但消耗的是精力 */
    // 加上膂力和膽識的效用, 並整體上調降berserk的破壞力, 除了因為
    // berserk + powerblow 目前過於暴力外, 亦先預留未開放技能有再
    // 增強軍人攻擊力的空間, 如: leadership  -dragoon
    cost = (me->query_attr("str")+me->query_attr("cor")) / 3 +
	me->query_stat("kee") / 30;
    if( me->query_stat("gin") < cost ) {
	tell_object(me, "你的精不夠了。\n");
	return;
    }
    // 加上扣氣設定, 限制berserk在每次實戰上所能使用的上限次數
    // 避免sr可以在fr一招未發狀況下便打死對方
    kee_cost = me->query_stat_maximum("kee") / 8;
    if( me->query_stat("kee") < kee_cost ) {
	tell_object(me, "你的氣不夠了。\n");
	return;
    }

    message_vision(HIR "\n$N一聲怒吼，大喊：「殺～～～～～！」\n\n" NOR,
        me);

    /* 強度跟所消耗的氣有關，攻擊次數和技能有關 */
    skill = me->query_skill("berserk");
    power = cost * 3500;		/* 最大(粗估) 700Kg */
    max_attack = skill / 20;		/* 攻擊次數 4 - 9 */
    if( max_attack < 4 ) max_attack = 4;
    max_hit = skill / 45;		/* 最大 = 4 */
    if( max_hit < 1 ) max_hit = 1;

    me->add_temp("apply/attack", skill/4);
    me->add_temp("apply/strength", power);

    /* 先對付所有敵人，如果還有餘力，再多打幾輪 */
    enemy = me->query_enemy();
    while( max_hit-- && max_attack ) {
	foreach(ob in enemy) {
	    if( ! ob ) continue;	/* 如果敵人已經掛了，跳過他 :P */
	    // 若是敵人不在同一個房間, 跳過
	    if( environment(ob) != environment(me) ) continue;
	    me->attack(ob);
	    // 每出手攻擊一次均給予經驗值, 以鼓勵玩家在實戰中學習 berserk, 
	    // 經驗值與攻擊的技巧有關
	    exp = 1 + me->query_skill("berserk")/3
		+ random(me->query_skill("berserk"))/3;
 	    // 在技巧不足時, 膽識高對技能學習有正面幫助
 	    if( skill < 80 ) exp += random(me->query_attr("cor"));
	    me->improve_skill("berserk", exp);
	    if( --max_attack < 1 ) break;
	}
    }

    me->add_temp("apply/attack", - skill/4);
    me->add_temp("apply/strength", - power);

    /* 消耗體力 */
    if( userp(me) )
    {
	me->damage_stat("gin", cost/6);
//	me->consume_stat("gin", cost);
	me->consume_stat("kee", kee_cost);
	
	// 給予基本的使用經驗值, 與悟性相關
	if( me->query_skill("berserk", 1) < 180 )
	    me->improve_skill("berserk", 1 + random(me->query_attr("int"))*2);
	me->gain_score("combat", 1 + random(max_hit));

	me->start_busy(2);
    }
    else me->start_busy(1);
}

int help()
{
    write(@TEXT
指令格式：berserk

一種適合在兵荒馬亂的戰場或群毆之中使用的戰術，藉由暫時性的放棄守禦，以狂
風暴雨般的攻勢殺退敵人。

這種打法極其粗暴野蠻，而且消耗體力甚鉅，但是在面對武藝高強的敵人時往往容
易奏效，因為這種戰術毫無章法可言，難以防禦。
TEXT
    );
    return 1;
}
