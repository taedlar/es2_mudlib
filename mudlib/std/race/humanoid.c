/*  humanoid.c - standard race daemon of humanoid type

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#define BASE_WEIGHT 40000

#include <ansi.h>
#include <statistic.h>
#include <combat.h>

inherit F_DBASE;

mapping *combat_action = ({
    ([	"action":	"$N揮拳攻擊$n的$l",
	"damage_type":	"瘀傷",
    ]),
    ([	"action":	"$N往$n的$l一抓",
	"damage_type":	"抓傷",
    ]),
    ([	"action":	"$N往$n的$l狠狠地踢了一腳",
	"damage_type":	"瘀傷",
    ]),
    ([	"action":	"$N提起拳頭往$n的$l捶去",
	"damage_type":	"瘀傷",
    ]),
    ([	"action":	"$N對準$n的$l用力揮出一拳",
	"damage_type":	"瘀傷",
    ]),
});

string* wear_type = ({
    "armor", "cloth", "feet_eq", "finger_eq", "hand_eq", "head_eq",
    "leg_eq", "neck_eq", "waist_eq"
});

// create()
//
// To save memory, we usually set the dafault object of a character
// to his race daemon. The properties we set here can be used as
// common default properties for characters of this race.

void create()
{
    seteuid(getuid());
    set("karma", 5);                // karma cost to select human when reincarnate
    set("life_form", "living");     // default life form.
    set("unit", "位");
    set("gender", "male");  // default gender

    set("default_body", USER_OB);   // default body object
    set("civilized", 1);
    set("humanoid", 1);
    set("commoner_score_base", 100);        // score base for gaining level
    set("attitude", "peaceful");            // default attitude
    set("limbs", ({
	"頭部", "頸部", "胸口", "後心", "左肩", "右肩", "左臂",
	"右臂", "左手", "右手", "腰間", "小腹", "左腿", "右腿",
	"左腳", "右腳"
    }) );
    set("phase", PHASE_PHYSICAL);
    set("default_actions", (: call_other, __FILE__, "query_action" :) );
}

// setup()
//
// setup() is called when a character is ready to enter the world. Note
// that for a player, setup() is called EVERYTIME he/she logins while
// initialize() is only called on character creation. For NPC, setup()
// is called everytime loaded.

void
setup(object ob)
{
    int food_max, water_max;

    seteuid(getuid());
    ob->set_default_object(__FILE__);
    ob->init_attribute();
    ob->init_statistic();

    food_max = ob->query_attr("con") *10 + ob->query_attr("str") * 5;
    water_max = ob->query_attr("cps") * 10 + ob->query_attr("dex") * 5;
    ob->set_stat_maximum("food", food_max);
    ob->set_stat_effective("food", food_max);
    ob->set_stat_maximum("water", water_max);
    ob->set_stat_effective("water", water_max);
}

// initialize()
//
// To initialize race-specific stuff of a character. 

void initialize(object ob)
{
    ob->set_default_object(__FILE__);
}

// This is the dafault_action implementation. You don't need call it
// "query_action" always. We usually set the property "default_action"
// to this function, and let characters access this via default object
// query. See create() for more information. If you are to define your
// own default_action for a character, just set the default_action
// property of the character to something else to override default object
// querying.

mapping query_action()
{
    return combat_action[random(sizeof(combat_action))];
}

string query_appearance(object ob)
{
    mapping attr;
    string a, best_a;
    int num;

    if( !mapp(attr = ob->query_attribute()) ) return "";

    best_a = "str";
    foreach(a, num in attr)
	if( num > attr[best_a] ) best_a = a;

    switch(best_a)
    {
	case "str": return "長得虎背熊腰，看起來相當孔武有力";
	case "dex": return "生得猿臂狼腰，身手定然十分矯捷";
	case "cor": return "看起來膽識過人，一副天不怕地不怕的模樣";
	case "cps": return "舉手投足間神色從容，頗有泰山崩於前面不改色的氣派";
	case "int": return "看起來十分機伶，想必聰穎過人";
	case "wis": return "外表氣宇軒昂，似乎頗富智慧";
	case "spi": return "的眼神深邃，給人一種世外高人的感覺";
	case "con": return "的身形勻稱，看起來體格相當不錯";
    }
}

// advance_level()
//
// This function is called by F_SCORE when a character's score reaches
// target_score. You can give bonus to the character here with race
// concern, i.e.  mature the character.

void advance_level(object ob)
{
    int roll;


    roll = random(100);
    if( roll < 30 ) {
	string attr, c_attr;
	if( roll==0 )		{ attr = "dex"; c_attr = "機敏"; }
	else if( roll < 2 )	{ attr = "spi"; c_attr = "靈性"; }
	else if( roll < 4 )	{ attr = "con"; c_attr = "根骨"; }
	else if( roll < 8 )	{ attr = "str"; c_attr = "膂力"; }
	else if( roll < 12 )	{ attr = "int"; c_attr = "悟性"; }
	else if( roll < 16 )	{ attr = "cps"; c_attr = "定力"; }
	else if( roll < 25 )	{ attr = "wis"; c_attr = "慧根"; }
	else			{ attr = "cor"; c_attr = "膽識"; }

	ob->set_attr( attr, (int)ob->query_attr(attr, 1) + 1 );
	tell_object(ob, HIW "你的" + c_attr + "值提高了﹗\n" NOR);
    }
}

// valid_wield()
//
// This function determines if a character of this race can wield a
// weapon in certain skill. When a character attempts to wield a weapon,
// the valid_wield() is called in his race daemon and class daemon to
// determine if he can wield it. In this example, we check that a human
// can only wield a primary weapon and a secondhand weapon.

int valid_wield(object me, object ob, string skill)
{
    mapping weapon;
    string *sk;
    int str, best_str;


    // Check strength
    str = me->query_attr("str");
    if( skill[0..8]=="twohanded" ) best_str = ob->weight() / 1000;
    else if( skill[0..9]=="secondhand" ) best_str = ob->weight() / 250;
    else best_str = ob->weight() / 500;

    if( str - best_str > 10 )
	write("這" + ob->query("unit") + ob->name()
		+ "拿在手裡輕飄飄的，不是十分趁手。\n");
    else if( str - best_str > 5 )
	write("這" + ob->query("unit") + ob->name()
		+ "對你來說有些輕了，使動起來覺得不怎麼實在。\n");
    else if( str - best_str > -1 )
	write("這" + ob->query("unit") + ob->name()
		+ "的重量對你來說剛剛好，相當趁手。\n");
    else if( str - best_str > -2 )
	write("這" + ob->query("unit") + ob->name()
		+ "的份量重了點，不過你勉強使得動。\n");
    else if( userp(me) )
	return notify_fail("這" + ob->query("unit") + ob->name()
		+ "對你來說太重了，你根本揮不動這麼沉重的兵刃。\n");

    weapon = me->query_temp("weapon");
    if( !mapp(weapon) || !sizeof(sk = keys(weapon)) ) return 1;

    if( sizeof(sk) >= 2 )
	return notify_fail("你的雙手都沒有空可以裝備這個武器。\n");
    if( skill[0..8]=="twohanded" )
	return notify_fail(ob->name() + "需要雙手。\n");
    if( sizeof(sk)==1 ) {
	if( sk[0][0..8]=="twohanded" )
	    return notify_fail("你正在使用需要雙手的武器。\n");
	if( sk[0][0..9] !="secondhand" && skill[0..9]!="secondhand" )
	    return notify_fail("使用兩把以上的武器必須為一主一副。\n");
	if( me->query_temp("armor/shield") )    
	    return notify_fail("你的雙手都沒有空可以裝備這個武器。\n");
    }
        
    return 1;
}

// valid_wear()
//
// This function determines if a character can wear an armor on certain
// body part. You shuold check if the character of this race DO HAVE that
// body part and didn't wear another armor on the same body part.

int valid_wear(object me, object ob, string part)
{
    if( me->query_temp("armor/" + part) )
	return notify_fail("你已經穿戴同類的護具了。\n");

    if( member_array(part, wear_type) == -1 )
	return notify_fail("你不能穿戴這一種護具。\n");

    return 1;
}

// statistic_destroyed()
//
// This function is called when the character of this race is physically
// damaged in any of his statistics. The most representive example is to
// check if the character is about to die.

int
statistic_destroyed(object ob, mapping flag)
{

    if( flag["gin"] ) {
	if( ob->query_stat_maximum("gin") > ob->query_attr("dex") )
	    ob->advance_stat("gin", -1);
	ob->heal_stat("gin", ob->query_attr("dex"));
    }

    if( flag["kee"] ) {
	if( ob->query_stat_maximum("kee") > ob->query_attr("con") )
	    ob->advance_stat("kee", -1);
	ob->heal_stat("kee", ob->query_attr("con"));
    }

    if( flag["sen"] ) {
	if( ob->query_stat_maximum("sen") > ob->query_attr("spi") )
	    ob->advance_stat("sen", -1);
	ob->heal_stat("sen", ob->query_attr("spi"));
    }

    if( flag["gin"] || flag["kee"] || flag["sen"] )
	ob->unconcious();

    return 1;
}

// statistic_exhausted()
//
// This function is called when the character of this race has exhausted
// any of his statistics. The most representive example is to check if the
// character is about to fall unconcious.

int
statistic_exhausted(object ob, mapping flag)
{
    object killer,ob1;

    killer = ob->last_damage_giver();
    if( ! killer ) killer = ob;

    // Dying
    // Note: this should be checked first bcz character often fall unconcious
    // and die at the same heart beat.
    if( flag["HP"] )
    {
	ob->die();
	if( !userp(ob) ) {
	    destruct(ob);
	    return 1;
	}
	switch( ob->query("life_form") )
	{
	case "ghost":
	    log_file("GHOST_DIE",
		sprintf("[%s] %s destroyed by %s\n",
			ctime(time()), ob->short(1), killer->short(1)));
	    CHAR_D->make_mist(ob);
	    break;
	case "living":
	    log_file("PLAYER_DIE",
		sprintf("[%s] %s killed by %s\n",
			ctime(time()), ob->short(1), killer->short(1)));
	    CHAR_D->make_ghost(ob);
	    break;
	}
	return 1;
    }

    // Unconcious
    if( flag["gin"] || flag["kee"] || flag["sen"] ) {
	ob->unconcious();
	ob->supplement_stat("gin", ob->query_attr("dex"));
	ob->supplement_stat("kee", ob->query_attr("con"));
	ob->supplement_stat("sen", ob->query_attr("spi"));
	return 1;
    }

    // Starving and thirsting
    if( flag["food"] ) {
	map_delete( flag, "food" );
	ob->consume_stat("kee", 1);
	tell_object(ob,
	    HIR "你已經餓得兩眼昏花，四肢無力了，最好找點東西來吃。\n" NOR);
    }
    if( flag["water"] ) {
	map_delete( flag, "water" );
	ob->consume_stat("kee", 1);
	tell_object(ob,
	    HIR "你覺得口乾舌燥，開始出現脫水的現象了，最好找點水喝。\n" NOR);
    }
    if( flag["fatigue"] ) map_delete(flag, "fatigue");

    return sizeof(flag)==0;
}
