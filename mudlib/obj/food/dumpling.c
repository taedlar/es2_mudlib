// dumpling.c

inherit COMBINED_ITEM;
inherit F_FOOD;

void create()
{
	set_name("牛肉包子", ({ "beef dumpling", "dumpling" }) );
	if( !clonep() ) {
		set("unit", "堆");
		set("base_unit", "個");
		set("base_value", 10);
		set("base_weight", 60);
		set("food_stuff", 80);
		set("long", "一個香噴噴的牛肉包子。\n");
	}
	setup();
}

