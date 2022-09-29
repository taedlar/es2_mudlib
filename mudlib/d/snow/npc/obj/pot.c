// pot.c

inherit ITEM;

void create()
{
	object water;

	set_name("大水缸", ({ "pot" }));
	set_max_encumbrance(180000);
	set("long", "一個裝滿清水的大水缸﹐如果你口渴﹐可以舀水來喝(drink)。\n");
	set("no_get", 1);
	set("liquid_container", 1);
	setup();
	if( clonep() ) {
		water = new("/obj/water");
		water->set_volume(100000);
		water->move(this_object());
	}
}

varargs int accept_object(object me, object ob)
{
	if( ob )
		if( !userp(ob) )
			return 1;
	else return notify_fail("你不能將玩家放到容器裡面。\n");
}

void reset()
{
	object water;
	if( water = present("water", this_object()) )
		water->set_volume(100000);
}

