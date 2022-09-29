// pork.c

inherit COMBINED_ITEM;
inherit F_FOOD;

void create()
{
	set_name("白切肉", ({ "pork" }) );
	if( !clonep() ) {
		set("unit", "堆");
		set("base_unit", "片");
		set("base_value", 200);
		set("base_weight", 400);
		set("heal_kee", 10);
		set("food_stuff", 90);
		set("long", "普通的水煮白切肉 ... 。\n");
	}
	setup();
}

