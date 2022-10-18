// hairpin.c

#include <armor.h>

inherit F_HEAD_EQ;

void create()
{
	set_name("古銅髮簪", ({ "bronze hairpin", "hairpin" }));
	set_weight(400);
	setup_head_eq();
	if( !clonep() ) {
		set("unit", "支");
		set("value", 100);
		set("long", "一支生滿銅綠﹐老舊不堪的髮簪。\n");
		set("wear_as", "head_eq");
		set("apply_armor/head_eq/spi", 1);
	}
	setup();
}
