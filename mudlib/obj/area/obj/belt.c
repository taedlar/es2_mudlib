// belt.c

#include <armor.h>

inherit F_WAIST_EQ;

void create()
{
	set_name("腰帶", ({ "belt" }) );
	set_weight(100);
	setup_waist_eq();

	if( !clonep() ) {
		set("unit", "件");
		set("value", 200);
		set("long",
			"一條用來束住褲子的腰帶，比較窮苦的人往往用褲子上的帶子繫住，稍微\n"
			"有點積蓄的人才會再外衣上加條腰帶。\n");
		set("wear_as", "waist_eq");
		set("apply_armor/waist_eq", ([
			"armor" :1,
		]));
	}

	setup();
}
