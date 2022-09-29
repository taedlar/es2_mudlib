// aquapill.c

inherit COMBINED_ITEM;

void create()
{
	set_name("金液丹", ({"aquapill", "pill"}));
	if( !clonep() ) {
		set("unit", "些");
		set("long",
			"金液丹是一種方家丹藥的總稱，主要的功\能是療傷，金液丹的療傷\n"
			"功\效相當不錯，而且所需的藥材很容易買到，但是因為煉製過程比\n"
			"較需要技術，所以只有大城市的藥舖才買得到。\n");
		set("base_unit", "粒");
		set("base_value", 160);
		set("base_weight", 5);
		set("mixture_value", 50);
		set("produce_param", ([
			"min_heat_power" : 90,
			"max_heat_power" : 180,
			// change from 4200/4500 to 2700/3000 by dragoon
			"min_heat" : 420,
			"max_heat" : 450,
		]));
		// skill changed from 5 to 30 by dragoon
		set("requirement", ([
			"literate" : 10,
			"alchemy-medication" : 30,
		]));
	}
	set_amount(1);
	setup();
}

int stuff_ob(object me)
{
	if( me->query_stat_maximum("food") - me->query_stat("food") < 20)
		return notify_fail("你的肚子已經撐得連這粒藥丸也吞不下去了。\n");
	message_vision("$N吞下一粒金液丹。\n", me);
	me->consume_stat("water", 10);
	// changed from 15 to 5 by dragoon
	me->supplement_stat("food", 5);
	me->damage_stat("gin", 3);
	me->consume_stat("gin", 5);
	me->heal_stat("kee", 25);
	me->supplement_stat("kee", 25);
	add_amount(-1);

	return 1;
}
