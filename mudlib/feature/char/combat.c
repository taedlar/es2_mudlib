// vim: syntax=lpc
#pragma save_binary

#include <attack.h>
#include <dbase.h>
#include <score.h>
#include <skill.h>
#include <statistic.h>
#include <combat.h>

private string combat_message = "";

void set_combat_message(string s) { combat_message = s; }
void add_combat_message(string s) { combat_message += s; }
string get_combat_message() { return combat_message; }

varargs int query_attr(string,int);

// query_strength()
//
// This function returns the strength an character can bestow on specific
// application in the UNIT OF GRAM. Currently the default definition 
// implements a convert based on human attributes. Human strength ranged
// from 13 to 18, yielding the strength range from 16.9 kg to 32.4 kg.

int
query_strength (string application)
{
    if( !living(this_object()) ) return 0;
    switch(application) {
    case "attack":	// Strength to perform an attack.
	return query_attr("str") * query_attr("cor") * 150
		+ query_stat("kee") * 30;
    case "defense":	// Strength to defense against attack.
	return query_attr("str") * query_attr("cps") * 150
		+ query_stat("kee") * 30;
    case "magic":	// Strength of magic power
	return query_attr("spi") * query_attr("spi") * 150
		+ query_stat("sen") * 30;
    case "spell":	// Strength of spell power
	return query_attr("wis") * query_attr("wis") * 150
		+ query_stat("sen") * 30;
    case "carriage":	// Strength to carry items.
	return query_attr("str") * query_attr("str") * 200;
    default:
	return 0;
    }
}

// query_ability()
//
// This function returns the ability measurement of the character's specific
// ability. Currently the value of ability had been adjusted to fit the skill
// system that with 100 as regular-maximum. Under this asumption, a human's
// natural attack ability ranges from 21 to 70 (an ultimate wimpy in dying
// status versus an top ace in perfect condition) in the UNIT OF SKILL LEVEL.

int
query_ability (string application)
{
    if( !living(this_object()) ) return 0;

    switch(application) {

    /* attack :
     *
     * 實體攻擊的基本技巧值。
     */
    case "attack":
	return query_attr("dex") * query_attr("cor") / 10
		+ (query_stat("gin") >> 5)
		+ query_temp("apply/attack");

    /* defense :
     *
     * 對實體攻擊的基本防禦值。
     */
    case "defense":
	return query_attr("dex") * query_attr("cps") / 10
		+ (query_stat("gin") >> 5)
		+ query_temp("apply/defense");

    /* intimidate:
     *
     * 攻勢等級。
     */
    case "intimidate":
	return query_attr("cor") * query_attr("str") / 10
		+ (query_stat("kee") >> 5)
		+ query_temp("apply/intimidate");

    /* wittiness:
     *
     * 守勢等級。
     */
    case "wittiness":
	return query_attr("cps") * query_attr("wis") / 10
		+ (query_stat("kee") >> 4)
		+ query_temp("apply/wittiness");

    /* magic:
     *
     * 魔力。
     */
    case "magic":
	return query_attr("spi") * query_attr("int") / 10
		+ (query_stat("sen") >> 5)
		+ query_temp("apply/magic");

    /* spell:
     *
     * 法力。
     */
    case "spell":
	return query_attr("spi") * query_attr("wis") / 10
		+ (query_stat("sen") >> 5)
		+ query_temp("apply/spell");

    case "move":
	return query_attr("str") * query_attr("dex") / 10
		+ (query_stat("gin") >> 5)
		+ query_temp("apply/move");

    /* awarness:
     *
     * 對隱藏事物的感應力。
     */
    case "awarness":
	return query_attr("cps") * query_attr("spi") / 10
		+ (query_stat("gin") >> 4)
		+ query_temp("apply/awarness");

    case "cauterization":
	return query_attr("cor") * query_attr("wis") / 10
		+ query_stat("gin") / 40;
    default:
	return query_skill(application);
    }
}

/*
 * int defend(int ability, int strength, object from)
 *
 * 這個函數定義一個人物的「防禦」能力。
 *
 * 當人物受到某種攻擊的時候，應該呼叫這個函數讓人物有防禦的機會。
 * ability 及 strength 分別描述這個攻擊的技巧性及強度，from 表示這個攻擊
 * 所使用的物件，可能是另一個人物、武器、魔法技能等。
 *
 * defend 若傳回一個非 0 的數值，表示這個攻擊被成功地防禦住，這次的攻擊
 * 應該無效。若傳回 0 表示這個人物無法防禦，或防禦失敗。
 */

int
defend (int ability, int strength, object from)
{
    string sk;
    int counter_ability, chance;

    if( !living(this_object()) ) return 0;

    switch(from->query("phase")) {
    case PHASE_MENTAL:
    case PHASE_ILLUSION:
    case PHASE_ELEMENTAL:
	counter_ability = query_ability("defense");
	break;
    case PHASE_PHYSICAL:
	counter_ability = query_ability("defense");
	if( (sk=skill_mapped("dodge")) )
	    counter_ability += SKILL_D(sk)->dodge_using(this_object(),
			ability, strength, from);
	break;
    default:	/* unknown phase */
	return 0;
    }

    chance = 50 + (ability - counter_ability) * 2;
    if( chance < 5 ) chance = 5;
    else if( chance > 95 ) chance = 95;
    return (random(100) > chance);
}

/*
 * int absorb(int ability, int strength, object from)
 *
 * 這個函數定義一個人物「承受」攻擊的能力。
 * ability 及 strength 表示這個攻擊的技巧性及強度，from 表示這個攻擊所使用的
 * 物件。
 *
 * absorb 應該傳回一個整數值，表示有多少攻擊的強度被「吸收」了。
 */

int
absorb (int ability, int strength, object from)
{
    string sk;

    /* 鬼魂永遠吸收掉任何攻擊的所有的力道 */
    if( query("life_form")=="ghost" ) {
	if( from->is_character() )
	    from->add_combat_message("但是從$n身上穿透過去，");
	else if( environment(from) && environment(from)->is_character() )
	    environment(from)->add_combat_message("但是$w從$n身上穿透過去，");
	return strength;
    }

    /* 鬼魂的力道也永遠被完全吸收 */
    if( from->is_character() && from->query("life_form")=="ghost" ) {
	from->add_combat_message("但是陷進$n的身體，");
	return strength;
    }

    if( !living(this_object()) ) return 0;

    if( (sk=skill_mapped("parry")) )
	return SKILL_D(sk)->parry_using(
	    this_object(), ability, strength, from);

    return 0;
}

// inflict_damage()
//
// This is the default function to inflict damage when unarmed. (If armed
// with weapon(s), the inflict_damage() in weapon object is called instead)
// For more information about the task of this function, see F_EQUIP for
// the comments of inflict_damage().

int
inflict_damage (int strength, object victim)
{
    int damage;
    string force_sk;

    // If we are not living, make no damage.
    if( !living(this_object()) ) return 0;

    // Convert strength to damage. If we are using force, use force formula.
    // Otherwise use default.
    force_sk = skill_mapped("force");

    // If using force, call force skill to inflict damage
    if( force_sk ) {
	damage = SKILL_D(force_sk)->inflict_damage(strength, victim);

	// Call default force skill in case the special force doesn't define
	// inflict_damage()
	if( ! damage && force_sk != "force" )
	    damage = SKILL_D("force")->inflict_damage(strength, victim);

	return damage;
    }

    damage = 1 + strength/10000 + random(strength/10000);
    damage += query_temp("apply/damage");

    // Call victim to resist this.
    damage -= victim->resist_damage(damage, this_object());

    return damage > 0 ? victim->receive_damage(damage, this_object(), this_object()) : 0;
}

// resist_damage()
//
// This function returns the non-initiative defense against all physical
// damage to this object. 

int
resist_damage (int damage, object from_ob)
{
    return query_temp("apply/armor");
}

// receive_damage()
//
// This function does actual damage on this object and return the actual
// number of damage received. Note that this function is for 'game system
// abstraction' reason and ANY DEFENSE should be defined in resist_damage
// instead. One example application  of this function is to convert hp/mp
// statistics system into gin/kee/sen statistic system.

varargs int
receive_damage (int damage, object from, object attacker)
{
    int max, kill_mode = 0;
    mapping flag;
    string from_type;

    if( damage < 1 ) return 0;
    if( !living(this_object()) ) damage *= 10;

    kill_mode = objectp(attacker) ? attacker->is_killing(this_object()) : 0;
    from_type = objectp(from) ? from->query("material") : "unknown";

    // Damage this character according the life form it is in.
    // Currently the default form is living, which is, damage kee.
    switch( query("life_form") ) {
    case "ghost":
	consume_stat("gin", damage, attacker);

	if( kill_mode )
	{
	    if( living(this_object()) ) {
		/* 鬼魂以精保護自己 */
		max = query_stat_effective("gin");
		if( random(max/3) < damage ) {
		    damage_stat("gin", damage, attacker);
		}

		/* 如果傷害力和精越接近，則傷害形體的機會越高 */
		if( random(query_stat("gin")) < damage )
		    consume_stat("HP", random(damage)+1, attacker);
	    } else {
		/* 人物呈昏迷狀態時，直接由形體承受所有傷害 */
		consume_stat("HP", damage, attacker);
	    }
	}
	break;
    case "living":
    default:
	/* 消耗氣力 */
	consume_stat("kee", damage, attacker);

	/* 只有 kill 模式才會受傷 */
	if( kill_mode )
	{
	    if( living(this_object()) ) {
		/* 氣的作用：保護人物不受傷害，所以當氣的狀態值越高時
		 * 受傷的機會越低，反之越高。
		 */
		max = query_stat_effective("kee");
		if( random(max/3) < damage ) {
		    damage_stat("kee", damage, attacker);
		    gain_score("survive", damage);
		}

		/* 如果傷害力和氣越接近，則傷害形體的機會越高 */
		if( random(query_stat("kee")) < damage )
		    consume_stat("HP", random(damage)+1, attacker);
	    } else {
		/* 人物呈昏迷狀態時，直接由形體承受所有傷害 */
		consume_stat("HP", damage, attacker);
	    }
	}
	break;
    }

    // Quick recover of character, occurs on the following condition:
    //
    // 1. Attacker is not killing this character.
    // 2. This character is not killing ANYONE.
    //    (And nobody is killing this character, however, this costs too much
    //    code to check, so let give it a chance in next turn.)
    // 3. This character is exhausting in next turn.
    //
    if(	!kill_mode 
    &&	!is_killing()
    &&	mapp(flag = query_exhausted())
    &&	(flag["gin"] || flag["kee"] || flag["sen"] || flag["HP"]) )
    {
        string stat;
        object ob;

	// fix all exhausted stats to zero.
        foreach(stat, ob in flag)
            set_stat_current(stat, 1);

	clear_statistic_flags();

        // ask all enemy to stop, they've done enough.
        remove_all_enemy();

        call_out("lose_fight", 0, attacker);
        return damage;
    }

    return damage;
}

