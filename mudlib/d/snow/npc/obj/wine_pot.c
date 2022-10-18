// wine_pot.c

inherit ITEM;

void create()
{
	object water;

	set_name("大酒缸", ({ "wine pot", "pot" }));
	set_max_encumbrance(180000);
	set("long", "一個裝滿酒的大酒缸﹐如果你不怕醉死的話﹐儘管舀來喝(drink)。\n");
	set("no_get", 1);
	set("liquid_container", 1);
	setup();
	if( clonep() ) {
		water = new("/d/choyin/npc/obj/red_wine");
		water->set_volume(150000);
		water->move(this_object());
	}
}

int accept_object(object me, object ob) { return 1; }

void reset()
{
object water;

if( water = present("red wine", this_object()) ) {
	water->set_volume(150000);
}
}

