// 給想寫 skill 的人當範例用的
// fonxansword.c

#include <ansi.h>

inherit SKILL;

mapping *actions = ({
	([	"action":		"$N使一招「峰迴路轉」﹐手中$w如一條銀蛇般刺向$n的$l",
		"dodge":		-20,
		"damae":		30,
		"damage_type":	"刺傷"
	]),
	([	"action":		"$N使出封山劍法中的「空山鳥語」﹐劍光霍霍斬向$n的$l",
		"dodge":		-20,
		"damae":		30,
		"damage_type":	"割傷"
	]),
	([	"action":		"$N一招「御風而行」﹐身形陡然滑出數尺﹐手中$w斬向$n的$l",
		"dodge":		-30,
		"damae":		20,
		"damage_type":	"割傷"
	]),
	([	"action":		"$N手中$w中宮直進﹐一式「旭日東昇」對準$n的$l刺出一劍",
		"dodge":		-40,
		"damage_type":	"刺傷"
	]),
	([	"action":		"$N縱身一躍﹐手中$w一招「金光瀉地」對準$n的$l斜斜刺出一劍",
		"dodge":		-40,
		"damage_type":	"刺傷"
	]),
	([	"action":		"$N的$w憑空一指﹐一招「童子引路」刺向$n的$l",
		"dodge":		20,
		"damage":		40,
		"damage_type":	"刺傷"
	]),
	([	"action":		"$N手中$w向外一分﹐使一招「柳暗花明」反手對準$n$l一劍刺去",
		"dodge":		-20,
		"damage":		20,
		"damage_type":	"刺傷"
	]),
	([	"action":		"$N橫劍上前﹐身形一轉手中$w使一招「空谷秋虹」畫出一道光弧斬向$n的$l",
		"dodge":		-30,
		"damage":		10,
		"damage_type":	"割傷"
	]),
});

string *interattack = ({
        "$N緊握著手中的$n﹐伺機而動。\n",

});

void create()
{
        seteuid(getuid());
		DAEMON_D->register_skill_daemon("fonxansword");
		setup();
}

void attack_using(object me, object opponent, object weapon)
{
		int damage;

        if( !opponent ) {
                if( me->query_temp("last_attacked_target"))
                message_vision(CYN + interattack[random(sizeof(interattack))] +
NOR,me, weapon);
                return;
        }

	damage = COMBAT_D->fight(me, opponent, "fonxansword",
		actions[random(sizeof(actions))], weapon);
}

int valid_enable(string usage)
{
        return usage=="sword" || usage=="parry";
}
