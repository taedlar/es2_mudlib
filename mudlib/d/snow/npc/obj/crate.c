// crate.c

inherit ITEM;

void create()
{
	set_name("大木箱", ({ "big crate", "crate"}));
	set_weight(40000);
	if( !clonep() ) {
		set("unit", "個");
		set("long", "這是一個用來裝貨物的大木箱﹐看起來相當沈重。\n");
		set("value", 1);
		set("wage", 25);
	}
	setup();
}

void init()
{
	add_action("do_get", "get");
}

int do_get(string arg)
{
	if( environment() != environment(this_player())
	||	!id(arg) ) return 0;

	if( (int)this_player()->query_stat("kee") < 10 ) {
		write("你的力氣不夠了﹐休息一下再說吧。\n");
		return 1;
	}

	if( move(this_player()) ) {
		message_vision("$N彎下腰將地上的" + name() + "扛了起來。\n", this_player());
		return 1;
	}
}

