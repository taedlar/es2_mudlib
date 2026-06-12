/*---
description: 化身神的定義。
author: Annihilator <taedlar@gmail.com>
---*/
#define BASE_WEIGHT 40000

#include <ansi.h>
#include <statistic.h>
#include <race.h>

inherit HUMANOID;

private void create() {
    ::create();

    seteuid (getuid());
    set ("karma", 100);      // karma cost to select avatar when reincarnate
    set ("civilized", 1);    // avatar is a civilized humanoid tribe.
    set ("commoner_score_base", 100);    // score base for gaining level

    DAEMON_D->register_race_daemon ("avatar");
}

void setup (object ob) {
    ::setup (ob);

    ob->set_default_object (__FILE__);
    if (!ob->query_weight())
        ob->set_weight (BASE_WEIGHT + ((int)ob->query_attr("str", 1) - 13 ) * 5000);

    ob->add_temp ("apply/armor", 5); // avatar natural armor = 5
}

void initialize (object ob) {
    ::initialize (ob);

    // avatar has a +3 bonus to all attributes, but with the same maximum as humans.
     ob->init_attribute (([
        "str": 16 + random(3),
        "int": 16 + random(3),
        "wis": 16 + random(3),
        "dex": 16 + random(3),
        "con": 16 + random(3),
        "spi": 16 + random(3),
        "cor": 16 + random(3),
        "cps": 16 + random(3)
    ]));

    ob->init_statistic(([
        "gin": 30,
        "kee": 30,
        "sen": 30,
    ]));

    if (!ob->query ("age")) {
        ob->set ("age", 14 + random(4)); // start age: 14-17
    }

    ob->set_default_object (__FILE__);
}
