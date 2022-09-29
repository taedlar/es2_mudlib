// man.c

#include <npc.h>

inherit F_VILLAGER;

void create()
{
	set_name("路人", ({ "man" }) );
	set_race("human");
	set_level(2);
	set("long", "一個普通的路人﹐像這樣的人一天可以在路上碰見幾十個。\n");
	setup();
	carry_money("coin", 30);
	carry_object("/obj/area/obj/cloth")->wear();
	carry_object("/obj/area/obj/shortsword")->wield();
}
