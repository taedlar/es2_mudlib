// cinnabar.c

inherit REAGENT_ITEM;

void create()
{
	set_name("辰砂", ({ "cinnabar" }));
	set_weight(500);
	if( !clonep() ) {
		set("unit", "包");
		set("long", "一種紅色的礦砂，加熱可以得到汞，是煉丹的重要材料。\n");
		set("unit_value", 0.6);
	}
	setup();
}
