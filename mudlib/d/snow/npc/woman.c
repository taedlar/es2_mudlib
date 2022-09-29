// woman.c

#include <npc.h>

inherit F_VILLAGER;

void create()
{
	set_name("少婦", ({ "woman" }) );
	set_race("human");
	set_level(2);

	set("gender", "female");
	set("long", "一個容色秀麗的少婦﹐穿著十分樸素。\n");
	setup();
	carry_money("coin", 10);
	carry_object("/obj/area/obj/cloth")->wear();
}
