// minor_heal_pill.c

inherit COMBINED_ITEM;

void create()
{
	set_name("小還丹", ({"minor heal pill", "minor", "pill" }));
	if( !clonep() ) {
		set("unit", "些");
		set("long",
			"小還丹是一種相當神奇的仙丹，可以讓服用者的外傷在瞬間癒合，換句\n"
			"話說，相對於一般只能恢復精氣神的普通丹藥，小還丹還能夠恢復形體\n"
			"所受的損傷，不過煉製小還丹所需的藥材頗為昂貴，而且燒煉時必須用\n"
			"大量的汞護住丹爐，因此所費不貲，很少有藥舖會販賣這種丹藥。\n");
		set("base_unit", "粒");
		set("base_value", 6000);
		set("base_weight", 5);
		set("mixture_value", 250);
		set("produce_param", ([
			"min_heat_power" : 130,
			"max_heat_power" : 150,
			// change from 8000 to 800 by dragoon
			"min_heat" : 800,
			// change from 8200 to 820 by dragoon
			"max_heat" : 820,
		]));
		// skill changed from 30 to 60 by dragoon
		set("requirement", ([
			"literate" : 15,
			"alchemy-medication" : 60,
		]));
	}
	set_amount(1);
	setup();
}

int stuff_ob(object me)
{
	if (me->query_stat_maximum("food") - me->query_stat("food") < 8)
		return notify_fail("你的肚子已經撐得連這粒藥丸也吞不下去了。\n");
	message_vision("$N吞下一粒小還丹。\n", me);
	me->consume_stat("water", 15);
	// changed from 20 to 5 by dragoon
	me->supplement_stat("food", 5);
	me->damage_stat("gin", 5);
	me->consume_stat("gin", 5);
	me->heal_stat("kee", 35);
	me->supplement_stat("kee", 60);
	me->supplement_stat("HP", 20);
	add_amount(-1);

	return 1;
}
