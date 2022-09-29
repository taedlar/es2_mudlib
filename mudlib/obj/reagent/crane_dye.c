// crane_dye.c

inherit REAGENT_ITEM;

void create()
{
	set_name("鶴頂紅", ({ "crane dye", "dye" }));
	set_weight(200);
	if( !clonep() ) {
		set("unit", "些");
		set("long", "一種含劇毒的藥材，但是有些少見的藥方需要這味藥材。\n");
		set("unit_value", 15);
	}
	setup();
}
