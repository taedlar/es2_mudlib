// silver.c

inherit MONEY;

void create()
{
	set_name("碎銀", ({"silver", "silver_money"}));
	if( !clonep() ) {
		set("money_id", "silver");
		set("long", "白花花的銀子﹐人見人愛的銀子。\n");
		set("unit", "些");
		set("material", "silver");
		set("base_value", 100);
		set("base_unit", "兩");
		set("base_weight", 37);
	}
	set_amount(1);
	setup();
}

