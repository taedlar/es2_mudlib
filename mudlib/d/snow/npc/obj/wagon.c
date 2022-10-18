// wagon.c

inherit CONTAINER_ITEM;

void create()
{
	set_name("運貨驢車", ({ "wagon" }));
	set_max_encumbrance(1000000);
	set("no_get", 1);
	set("unit", "輛");
	set("long", "一輛運貨的四輪驢車﹐停在這裡等著載貨(load)。\n");
	set("inside_long",
		"你現在正坐在一輛運貨用的四輪驢車上。\n");
	set("transparent", 1);
	setup();
}

void init()
{
	add_action("do_load", "load");
}

int do_load(string arg)
{
	object ob;

	if( 	!arg 
	||	!sscanf(arg, "wagon with %s", arg)
	&&	!sscanf(arg, "%s into wagon", arg) )
		return notify_fail("你可試試: load wagon with 東西 或 load 東西 into wagon.\n");

	ob = present(arg, this_player());
	if( !ob ) return notify_fail("你身上沒有這件東西, 怎麼運上車﹖\n");
	if( environment(this_player())==this_player() )
		return notify_fail("你必須先下車。\n");

	message_vision("$N把$n裝上運貨驢車。\n", this_player(), ob);
	ob->move(this_object());
	tell_object(this_player(), "呼  呼.. 好累喔。\n");
	this_player()->consume_stat("kee",5);
	this_player()->add_temp("wage_deserved", ob->query("wage"));

	if( sizeof(all_inventory()) >= 20 ) {
		write("趕驢車的車夫鞭子一揚﹐趕著驢車送貨去了。\n");
		destruct(this_object());
	}

	return 1;
}

// To prevent player from getting things in wagon.
int hold_object(object ob)
{
	notify_fail("什麼﹖\n");
	return 1;
}
