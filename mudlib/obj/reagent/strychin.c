// strychin.c

inherit REAGENT_ITEM;

void create()
{
	set_name("番木鱉", ({ "strychin" }));
	set_weight(30);
	if( !clonep() ) {
		set("unit", "些");
		set("long", "一種含劇毒的植物所提煉的藥材，價格不非。\n");
		set("unit_value", 35);
	}
	setup();
}
