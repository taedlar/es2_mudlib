// leather_boot.c

#include <armor.h>

inherit F_FEET_EQ;

void create()
{
	set_name("皮短靴", ({ "leather boots", "boots" }));
	set_weight(1000);
	setup_feet_eq();

	if( !clonep() ) {
		set("unit", "雙");
		set("long", "這是一雙皮製的短靴﹐看起來有些舊了。\n");
		set("value", 400);
		set("wear_as", "feet_eq");
		set("apply_armor/feet_eq", ([
			"armor" : 1
		]) );
	}

	setup();
}
