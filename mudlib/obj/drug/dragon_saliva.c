// dragon_saliva.c

inherit COMBINED_ITEM;

void create()
{
	set_name("龍涎草", ({"dragon-saliva herb", "dragon-saliva", "herb"}));
	if( !clonep() ) {
		set("unit", "些");
		set("long",
			"龍涎草是一種相當名貴的療傷藥﹐一般人是不太買得起這種藥材的﹐\n"
			"龍涎草經常被方士們用於煉製一些珍貴的丹藥﹐但是直接服食這種草\n"
			"藥也有些許\的療傷效果﹐只不過有些浪費罷了。\n");
		set("base_unit", "株");
		set("base_value", 5000);
		set("base_weight", 7);
	}
	set_amount(1);
	setup();
}

int stuff_ob(object me)
{
	if( me->query_stat_maximum("food") - me->query_stat("food") < 10 )
		return notify_fail("你的肚子已經撐得什麼東西也吞不下去了。\n");
	message_vision("$N拿出一株龍涎草放入口中嚼了嚼吞了下去。\n", me);
	me->consume_stat("water", 10);
	me->supplement_stat("food", 10);
	me->consume_stat("sen", 3);
	me->heal_stat("kee", 30);
	add_amount(-1);

	return 1;
}
