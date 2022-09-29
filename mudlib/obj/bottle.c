// bottle.c
// This is the DEFAULT container for clone command to contain cloned liquid.

inherit ITEM;

void create()
{
	set_name("玻璃瓶", ({ "glass bottle", "bottle" }));
	set_weight(700);
	set_max_encumbrance(3000);
	if( !clonep() ) {
		set("unit", "個");
                set("value",100);
		set("container_unit", "瓶");
		set("long", "一個透明的玻璃瓶。\n");
		set("liquid_container", 1);
	}
	setup();
}

int accept_object(object me, object ob)
{
	if( ob->weight() > 500
	&&	!function_exists("set_volume", ob) )
		return notify_fail("玻璃瓶口太小了﹐裝不進" + ob->name() + "。\n");

	return 1;
}
