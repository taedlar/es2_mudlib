// coin.c

inherit ITEM;

void create()
{
	set_name("古錢", ({ "ancient coin", "coin" }));
	set_weight(3);
	if( !clonep() ) {
		set("unit", "枚");
		set("value", 60);
		set("long", "一枚黑黝黝的古錢，看不出有什麼特別。\n");
		set("thief_token", 1);
	}
	setup();
}
