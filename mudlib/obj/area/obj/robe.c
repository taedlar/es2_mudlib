// robe.c

#include <armor.h>

inherit F_CLOTH;

void create()
{
	set_name("布袍", ({ "robe" }) );
	set_weight(1000);
	setup_cloth();

	if( !clonep() ) {
		set("unit", "件");
		set("value", 600);
		set("long", "一套普通的棉布袍子，上面有一些灰塵。\n");
		set("wear_as", "cloth");
		set("apply_armor/cloth", ([
			"armor": 1,
		]));
	}

	setup();
}
