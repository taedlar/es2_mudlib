// boar_berry.c

inherit COMBINED_ITEM;

void create()
{
	set_name("山豬果", ({"boar berry", "berry"}));
	if( !clonep() ) {
		set("unit", "些");
		set("long",
			"山豬果是一種生長在深山中﹐尤其是野豬的巢穴附近的一種暗紅色\n"
			"野莓﹐有些山豬果是有毒的﹐有些則是無毒的﹐無毒的山豬果可以\n"
			"用來當解毒劑的藥材﹐直接服用則可以提神﹐不過先警告你﹐味道\n"
			"不....太好。\n");
		set("base_unit", "顆");
        set("base_value", 2000);
		set("base_weight", 70);
	}
	set_amount(1);
	setup();
}

int stuff_ob(object me)
{
	if( me->query_stat_maximum("food") - me->query_stat("food") < 20 )
		return notify_fail("你的肚子已經撐得什麼東西也吞不下去了。\n");
	message_vision("$N吃下一顆發著惡臭的山豬果 ... 天啊﹐$P竟然吞得下去 .... 。\n", me);
    me->supplement_stat("food", 10);
	me->consume_stat("water", 20);
    me->heal_stat("sen", 15);
    me->supplement_stat("sen", 10);
	add_amount(-1);

	return 1;
}
