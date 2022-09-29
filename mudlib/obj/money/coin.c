// coin.c

inherit MONEY;

void create()
{
	set_name("錢", ({"coin", "coins", "coin_money" }));
	if( !clonep() ) {
		set("money_id", "coin");
		set("long", "這是流通中單位最小的貨幣﹐約要一百文錢才值得一兩銀子。\n");
		set("unit", "些");
		set("material", "copper");
		set("base_value", 1);
		set("base_unit", "文");
		set("base_weight", 3);
	}
	set_amount(1);
	setup();
}

