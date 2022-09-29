// flower_pill.c

inherit COMBINED_ITEM;

void create()
{
	set_name("紅花丸", ({"flower pill", "pill"}));
	if( !clonep() ) {
		set("unit", "些");
		set("long",
			"紅花丸是一種治療腹瀉的常見丹方，不過製造相當費時麻煩，多\n"
			"半要到大一點的市鎮才買得到。\n");
		set("base_unit", "粒");
		set("base_value", 150);
		set("base_weight", 5);
	}
	set_amount(1);
	setup();
}

int stuff_ob(object me)
{
	if( me->query_stat_maximum("food") - me->query_stat("food") < 5 )
		return notify_fail("你的肚子已經撐得連這粒藥丸也吞不下去了。\n");
	message_vision("$N吞下一粒暗紅色的藥丸。\n", me);
	me->consume_stat("water", 30);
	me->supplement_stat("food", 5);

	// TODO: implement medical function.

	add_amount(-1);

	return 1;
}
