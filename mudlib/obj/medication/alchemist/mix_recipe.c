// minor_heal_pill.c

inherit COMBINED_ITEM;

void create()
{
	set_name("混合藥丹", ({"mixed pill", "mixed", "pill" }));
	if( !clonep() ) {
		set("unit", "些");
		set("long",
			"這是一種正在測試中的藥丹, 藥性未知, 請勿誤食, 否則後果自負。\n");
		set("base_unit", "粒");
		set("base_value", 1000);
		set("base_weight", 5);
		set("mixture_value", 250);
		set("produce_param", ([
			"min_heat_power" : 130,
			"max_heat_power" : 150,
			"min_heat" : 4000,
			"max_heat" : 4500,
		]));
		set("requirement", ([
			"literate" : 15,
			"alchemy-medication" : 30,
		]));
	}
	set_amount(1);
	setup();
}

int stuff_ob(object me)
{
	if( me->query_stat_maximum("food") - me->query_stat("food") < 5 )
		return notify_fail("你的肚子已經撐得連這粒藥丸也吞不下去了。\n");
	message_vision("$N吞下一粒小還丹。\n", me);
	me->consume_stat("water", 20);
	me->supplement_stat("food", 3);
	me->damage_stat("gin", 3);
	me->consume_stat("gin", 5);
	me->heal_stat("kee", 35);
	me->supplement_stat("kee", 60);
	me->supplement_stat("HP", 20);
	add_amount(-1);

	return 1;
}
