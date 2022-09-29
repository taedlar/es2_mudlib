/*  dragon_force.c

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

inherit SKILL;

void create()
{
    seteuid(getuid());
    DAEMON_D->register_skill_daemon("dragon force");
}

int valid_enable(string usage)
{
    return usage == "force";
}

void skill_improved(object me, string sk)
{
    int skill, learn;

    skill = me->query_skill("dragon force", 1);
    learn = me->query_learn("dragon force", 1);

    if( !skill ) {
	if( learn >= 1500 ) {
	    tell_object(me,
		HIY "你覺得腹中一股暖洋洋的熱氣忽然膨脹，立刻充滿了全身各處，看來你的\n"
		    "龍圖心經已經練成了﹗" NOR);
	    me->advance_skill("dragon force", 40);
	    me->gain_score("martial art", 500);
	}
	return;
    }

    if( (skill < 120) && (skill-39) * (skill-39) * 200 < learn - 1500 ) {
	me->advance_skill("dragon force", 1);
	me->gain_score("martial art", (skill+1) * 10);
	if( skill >= 50 )
	    me->gain_score("martial mastery", (skill-40) * 10);
    }
}

void skill_advanced(object me, string sk)
{
    int level;

    level = me->query_skill(sk, 1);
    if( me->query_stat_maximum("gin") < level * 13 )
	me->advance_stat("gin", 5);
    if( me->query_stat_maximum("kee") < level * 13 )
	me->advance_stat("kee", 5);
    if( me->query_stat_maximum("sen") < level * 13 )
	me->advance_stat("sen", 5);
}

int do_exercise(object me)
{
    if( me->query_stat("gin") < 10
    ||    me->query_stat("kee") < 10
    ||    me->query_stat("sen") < 10 ) {
	tell_object(me, "你覺得神困力乏﹐沒有辦法繼續練功\了。\n");
	me->interrupt_me(me, "exhausted");
	return 1;
    }
    me->consume_stat("gin", 10);
    me->consume_stat("kee", 10);
    me->consume_stat("sen", 10);

    if( random(me->query_attr("cps") + me->query_attr("con") + me->query_skill("force", 1)) > 20 ) {
	me->damage_stat("gin", 1);
	me->damage_stat("kee", 1);
	me->damage_stat("sen", 1);
	me->improve_skill("dragon force", random(me->query_attr("con")/10) + 1);
	me->improve_skill("force", random(me->query_attr("int")/10) + 1);
    }
    return 1;
}

int halt_exercise(object me, object owner, object from, string how)
{
    return 1;
}

varargs int
exert_function(object me, string func, object target)
{
    mapping gt;

    switch(func) {
    case "dragon force":
	message_vision(HIY "$N盤膝而坐，深深地吸了口氣，開始修習龍圖心經的內功\。\n" NOR,
	    me);
	me->start_busy((: do_exercise, me :), (: halt_exercise, me :));
	return 1;
    default:
	return notify_fail("龍圖心經沒有這種功\能。\n");
    }
}
