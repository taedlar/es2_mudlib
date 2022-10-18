// clothboot.c

#include <armor.h>

inherit F_FEET_EQ;

void create()
{
	set_name("皂步靴", ({ "black boots", "boots" }));
	set_weight(1000);
	setup_feet_eq();

	if( !clonep() ) {
		set("unit", "雙");
		set("long", "這是軍隊中的步兵武官常穿的靴子﹐輕便耐用。\n");
		set("value", 1000);
		set("wear_as", "feet_eq");
		set("apply_armor/feet_eq", ([
			"armor" : 1,
			"dodge" : 1,
		]) );
	}
	setup();	
}