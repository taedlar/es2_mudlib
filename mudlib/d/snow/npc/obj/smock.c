// smock.c

#include <armor.h>

inherit F_ARMOR;

void create()
{
	set_name("打鐵褂子", ({ "work smock", "smock" }) );
	set_weight(5500);
	setup_armor();

	if( !clonep() ) {
		set("unit", "件");
		set("value", 720);
		set("long", "這是一件鐵匠工作時用來遮擋火星飛濺的工作服。\n");
		set("wear_as", "armor");
		set("apply_armor/armor", ([
			"armor": 2,
			"armor_vs_fire": 5,
		]));
	}

	setup();
}
