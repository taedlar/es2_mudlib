/*  combatd.c - combat arbitrator daemon

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

// MudOS v21.6 #pragma optimize's jump threading has bug! (line 200)
//#pragma optimize

#pragma save_binary

#include <ansi.h>
#include <combat.h>

//#define SHOW_DAMAGE
//#define SHOW_COMBAT_STAT

inherit F_CLEAN_UP;
inherit F_DBASE;

string *catch_hunt_msg = ({
    HIW "$N和$n仇人相見分外眼紅，立刻打了起來﹗\n" NOR,
    HIW "$N對著$n大喝﹕「可惡，又是你﹗」\n" NOR,
    HIW "$N和$n一碰面，二話不說就打了起來﹗\n" NOR,
    HIW "$N一眼瞥見$n，「哼」的一聲衝了過來﹗\n" NOR,
    HIW "$N喝道﹕「狗賊﹗別跑」\n" NOR,
    HIW "$N喝道﹕「納命來﹗」\n" NOR
});

string *dead_msg = ({
    "\n$N死了。\n\n",
    "\n$N吐出幾口鮮血，抽搐了幾下 ... 死了。\n\n",
    "\n$N倒臥在血泊中，嘴角流出鮮血，眼看是不活了。\n\n",
    "\n$N呻吟了幾聲，咳出一口鮮血，便斷了氣。\n\n",
    "\n$N趴在地上，無力地掙扎了一會兒，隨即死去。\n\n",
});

void create()
{
    seteuid(getuid());
    set("name", "戰鬥精靈");
    set("id", "combatd");
}

string damage_msg(int damage, string type)
{
    string str;

    if( !type ) type = "傷害";
#ifdef SHOW_DAMAGE
    return "結果造成 " + damage + " 點" + type + "。\n";
#else
    if( damage == -1 ) switch(type) {
    case "瘀傷":
        return "結果「砰」地一聲打個正著。\n";
    case "割傷":
        return "結果「刷」地一聲砍出一道缺口。\n";
    case "刺傷":
        return "結果「噗」地一聲刺了進去。\n";
    default:
        return "結果直接命中$n。\n";
    }

    if( damage == 0 ) return "結果似乎沒有造成明顯的傷害。\n";
    if( damage > 100 ) damage = 100;

    switch( type ) {
    case "斲傷":
    case "割傷":
        if( damage < 5 ) return "結果只是輕輕地劃破$p的皮肉。\n";
        else if( damage < 10 ) return "結果在$p$l劃出一道細長的血痕。\n";
        else if( damage < 20 ) return "結果「嗤」地一聲劃出一道傷口﹗\n";
        else if( damage < 40 ) return "結果「嗤」地一聲劃出一道血淋淋的傷口﹗\n";
        else if( damage < 60 ) return "結果「嗤」地一聲劃出一道又長又深的傷口，濺得$N滿臉鮮血﹗\n";
        else return "結果只聽見$n一聲慘嚎，$w已在$p$l劃出一道深及見骨的可怕傷口﹗﹗\n";
        break;
    case "刺傷":
        if( damage < 5 ) return "結果只是輕輕地刺破$p的皮肉。\n";
        else if( damage < 10 ) return "結果在$p$l刺出一個創口。\n";
        else if( damage < 20 ) return "結果「噗」地一聲刺入了$n$l寸許\﹗\n";
        else if( damage < 40 ) return "結果「噗」地一聲刺進$n的$l，使$p不由自主地退了幾步﹗\n";
        else if( damage < 60 ) return "結果「噗嗤」地一聲，$w已在$p$l刺出一個血肉糢糊的血窟窿﹗\n";
        else return "結果只聽見$n一聲慘嚎，$w已在$p的$l對穿而出，鮮血濺得滿地﹗﹗\n";
        break;
    case "瘀傷":
        if( damage < 2 ) return "結果「啪」地一聲擊中，可是似乎傷害不大。\n";
        else if( damage < 5 ) return "結果在$p的$l造成一處瘀青。\n";
        else if( damage < 10 ) return "結果一擊命中，$n的$l登時腫了一塊老高﹗\n";
        else if( damage < 20 ) return "結果一擊命中，$n悶哼了一聲顯然吃了不小的虧﹗\n";
        else if( damage < 40 ) return "結果「砰」地一聲，$n退了兩步﹗\n";
        else if( damage < 60 ) return "結果這一下「砰」地一聲打得$n連退了好幾步，差一點摔倒﹗\n";
        return "結果重重地擊中，$n「哇」地一聲吐出一口鮮血﹗\n";
        break;
    default:
        if( damage < 2 ) str =  "結果只是勉強造成一處輕微";
        else if( damage < 5 ) str = "結果造成輕微的";
        else if( damage < 10 ) str = "結果造成一處";
        else if( damage < 20 ) str = "結果造成一處嚴重";
        else if( damage < 30 ) str = "結果造成頗為嚴重的";
        else if( damage < 40 ) str = "結果造成相當嚴重的";
        else if( damage < 60 ) str = "結果造成十分嚴重的";
        else if( damage < 80 ) str = "結果造成極其嚴重的";
        else str =  "結果造成非常可怕的嚴重";
        return str + type + "﹗\n";
    }
#endif
}

string eff_status_msg(int ratio)
{
    if( ratio==100 ) return HIG "( $N看起來並沒有受傷。 )\n" NOR;
    if( ratio > 95 ) return HIG "( $N似乎受了點輕傷，不過光從外表看不大出來。 )\n" NOR;
    if( ratio > 90 ) return HIG "( $N看起來可能受了點輕傷。 )\n" NOR;
    if( ratio > 80 ) return HIY "( $N受了幾處傷，不過似乎並不礙事。 )\n" NOR;
    if( ratio > 60 ) return HIY "( $N受傷不輕，看起來狀況並不太好。 )\n" NOR;
    if( ratio > 40 ) return HIY "( $N氣息粗重，動作開始散亂，看來所受的傷著實不輕。 )\n" NOR;
    if( ratio > 30 ) return HIY "( $N已經傷痕累累，正在勉力支撐著不倒下去。 )\n" NOR;
    if( ratio > 20 ) return HIR "( $N受了相當重的傷，只怕會有生命危險。 )\n" NOR;
    if( ratio > 10 ) return HIR "( $N傷重之下已經難以支撐，眼看就要倒在地上。 )\n" NOR;
    if( ratio > 5  ) return HIR "( $N受傷過重，已經奄奄一息，命在旦夕了。 )\n" NOR;
    return HIR "( $N受傷過重，已經有如風中殘燭，隨時都可能斷氣。 )\n" NOR;
}

string status_msg(int ratio)
{
    if( ratio==100 ) return HIG "( $N看起來對這種程度的攻擊一點也不在乎。 )\n" NOR;
    if( ratio > 95 ) return HIG "( $N似乎有些疲憊，但也許\是誘敵之計，你無法確定。 )\n" NOR;
    if( ratio > 90 ) return HIG "( $N看起來可能有些累了，出招開始慢了下來。 )\n" NOR;
    if( ratio > 80 ) return HIY "( $N動作似乎開始有點不太靈光，但是仍然有條不紊。 )\n" NOR;
    if( ratio > 60 ) return HIY "( $N氣喘噓噓，看起來狀況並不太好。 )\n" NOR;
    if( ratio > 40 ) return HIY "( $N似乎十分疲憊，看來體力的消耗相當嚴重。 )\n" NOR;
    if( ratio > 30 ) return HIY "( $N已經一副頭重腳輕的模樣，正在勉力支撐著不倒下去。 )\n" NOR;
    if( ratio > 20 ) return HIR "( $N看起來已經力不從心了。 )\n" NOR;
    if( ratio > 10 ) return HIR "( $N搖頭晃腦、歪歪斜斜地站都站不穩，眼看就要倒在地上。 )\n" NOR;
    return HIR "( $N已經陷入半昏迷狀態，隨時都可能摔倒暈去。 )\n" NOR;
}

varargs void report_status(object ob, int effective)
{
    if( !ob->query_stat_maximum("kee") ) return;
    if( effective ) 
        message_vision( eff_status_msg(
            (int)ob->query_stat_effective("kee") * 100 / (int)ob->query_stat_maximum("kee") ), ob);
    else
        message_vision( status_msg(
            (int)ob->query_stat("kee") * 100 / (int)ob->query_stat_maximum("kee") ) , ob);
}

// fight()
//
// This function serves as a lowest level routine for combat system and
// is usually called from a daemon of combat skill such as unarmed, sword
// etc.

varargs int
fight(object me, object victim, string skill, mapping action, object weapon)
{
    int ability, strength, damage, gin_cost, force_bonus;
    string msg, force_skill;

    // 若在非戰區, 且戰鬥雙方都沒被arrest, 停止戰鬥 -Dragoon
    // 拿掉userp(me) check, mob也不準在非戰區出手攻擊 -dragoon
    if( environment(me)->query("no_fight") ) {
	if( !victim->query("unprotect_mark") 
	&& !me->query("unprotect_mark") ) return 0;
    }

    // 若戰鬥到一半斷線, 停止雙方戰鬥 -Dragoon
    if( ( userp(me) && !interactive(me) ) 
    || ( userp(victim) && !interactive(victim) ) ) return 0;

    me->set_combat_message(action["action"]);
    me->set_temp("last_action", action);

    /* 力道
     */
    strength = me->query_strength("attack");

    /* 內力對戰鬥的影響:
     *
     * 力道增加: 技能 * 氣 * (功力百分比 + 招式係數)% * 4
     *           => 技能 100, 氣 1000, 十成功力, 無招式, 會增加 400Kg 力道
     * 需要氣: 所增加力道 / 500;
     * 消耗精: 隨機(1 到 1 + 所增加力道 / 30000)
     */
    gin_cost = 1;
    if( force_skill = me->skill_mapped("force") )
    {
        int ratio, kee_required, kee;

        if( !(ratio = me->query("force_ratio")) ) ratio = 75;

        force_bonus = me->query_skill("force") * me->query_stat("kee") 
                * (ratio + action["force"]) / 25;
        kee_required = force_bonus / 500;

        if( userp(me) && kee_required > 0 ) {
            kee = me->query_stat("kee");
            if( kee < kee_required )
                force_bonus = force_bonus * kee / kee_required;
        }

        /* 當使用內功，以極低的功力百分比戰鬥時，有機會 gin_cost = 0 */
        gin_cost = random(2 + force_bonus/30000);
    }

    /* 命中率: 攻擊能力 + 攻擊技能 + 招式係數 */
    ability = me->query_ability("attack")
        + me->query_skill(skill)
        + action["attack"]
        ;

    /* 若攻擊對象對攻擊者所使用的技能也熟悉，則給予命中率的負面效應。 */
    ability -= random( victim->query_skill(skill) );

    /* 若攻擊對象上一個招式有防禦效果，則給予命中率影響。 */
    ability -= victim->query_temp("last_action/defense");

    /* 攻擊者消耗少許精力。 */
    if( userp(me) ) {
        if( me->query_stat("gin") > gin_cost ) {
            me->consume_stat("gin", gin_cost);
            strength += force_bonus;
        } else {
            // 當攻擊者精少於要求的精力時, 除了降低攻擊能力, 另
            // 外仍需扣精 -Dragoon
            me->consume_stat("gin", gin_cost/2 + 1);
            ability /= 2;
            strength /= 2;
        }
    }
    else strength += force_bonus;

    if( ability < 1 ) ability = 1;

    /* 要求攻擊對象進行防禦。 */
    me->set_temp("defend_message", 0);
    if( !victim->is_busy()
    &&  victim->defend(ability, strength, weapon ? weapon : me) )
    {
        string defend_msg;

        /* 防禦成功，將防禦的訊息加入戰鬥訊息。 */
        defend_msg = me->query_temp("defend_message");
        me->add_combat_message( "，" );
        me->add_combat_message( stringp(defend_msg) ? defend_msg
                : "但是沒中" );
        damage = -1;
    }
    else
    {
        string absorb_msg;

        /* 防禦失敗，給予攻擊對象吸收力道的機會。 */
        me->set_temp("absorb_message", 0);
        strength -= (int)victim->absorb(ability, strength,
                weapon ? weapon : me);
        absorb_msg = me->query_temp("absorb_message");
        me->add_combat_message( "，" );
        me->add_combat_message( stringp(absorb_msg) ? absorb_msg
                : "但是被$n格開");

        /* 若力道未完全被吸收，則產生傷害。 */
        /* 產生傷害，則視攻擊方式給予少數精熟度 -Dragoon */
        if( strength > 0 ) {
            if( weapon ) {
                damage = weapon->inflict_damage(strength, victim);
                me->gain_score("weapon mastery", random(me->query_attr("int")/3));
            } else {
                damage = me->inflict_damage(strength, victim);
                me->gain_score("unarmed mastery", random(me->query_attr("int")/3));
            }
        }
        else damage = 0;
    }

    /* 將傷害程度的訊息加入戰鬥訊息。 */
    if( damage >= 0 ) {
#ifdef SHOW_DAMAGE
        me->add_combat_message("，" + damage_msg( damage, action["damage_type"] ));
#else
        int max, percent;

        max = victim->query_stat_effective("kee");
        if( max ) percent = damage * 100 / max;
        else percent = -1;      /* 非生物的傷害訊息 */
        me->add_combat_message("，" + damage_msg( percent, action["damage_type"]));
#endif
    }
    else me->add_combat_message("。\n");

#ifdef SHOW_COMBAT_STAT
    message_vision(sprintf(HIY "%s的攻擊：[技巧 %d, 力道 %.2fkg]\n" NOR,
        me->name(), ability, strength / 1000.0), me);
#endif

    /* 送出戰鬥訊息。 */
    msg = me->get_combat_message();
    if( stringp(msg) )
    {
        string *limbs = victim->query("limbs");

        if( !arrayp(limbs) || !sizeof(limbs) ) {
            msg = replace_string(msg, "$l", "身上");
        } else {
            msg = replace_string(msg, "$l", limbs[random(sizeof(limbs))]);
        }
        if( weapon ) msg = replace_string(msg, "$w", weapon->name());

        message_vision( msg, me, victim);
        if( damage > 0 ) report_status(victim);
    }

    /* 傳回非零值(傷害力)表示攻擊者可獲得經驗 */
    if( living(victim) ) {
        /* 對手是生物 */
        return damage;
    } else {
        /* 對手是非生物，攻擊者能力需小於 80 才有機會得到經驗 */
        // ability > 80 的限制有點過嚴, 同狀況下force exp卻無此限制
        // 略為放寬這裏的設限, 反正stake已經限制了頂多能練到那一級
        // -Dragoon 
        return random(500) > ability ? damage : 0;
    }
}

// auto_fight()
//
// This function is to start an automatically fight. Currently this is
// used in "aggressive", "vendetta", "hatred", "berserk" fight.

void auto_fight(object me, object obj, string type)
{
    // Don't let NPC autofight NPC.
    if( !userp(me) && !userp(obj) ) return;

    // This call_out gives victim a chance to slip trough the fierce guys.
    call_out( "start_" + type, 0, me, obj);
}

// add check net connection of player, testing work or not -Dragoon
void
start_hatred(object me, object obj)
{
    if( !me || !obj ) return;   // Are we still alive(not becoming a corpse)?

    if( !living(me)                             // Are we capable for a fight?
    ||  (userp(obj) && !interactive(obj))       // Is player net dead?
    ||  environment(me)!=environment(obj)       // Are we still in the same room?
    ||  environment(me)->query("no_fight")      // Are we in a peace room?
    )   return;

    // We found our hatred! Charge!
    message_vision( catch_hunt_msg[random(sizeof(catch_hunt_msg))], me, obj);
    me->kill_ob(obj);
    if( userp(obj) ) obj->fight_ob(me);
    else obj->kill_ob(me);
    me->attack(obj);
}

void
start_vendetta(object me, object obj)
{
    if( !me || !obj ) return;   // Are we still exist( not becoming a corpse )?

    if( me->is_fighting(obj)    // Are we busy fighting?
    ||  !living(me)             // Are we capable for a fight?
    ||  (userp(obj) && !interactive(obj))       // Is player net dead?
    ||  environment(me)!=environment(obj)       // Are we still in the same room?
    ||  environment(me)->query("no_fight")      // Are we in a peace room?
    )   return;

    // We found our vendetta opponent! Charge!
    message_vision( catch_hunt_msg[random(sizeof(catch_hunt_msg))], me, obj);
    me->kill_ob(obj);
    if( userp(obj) ) obj->fight_ob(me);
    else obj->kill_ob(me);
    me->attack(obj);
}

void start_aggressive(object me, object obj)
{
    int def;

    if( !me || !obj ) return;   // Are we still exist( not becoming a corpse )?

    if( me->is_fighting()       // Are we busy fighting?
    ||  !living(me)             // Are we capable for a fight?
    ||  (userp(obj) && !interactive(obj))       // Is player net dead?
    ||  environment(me)!=environment(obj)       // Are we still in the same room?
    ||  environment(me)->query("no_fight")      // Are we in a peace room?
    ||  obj->query("life_form") == "ghost"	// Only kill living -Dragoon
    ||  me->query_stat("kee") < obj->query_stat("kee")
    )   return;

    // let cor, cps, and command: agg work -dragoon
    def = (int)me->query_attr("cps");
    if( me->query_temp("ashura/aggressive") < 1 )
	def += (int)me->query_attr("cps");
    if( random(me->query_attr("cor")) < def ) return;

    /* 讓阿修羅族遇到其他玩家時，只打攻勢比自己強的 */
//  好像設反了?? 先取消 -dragoon
//    if( userp(me) && userp(obj) 
//    && obj->query_ability("intimidate") > me->query_ability("intimidate") )
//        return;

    // We got a nice victim! Kill him/her/it!!!
    message_vision( catch_hunt_msg[random(sizeof(catch_hunt_msg))], me, obj);
    me->kill_ob(obj);
    if( userp(obj) ) obj->fight_ob(me);
    else obj->kill_ob(me);
    me->attack(obj);
}

// This function is to announce the special events of the combat.
// This should be moved to another daemon in the future.
void announce(object ob, string event)
{
    switch(event)
    {
    case "dead":
        message_vision( dead_msg[random(sizeof(dead_msg))], ob );
        break;
    case "unconcious":
        message_vision("\n$N腳下一個不穩﹐跌在地上一動也不動了。\n\n", ob);
        break;
    case "revive":
        message_vision("\n$N慢慢睜開眼睛﹐清醒了過來。\n\n", ob);
        break;
    }
}

