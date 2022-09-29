// leather_armor.c

#include <armor.h>

inherit F_ARMOR;

void create()
{
	set_name("硬皮甲冑", ({ "leather armor", "armor" }));
	set_weight(10000);
	setup_armor();

	if( !clonep() ) {
		set("unit", "件");
		set("long",
			"一件用牛皮漿硬縫合﹐用來保護身體的甲冑﹐雖然防護效果比不上\n"
			"金屬製成的鎧甲﹐但是輕便便宜也是這類皮製甲冑的優點。\n");
		set("value", 1400);
		set("wear_as", "armor");
		set("apply_armor/armor", ([
			"armor": 6
		]) );
	}
	setup();
}
