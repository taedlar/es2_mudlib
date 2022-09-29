/*  equip.c

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
#include <combat.h>
#include <dbase.h>
#include <name.h>
#include <move.h>
#include <type.h>

private void
apply(mapping a)
{
    string prop;
    mixed app;
    mapping applied;

    applied = environment()->query_temp("apply");
    if( !mapp(applied) ) applied = ([]);

    foreach(prop, app in a) {
        switch(typeof(app)) {
        case INT:
        case FLOAT:
            applied[prop] += app;
            break;
        case ARRAY:
        case MAPPING:
        case OBJECT:
        case STRING:
            if( undefinedp(applied[prop]) ) applied[prop] = ({ app });
            else if( arrayp(applied[prop]) ) applied[prop] += ({ app });
            else error("Incompatible equipment apply.\n");
            break;
        default:
            error("Illegal type of equipment apply.\n");
            break;
        }
    }
}

private void
unapply(mapping a)
{
    string prop;
    mixed app;
    mapping applied;

    applied = environment()->query_temp("apply");
    if( !mapp(applied) ) applied = ([]);

    foreach(prop, app in a) {
        switch(typeof(app)) {
        case INT:
        case FLOAT:
            applied[prop] -= app;
            break;
        case ARRAY:
        case MAPPING:
        case OBJECT:
        case STRING:
            if( !arrayp(applied[prop]) )
                error("Unconsistent type of apply encountered when attempt to unapply.\n");
            applied[prop] -= ({ app });
            break;
        default:
            error("Illegal type of equipment apply.\n");
            break;
        }
    }
}

int
unequip()
{
    object owner;
    mapping prop, applied_prop;
    string *apply, equipped, type;

    if( !(owner = environment())->is_character() ) return 0;

    if( !stringp(equipped = query("equipped")) )
        return notify_fail("你目前並沒有裝備這樣東西。\n");

    // This is a speed-generality concern hack.
    // Assume we wield this as 'sword', so we mark the wield in ourself
    // with set("equipped", "weapon/sword"), and call player->set_temp(
    // "weapon/sword", this_object(). Here the "weapon/sword" is the
    // equipped path in the database.
    owner->delete_temp(equipped);
    delete("equipped");

    if( mapp(prop = query("apply_" + equipped)) )
        unapply(prop);

    return 1;
}

varargs int
wear(string on_part)
{
    object owner, old;
    mapping armor_prop;
    string *apply;
    mixed valid_types;

    // Only character object can wear armors.
    if( !(owner = environment())->is_character() ) return 0;

    // If already equipped in another usage, unequip first.
    if( query("equipped") && !unequip() ) return 0;

    // Check if this can be worn on some owner's body part.
    valid_types = query("wear_as");
    if( on_part ) {
        if( stringp(valid_types) ) {
            if(on_part != valid_types)
                return notify_fail("這樣東西不能被當作這類護具使用。\n");
        } else if( arrayp(valid_types) ) {
            if(member_array(on_part, valid_types)==-1)
                return notify_fail("這樣東西不能被當作這類護具使用。\n");
        } else 
            return notify_fail("這樣東西不能當作護具。\n");
    } else {
        if( stringp(valid_types) ) on_part = valid_types;
        else if( arrayp(valid_types) ) on_part = valid_types[0];
        else return notify_fail("這樣東西不能當作護具。\n");
    }

    // Ask race/class daemon to check if we can wield such weapon.
    notify_fail("你的種族或職業不允許\你使用這種護具。\n");
    if( !(RACE_D(owner->query_race())->valid_wear(owner, this_object(), on_part))
    ||    !(CLASS_D(owner->query_class())->valid_wear(owner, this_object()), on_part))
        return 0;

    // if any object occupying on the same part, unequip it.
    if( old = owner->query_temp("armor/" + on_part) ) 
        if( !old->unequip() ) return 0;

    // Apply armor properties if any.
    if( mapp(armor_prop = query("apply_armor/" + on_part)) )
        apply(armor_prop);

    // okay, wear it.
    owner->set_temp("armor/" + on_part, this_object());
    set("equipped", "armor/" + on_part);

    return 1;
}

varargs int
wield(string as_skill)
{
    object owner, old;
    mapping weapon_prop;
    string *apply, type, base;
    mixed valid_types;

    // Only character object can wear armors.
    if( !objectp(owner = environment())
    ||	!owner->is_character() ) return 0;

    // If already equipped in another usage, unequip first.
    if( query("equipped") && !unequip() ) return 0;

    // Check if this can be wielded as weapon with some skill
    valid_types = query("wield_as");
    if( as_skill ) {
        if( stringp(valid_types) ) {
            if(as_skill != valid_types) 
                return notify_fail("這樣東西不能被當作這類武器使用。\n");
        } else if( arrayp(valid_types) ) {
            if(member_array(as_skill, valid_types)==-1) 
                return notify_fail("這樣東西不能被當作這類武器使用。\n");
        } else
            return notify_fail("這樣東西不能當作武器。\n");
    } else {
        if( stringp(valid_types) ) as_skill = valid_types;
        else if( arrayp(valid_types) ) as_skill = valid_types[0];
        else return notify_fail("這樣東西不能當作武器。\n");
    }

    // if any object occupying on the same part, unequip it.
    if( old = owner->query_temp("weapon/" + as_skill) ) 
        if( !old->unequip() ) return notify_fail("你無法取下正在使用中的同種類武器");

    // Ask race/class daemon to check if we can wield such weapon.
    notify_fail("你的種族或職業不允許\你使用這種武器。\n");
    if( !(RACE_D(owner->query_race())->valid_wield(owner, this_object(), as_skill))
    ||    !(CLASS_D(owner->query_class())->valid_wield(owner, this_object()), as_skill))
        return 0;

    // Apply the weapon properties if any.
    if( mapp(weapon_prop = query("apply_weapon/" + as_skill)) )
        apply(weapon_prop);

    // Set information about wielded.
    owner->set_temp("weapon/" + as_skill, this_object());
    set("equipped", "weapon/" + as_skill);

    return 1;
}

void
attack_with(object me, object opponent, string skill_type)
{
    SKILL_D(skill_type)->attack_using(me, opponent, this_object());
}

/*  inflict_damage()
 *
 *  傷害產生函數。
 *
 *  當一個武器物件擊中敵人時，會呼叫這個函數以對敵人造成傷害。傷害產生函
 *  數會帶有一個 strength 參數，表示這個傷害是由多少的力量造成的。
 */

int
inflict_damage(int strength, object victim)
{
    class damage_parameter dam;
    object owner;
    string skill;
    int damage, weight, max_bonus, bonus = 0;

    /* 檢查持有者 */
    owner = environment();
    if( ! owner ) return 0;

    /* 檢查所使用的技能 */
    if( stringp(skill = query("equipped")) ) {
	if( skill[0..6] != "weapon/" ) return 0;
	skill = skill[7..];
    }
    else return 0;

    if( userp(owner) ) {
	/* 檢查力道 */
	if( weight() > strength ) return 0;
    }

    /* 取得殺傷力參數 */
    if( !classp(dam = query("damage/" + skill)) ) return 0;

    max_bonus = dam->multipler * dam->range * dam->bonus / 100;

    /* 計算力量與重量的加值 */
    bonus = random(strength / 20000) + weight / 1000;
    if( bonus > max_bonus ) bonus = max_bonus;

    /* 重擊術的效用 */
    if( bonus < max_bonus
    &&	owner->query_learn("powerblow")
    &&	random(max_bonus - owner->query_skill("powerblow")) < bonus )
    {
	bonus = max_bonus;
	damage = dam->multipler * dam->range;
	owner->improve_skill("powerblow", 1 + bonus/5);
    }
    else damage = dam->multipler * (1+random(dam->range));

    damage += dam->roll;
    damage += bonus;
    damage += environment()->query_temp("apply/damage");

    damage -= random(victim->resist_damage(damage, this_object()));

    if( damage > 0 ) 
	return (int)victim->receive_damage(damage, this_object(), owner);

    return 0;
}

void
init_damage(int x, int y, int z, int r, mixed *slist ...)
{
    string skill;
    class damage_parameter dp;
    
    dp = new(class damage_parameter);
    dp->multipler = x;
    dp->range = y;
    dp->bonus = z;
    dp->roll = r;

    foreach(skill in slist)
	set("damage/" + skill, dp);

    set("phase", PHASE_PHYSICAL);
}
