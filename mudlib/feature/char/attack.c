// vim: syntax=lpc

#include <ansi.h>
#include <dbase.h>
#include <name.h>
#include <origin.h>
#include <skill.h>

#define MAX_OPPONENT    4

private object charge_target = 0;
private object guarding = 0, *guarded = ({});
private object *enemy = ({});
private mapping killer = ([]);
private int attack_patience;

private object current_target;

void fight_ob(object ob);
void kill_ob(object ob);

void add_guard(object who);
void remove_guard(object who);
object* wake_guard(object enemy);
void activate_guard(object enemy);
void guard_ob(object target);

object *query_enemy() { return enemy; }
mapping query_killer() { return killer; }
object query_charging() { return charge_target; }
object query_guarding() { return guarding; }
object query_opponent() { return current_target; }

varargs int is_fighting(object ob)
{
    return ob ? (member_array(ob, enemy) >= 0) : sizeof(enemy);
}

varargs int is_killing(mixed id)
{
    if( !killer ) return 0;
    if( !id ) return sizeof(killer);

    if( objectp(id) && userp(id) ) id = id->query("id");

    if( stringp(id) || objectp(id) )
	return !undefinedp(killer[id]);

    return 0;
}

// add_guard()
//
// Let someone guard us when another one try to kill us.

void add_guard(object ob)
{
    if( member_array(ob, guarded) >= 0 ) return;

    tell_object(this_object(), HIY + ob->name() + "開始保護你。\n" NOR);
    guarded += ({ ob });
}

// remove_guard()
//
// Remove someone who is not guarding us any more.

void remove_guard(object ob)
{
    if( member_array(ob, guarded) < 0 ) return;

    guarded -= ({ ob, 0 });
    tell_object(this_object(), HIY + ob->name() + "不再保護你了。\n"+ 
	NOR);
}

// wake_guard()
//
// This function returns awaken guards that is not yet fighting specific
// enemy. Visibility of enemy and presence of guard is also checked.

object *wake_guard(object target)
{
    return filter(guarded, (:
	objectp($1)
	&& living($1)
	&& environment($1)==environment()
	&& (!$1->is_fighting($(target)))
	&& $(target)->visible($1) :),
    );
}

// activate_guard()
//
// Activate guarding action upon specific target. Default action is to fight
// target.

void activate_guard(object target)
{
    fight_ob(target);
}

// guard_ob()
//
// Set this character to guard someone. guard_ob(0) to clear guarding status.

void
guard_ob(object ob)
{
    if( objectp(guarding) && (guarding != ob) ) {
	tell_object(guarding, HIY + name() + "不再保護你了。\n" NOR);
	guarding->remove_guard(this_object());
    }

    guarding = ob;
    if( objectp(ob) ) ob->add_guard(this_object());
}

// fight_ob()
//
// Start fighting someone.

void fight_ob(object ob)
{
    object *guard;

    // Check the target if it is valid and at the same room with us.
    if( !objectp(ob)
    ||	ob==this_object()
    ||	environment(ob) != environment())
	return;

    // If we are to fight the one we are guarding, then stop guarding before
    // we fight.
    if( ob==guarding ) guard_ob(0);

    // Start heart beating.
    set_heart_beat(1);

    // Insert the target to the first attack target.
    if( member_array(ob, enemy)==-1 )
	enemy += ({ ob });

    // Check if any characters is guarding the target, and let them join
    // the fight.
    guard = ob->wake_guard(this_object());
    if( sizeof(guard) ) {
	message("system", HIY + ob->name() + "正遭受攻擊！你上前助戰！\n" NOR, guard);
	guard->activate_guard(this_object());
	enemy += guard;
    }
}

// kill_ob()
//
// This function starts killing between this_object() and ob

void kill_ob(object ob)
{
    if( userp(ob) ) {
	if( living(ob) && undefinedp(killer[ob->query("id")]) )
	    tell_object(ob, HIR "看起來" + this_object()->name() + "想殺死你！\n" NOR);
	killer[ob->query("id")] = time();
    }
    else
	killer[ob] = time();

    // Remove dead npc killers, but player killer remains.
    killer = filter(killer, (: stringp($1) || objectp($1) :));

    // Start the fight.
    fight_ob(ob);
}

// charge_ob()
//
// This function promotes ob to the first target to attack.
void
charge_ob(object ob)
{
    if( ! ob ) { charge_target = 0; return; }

    if( member_array(ob, enemy) < 0 ) enemy += ({ ob });
    tell_object(this_object(),
	HIY "你開始將" + ob->name() + "當成首要攻擊目標！\n" NOR);
    charge_target = ob;
}

// remove_enemy()
//
// Stop fighting specific object. (Might fight again on next encounter)
int remove_enemy(object ob)
{
    if( is_killing(ob) ) return 0;
    enemy -= ({ ob });
    return 1;
}

// remove_killer()
//
// Stop fighting specific object no matter if he is a killer or not.

void remove_killer(object ob)
{
    map_delete(killer, userp(ob) ? ob->query("id") : ob);
    remove_enemy(ob);
}

// remove_charge()
//
// Stop charging specific object;

int remove_charge()
{
    charge_target = 0;
}

// remove_all_enemy()
//
// Stop all fighting, but killer remains.

void remove_all_enemy()
{
    enemy = filter(enemy,
	(: objectp($1) && $1->remove_enemy($(this_object()))==0 :));
}

// remove_all_killer()
//
// Remove all enemies at once, killer or not.

void remove_all_killer()
{
    object ob;

    // We MUST stop killing anyone before asking them to forget us.
    killer = ([]);
        
    // Call all enemies seeing we die stop killing us. They have done enough.
    enemy->remove_killer(this_object());
    enemy = ({});
}

/* clean_up_enemy()
 *
 * 這個函數用來將已經沒有戰鬥能力的敵人自戰鬥對象中剔除，傳回值如果大於 0
 * 表示尚有敵人。
 */
int
clean_up_enemy()
{
    enemy = filter(enemy, (:
	objectp($1)
	&& (environment($1)==environment())
	&& (living($1) || is_killing($1))
    :) );

    return sizeof(enemy);
}

/* select_opponent()
 *
 * 這個函數用來為人物選擇一個攻擊對象。
 */

private object
select_opponent()
{
    object opp;
    int which, intimidate, wittiness, chance;

    if( !arrayp(enemy) || !sizeof(enemy) ) return 0;

    /* 攻擊傾向 = 攻勢等級 + 等待係數 */
    intimidate = this_object()->query_ability("intimidate") + attack_patience;

    /* 如果有首要攻擊目標，優先考慮這個目標 */
    if( charge_target && member_array(charge_target, enemy) >= 0 ) {
	intimidate += intimidate / 5;	// charge 的正面效果
	opp = charge_target;
    } else
	opp = enemy[random(sizeof(enemy))];

    /* 守勢等級 */
    if( living(opp) ) {
	wittiness = opp->query_ability("wittiness");
	if( opp->query_charging() )
	    wittiness -= wittiness / 3;	// charge 的負面效果
    }
    else wittiness = 0;

    if( ! wittiness ) return opp;
    chance = 50 + (intimidate - wittiness) * 2;

    /* 將發動攻擊的機會限定在 20% 到 80% 之間 */
    if( chance < 20 ) chance = 20;
    else if( chance > 80 ) chance = 80;

    /* 發動攻擊！清除等待係數 */
    if( random(100) < chance ) {
	attack_patience = 0;
	return opp;
    }

    /* 攻勢不足，累加等待係數 */
    attack_patience += intimidate / 10;
    return 0;
}

/* attack()
 *
 * 這個函是用來控制人物每回合的戰鬥：使用武器技能或徒手搏鬥技能攻擊。
 */

varargs int
attack(object opponent)
{
    string skill;
    mapping wielded_weapon;

    if( !living(this_object()) ) return 0;

    /* 若沒有指定目標，則選定一個目標。一般而言指定目標的呼叫方式是給
     * 特殊技能用的，沒有指定目標的方式是給 heart_beat 用的。
     */
    if( !objectp(opponent) )
        opponent = select_opponent();

    if( opponent ) current_target = opponent;

    wielded_weapon = query_temp("weapon");
    if( mapp(wielded_weapon) && sizeof(wielded_weapon) )
    {
	object weapon;
	string art;

	// Attack with each of our weapons.
	foreach(skill, weapon in wielded_weapon) {
	    art = skill_mapped(skill);
	    if( ! art ) continue;
	    weapon->attack_with( this_object(), opponent, art );
	}
    } else {
	// Or, attack with unarmed.
	skill = skill_mapped("unarmed");
	if( skill )
	    SKILL_D(skill)->attack_using(this_object(), opponent, skill);
    }

    return 1;
}

// init()

void init()
{
// 這個check似乎與/adm/daemon/combatd.c裏重複, 造成一些異常
//   取消測試 -Dragoon
//    /* 若這個物件被移動到非戰區，則不引發戰鬥。 */
//  if(	environment() && environment()->query("no_fight") ) return;
//
    /* 若 this_player() 看不見這個人物，則不引發戰鬥。 */
    if( ! this_object()->visible(this_player()) ) return;

    /* 若雙方正互相仇視中，開始戰鬥。 */
    if( this_player()->is_killing(query("id")) ) {
	COMBAT_D->auto_fight(this_player(), this_object(), "hatred");
	return;
    }

    /* 若雙方有家族仇恨，則開始戰鬥。 */
    if( this_player()->query("vendetta/" + query("vendetta_mark")) ) {
	COMBAT_D->auto_fight(this_player(), this_object(), "vendetta");
	return;
    }

    switch( query("attitude") )
    {
    case "aggressive":
	/* 主動攻擊的生物 */
	COMBAT_D->auto_fight(this_object(), this_player(), "aggressive");
	break;
    case "peaceful":
    default:
	break;
    }
}
