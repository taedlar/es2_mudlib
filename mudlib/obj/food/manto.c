// manto.c

inherit COMBINED_ITEM;
inherit F_FOOD;

void create()
{
	set_name("饅頭", ({ "manto" }));
	if( !clonep() ) {
		set("unit", "堆");
		set("base_unit", "個");
		set("base_value", 10);
		set("base_weight", 100);
		set("food_stuff", 80);
		set("long", "饅頭 ... 跟你想像中的一模一樣﹐感動吧。\n");
	}
	setup();
}

