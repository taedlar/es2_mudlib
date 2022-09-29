// green_pill.c

inherit COMBINED_ITEM;

void do_cure_poison(object me);

void create()
{
	set_name("甘靈清毒丹", ({"green pill", "pill"}));
	if( !clonep() ) {
		set("unit", "些");
		set("long",
"甘靈清毒丹是一種清熱解毒的丹藥,由黃芩, 川貝母, 木通, 飛滑石\n"
"等多種藥材鍊製而成, 為相當普遍的解藥藥物之一。\n");
		set("base_unit", "粒");
		set("base_value", 650);
		set("base_weight", 5);
		set("mixture_value", 100);
		set("produce_param", ([
			"min_heat_power" : 85,
			"max_heat_power" : 230,
			"min_heat" : 150,
			"max_heat" : 200,
		]));
		set("cure_poison", ([
		"金" : -1, "水" : -1, "火" : -1, "土" : -1, "木" : -1,
		"damage" :   -1,
		]));
		set("requirement", ([
			"literate" : 5,
			"alchemy-medication" : 15,
		]));
	}
	set_amount(1);
	setup();
}

int stuff_ob(object me)
{
	if( me->query_stat_maximum("food") - me->query_stat("food") < 20)
		return notify_fail("你的肚子已經撐得連這粒藥丸也吞不下去了。\n");
	message_vision("$N吞下一粒甘靈清毒丹。\n", me);
	me->consume_stat("water", 10);
	me->supplement_stat("food", 5);
	me->supplement_stat("sen", 2);
	do_cure_poison( me );	
	add_amount(-1);
	return 1;
}

void do_cure_poison(object me)
{
        mixed cnd_data;
        object ob;
        ob=this_object();
        cnd_data = me->query_condition("poison");

	if ( me->query_condition("poison") == 0 ) {
		tell_object(me, "你吞下這顆藥丸, 但是卻沒什麼感覺。\n");
	} else {
		if ( ob->query("cure_poison/damage") )
		cnd_data["damage"] = cnd_data["damage"] + 
			ob->query("cure_poison/damage");
                if (cnd_data["damage"] <= 0 ){
                        me->delete_condition("poison");
	                tell_object(me, 
			"你吞下這顆藥丸後, 身體的不適已經完全消除了。\n");
                        return;
                }
                else {
	                tell_object(me, 
			"你吞下這顆藥丸後, 身體的不適感似乎稍微減輕了。\n");
                }
                return;
        }
}
