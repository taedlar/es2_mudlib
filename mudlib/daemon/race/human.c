// vim: syntax=lpc

#define BASE_WEIGHT 40000

#include <ansi.h>
#include <statistic.h>
#include <race.h>

inherit HUMANOID;

private void
create()
{
    ::create();

    seteuid(getuid());

    set("karma", 5);        // karma cost to select human when reincarnate
    set("civilized", 1);    // human is a civilized humanoid tribe.

    set("commoner_score_base", 100);    // score base for gaining level

    DAEMON_D->register_race_daemon("human");
}


void setup(object ob)
{
    ::setup(ob);

    ob->set_default_object(__FILE__);

    if( !ob->query_weight() )
        ob->set_weight(BASE_WEIGHT + ((int)ob->query_attr("str", 1) - 13 ) * 5000);

    ob->add_temp("apply/armor", 3);
}

void initialize(object ob)
{
    ::initialize(ob);

    // We got 0.01% chance to be an Avatar!
    if( base_name(ob)==USER_OB && random(10000)==9999 ) {
        ob->set_race("avatar");
        return;
    }

    ob->init_attribute(([
        "str": 13 + random(6),
        "int": 13 + random(6),
        "wis": 13 + random(6),
        "dex": 13 + random(6),
        "con": 13 + random(6),
        "spi": 13 + random(6),
        "cor": 13 + random(6),
        "cps": 13 + random(6)
    ]));

    ob->init_statistic(([
        "gin": 30,
        "kee": 30,
        "sen": 30,
    ]));

    if( !ob->query("age") )
        if( (string)ob->query("gender")=="female" )
            ob->set("age", 13);
        else 
            ob->set("age", 14);

    ob->set_default_object(__FILE__);
}

