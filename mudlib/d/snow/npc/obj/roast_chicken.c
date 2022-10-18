// roast_chicken.c

inherit COMBINED_ITEM;
inherit F_FOOD;

void create()
{
	set_name("紅燒雞", ({ "roast chicken", "chicken" }) );
	if( !clonep() ) {
		set("unit", "堆");
		set("base_unit", "隻");
		set("base_value", 800);
		set("base_weight", 1500);
		set("food_stuff", 190);
		set("heal_kee", 30);
		set("long", "一隻香噴噴的紅燒雞﹐哇﹐你的口水都快流出來了。\n");
	}
	setup();
}

