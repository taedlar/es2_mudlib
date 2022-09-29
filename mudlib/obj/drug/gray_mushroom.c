// gray_mushroom.c

inherit COMBINED_ITEM;

void create()
{
	set_name("灰草菇", ({ "gray mushroom", "mushroom"}));
	if( !clonep() ) {
		set("unit", "些");
		set("long",
			"灰草菇是一種生長在潮濕的湖邊沼地，可以用來治療刀傷的\n"
			"細小草菇，通常提煉以後用來製造外敷的膏藥，不過直接服\n"
			"用也有少許\治療內傷的功\效。\n");
		set("base_unit", "株");
		set("base_value", 90);
		set("base_weight", 30);
	}
	set_amount(1);
	setup();
}

int stuff_ob(object me)
{
	if( me->query_stat_maximum("food") - me->query_stat("food") < 20 )
		return notify_fail("你的肚子已經撐得什麼東西也吞不下去了。\n");
	message_vision("$N拿出一株灰草菇放入嘴裡，嚼了嚼吞了下去。\n", me);
	me->supplement_stat("food", 20);
	me->consume_stat("water", 20);
	me->heat_stat("gin", 3);
	me->heat_stat("kee", 3);
	add_amount(-1);

	return 1;
}
