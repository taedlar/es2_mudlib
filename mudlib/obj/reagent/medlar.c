// medlar.c

inherit REAGENT_ITEM;

void create()
{
	set_name("枸杞", ({ "medlar" }));
	set_weight(100);
	if( !clonep() ) {
		set("unit", "些");
		set("unit_value", 5);
		set("long", "常見的藥材，多用於提煉治療眼疾的丹藥。\n");
	}
	setup();
}
