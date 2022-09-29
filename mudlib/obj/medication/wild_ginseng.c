// wild_ginseng.c

inherit COMBINED_ITEM;

void create()
{
	set_name("野山蔘", ({"wild ginseng", "ginseng"}));
	if( !clonep() ) {
		set("unit", "些");
		set("long",
			"野山蔘是一種廣為人知的補品﹐對人的精氣神都頗有助益﹐而且\n"
			"沒有什麼明顯的副作用﹐通常作為療傷藥的藥材﹐不過偶爾也有\n"
			"人直接服用﹐但是效果較不顯著。\n");
		set("base_unit", "根");
		set("base_value", 1500);
		set("base_weight", 400);
	}
	set_amount(1);
	setup();
}

int stuff_ob(object me)
{
	if( me->query_stat_maximum("food") - me->query_stat("food") < 50 )
		return notify_fail("你的肚子已經撐得什麼東西也吞不下去了。\n");
	message_vision("$N拿出一株野山蔘放入口中嚼了嚼吞了下去。\n", me);
	me->supplement_stat("food", 50);
	me->heal_stat("gin", 5);
	me->heal_stat("kee", 5);
	me->heal_stat("sen", 5);
	add_amount(-1);

	return 1;
}
