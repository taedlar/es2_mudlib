// gold.c

inherit MONEY;

void create()
{
	set_name("黃金", ({"gold", "ingot", "gold_money"}));
	if( !clonep() ) {
		set("money_id", "gold");
		set("long", "黃澄澄的金子﹐人見人愛的金子﹐啊～～金子﹗\n");
		set("unit", "些");
		set("material", "gold");
		set("base_value", 10000 );
		set("base_unit", "兩");
		set("base_weight", 37);
	}
	set_amount(1);
	setup();
}

