// black_pill.c

inherit COMBINED_ITEM;

void create()
{
	set_name("烏心丹", ({"black pill", "pill"}));
	if( !clonep() ) {
		set("unit", "些");
		set("long",
			"烏心丹是一種民間很普遍的藥方﹐用來恢復因操練、工作所耗的體力\n"
			"有不錯的效果﹐缺點是吃了以後會昏昏沈沈想睡覺﹐且容易口渴。\n");
		set("base_unit", "粒");
		set("base_value", 80);
		set("base_weight", 5);
		// 藥方的價格
		set("mixture_value", 30);
		// 煉製參數
		set("produce_param", ([
			"min_heat_power" : 80,
			"max_heat_power" : 250,
			//change from 1000/1500 to  100/150
			"min_heat" : 100,
			"max_heat" : 150,
		]));
		// 抄錄藥方所需最技能下限
		set("requirement", ([
			"literate" : 1,
		]));
	}
	set_amount(1);
	setup();
}

int stuff_ob(object me)
{
	if( me->query_stat_maximum("food") - me->query_stat("food") < 5 )
		return notify_fail("你的肚子已經撐得連這粒藥丸也吞不下去了。\n");
	message_vision("$N吞下一粒黑黑的藥丸。\n", me);
	me->consume_stat("water", 30);
	me->supplement_stat("food", 5);
	me->damage_stat("sen", 1);
	me->consume_stat("sen", 4);
	me->supplement_stat("gin", 15);
	me->supplement_stat("kee", 15);
	add_amount(-1);

	return 1;
}
