/*  npc.c - NPC standard object

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
#include <dbase.h>
#include <localtime.h>
#include <command.h>
#include <skill.h>
#include <type.h>

inherit CHARACTER;
inherit F_CLEAN_UP;     // Only to clean up master copy of NPCs.

static mixed next_chat;
static int last_scheduled_time_tag = 0;

int chat();

static void
heart_beat()
{
    mapping schedule;

    ::heart_beat();

    if( ! this_object() || ! living(this_object()) ) return;

    if( !is_busy() ) chat();

    if( living(this_object())
    &&	clonep(this_object())
    &&	mapp(schedule = query("schedule")) ) {
	mapping gt;
	int time_tag;

	gt = NATURE_D->game_time(1);
	time_tag = gt[LT_HOUR] * 100 + (gt[LT_MIN] / 10) * 10;
	if( time_tag != last_scheduled_time_tag ) {
	    evaluate(schedule[time_tag]);
	    last_scheduled_time_tag = time_tag;
	}
	if( !this_object() ) return;
    }
}

void
die()
{
    object ob;
    mapping bounty;

    if( objectp(ob = last_damage_giver())
    &&	ob != this_object()
    &&	mapp(bounty = query("bounty")) )
    {
	string score;
	int amount;

	foreach(score, amount in bounty)
	    ob->gain_score(score, amount);
    }

    ::die();
}

mixed
carry_object(string file)
{
    object ob;

    // Don't let master copy clone equips
    if( !clonep() ) return VOID_OB;

    if( !objectp(ob = new(file)) ) return 0;

    // Support of uniqueness.
    if( ob->violate_unique() ) ob = ob->create_replica();
	if( !ob ) return VOID_OB;

    ob->varify_template(this_object());
    ob->move(this_object());

    return ob;
}

object
carry_money(string type, int amount)
{
    object ob;

    ob = carry_object("/obj/money/" + type);
    if( !ob ) return 0;
    ob->set_amount(amount);
}

int is_chatting() { return functionp(next_chat) != 0; }

int is_npc() { return 1; }

void do_chat(mixed c) { next_chat = c; }

mixed
eval_chat(mixed chat)
{
    mixed ret;

    chat = evaluate(chat);
    switch(typeof(chat))
    {
    case STRING:
	say(CYN + chat + NOR);
	return 0;
    case ARRAY:
	if( !sizeof(chat) ) return 0;
	chat[0] =  eval_chat(chat[0]);
	return chat - ({ 0 });
    case FUNCTION:
	return chat;
    default:
	return 0;
    }
}

int
chat()
{
    string *msg;
    mixed ret;
    int chance, rnd;

    if( !environment() ) return 0;

    // Evaluate programmed chat first.
    if( next_chat ) {
	next_chat = eval_chat(next_chat);
	return 1;
    }

    // Else, do random chat if any.
    if( ! (chance = (int)query(is_fighting()? "chat_chance_combat": "chat_chance")) )
	return 0;

    if( arrayp(msg = query(is_fighting()? "chat_msg_combat": "chat_msg"))
    &&	sizeof(msg) ) {
	if( random(100) > chance ) return 0;
	rnd = random(sizeof(msg));
	if( stringp(msg[rnd]) )
	    say(CYN + msg[rnd] + NOR);
	else if( functionp(msg[rnd]) )
	    evaluate(msg[rnd]);
	return 1;
    }
}

// -----------------------
// Standard chat functions
// -----------------------

// random_move() : Move NPC randomly to adjacent room

int random_move()
{
    mapping exits;
    string *dirs;

    if( !mapp(exits = environment()->query("exits")) ) return 0;
    dirs = keys(exits);
    GO_CMD->main(this_object(), dirs[random(sizeof(dirs))]);
}

// cast_spell() : Cause the NPC to cast a specific spell

void cast_spell(string spell)
{
    string spell_skill;

    if( stringp(spell_skill = skill_mapped("spells")))
	SKILL_D(spell_skill)->cast_spell(this_object(), spell);
}

// conjure_magic() : Cuase the NPC to conjure a specific magic power

void conjure_magic(string magic)
{
    string magic_skill;

    if( stringp(magic_skill  = skill_mapped("magic")))
	SKILL_D(magic_skill)->conjure_magic(this_object(), magic);
}

void acupuncture_cauterization(string cauterization)
{
    string cauterization_skill;

    if( stringp(cauterization_skill  = skill_mapped("cauterization")))
	SKILL_D(cauterization_skill)->acupuncture_cauterization(this_object(), cauterization);
}

// exert_function() : Cause the NPC to exert a specific martial skill

int exert_function(string func)
{
    string force_skill;

    if( stringp(force_skill = skill_mapped("force")))
	SKILL_D(force_skill)->exert_function(this_object(), func);
}

// perform_action() : Cause the NPC to perform a specific function of a skill

int perform_action(string skill, string action)
{
    if( stringp(skill) && stringp(action) )
	SKILL_D(skill)->perform_action(this_object(),
		action, query_opponent());
}

// This overrides default activate_guard() in F_ATTACK.

void activate_guard(object target)
{
    kill_ob(target);
}

void do_heal()
{
#if 0
    heal_stat("gin", random(150));
    supplement_stat("gin", random(150));
    heal_stat("kee", random(150));
    supplement_stat("kee", random(150));
    heal_stat("sen", random(150));
    supplement_stat("sen", random(150));
    heal_stat("HP", random(20));
    supplement_stat("HP", random(20));
#endif
}

varargs void
improve_skill(string skill, int amount)
{
    /* 重定義成不做任何事，以節省系統資源 */
}

void
gain_score(string term, int amount)
{
    /* 重定義成不做任何事，以節省系統資源 */
}
