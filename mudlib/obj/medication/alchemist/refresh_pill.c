// refresh_pill.c

inherit COMBINED_ITEM;

void create()
{
	set_name("金鎖固精丸", ({"refresh pill", "pill"}));
	if( !clonep() ) {
		set("unit", "些");
		set("long",
"金鎖固精丸為合芡實, 龍骨, 連鬚, 潼蒺藜及牡蠣數種藥材所鍊製\n"
"而成是一種方家丹藥的總稱，具澀精之療效。\n");
		set("base_unit", "粒");
		set("base_value", 1000);
		set("base_weight", 5);
		set("mixture_value", 200);
		set("produce_param", ([
			"min_heat_power" : 110,
			"max_heat_power" : 170,
			"min_heat" : 400,
			"max_heat" : 428,
		]));
		set("requirement", ([
			"literate" : 15,
			"alchemy-medication" : 50,
		]));
	}
	set_amount(1);
	setup();
}

int stuff_ob(object me)
{
	if( me->query_stat_maximum("food") - me->query_stat("food") < 20)
		return notify_fail("你的肚子已經撐得連這粒藥丸也吞不下去了。\n");
	message_vision("$N吞下一粒金鎖固精丸。\n", me);
	me->consume_stat("water", 10);
	me->supplement_stat("food", 5);
	me->heal_stat("gin", 25 + random(7));
	me->supplement_stat("gin", 20 + random(15));
	me->consume_stat("sen", 8);
	add_amount(-1);

	return 1;
}
