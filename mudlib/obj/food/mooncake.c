// manto.c

inherit COMBINED_ITEM;
inherit F_FOOD;

void create()
{
	set_name("月餅", ({"mooncake", "cake"}));
	if( !clonep() ) {
		set("unit", "堆");
		set("base_unit", "個");
		set("base_value", 1);
		set("base_weight", 300);
		set("food_stuff", 140);
		set("long", "一個又圓又大的月餅，就像天上的月亮一樣。\n");
	}
	setup();
}

