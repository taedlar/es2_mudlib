// dice.c

inherit ITEM;

void create()
{
	set_name("骰子", ({ "dice"}));
	set_weight(3);
	if( !clonep() ) {
		set("unit", "粒");
		set("value", 5);
		set("long",
			"一粒骰子，你可以用 roll 擲骰子玩。\n");
	}
	setup();
}

void init()
{
	add_action("do_roll", ({ "roll", "throw" }));
}

int do_roll(string arg)
{
	if( !id(arg) ) return 0;

	message_vision("$N拿出骰子一擲，骰子滾了滾停下來，結果是個"
		+ (random(6)+1) + "點。\n", this_player());
	return 1;
}
