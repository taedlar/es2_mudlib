// bag.c

inherit ITEM;

void create()
{
	set_name("麻布袋", ({ "bag" }));
	set_weight(700);
	set_max_encumbrance(30000);
	if( !clonep() ) {
		set("unit", "隻");
		set("value", 130);
		set("long", "一隻麻布口袋，看來可以裝不少東西。\n");
	}
	setup();
}

int accept_object() { return 1; }
