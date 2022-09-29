// blue_cloth.c

#include <armor.h>

inherit F_CLOTH;

void create()
{
	set_name("藍布長衫", ({ "blue cloth", "cloth" }) );
	set_weight(2000);
	setup_cloth();

	if( !clonep() ) {
		set("unit", "件");
		set("value", 600);
		set("long", "一件粗布縫製的布衣﹐看起來像是鄉下姑娘的衣服。\n");
		set("wear_as", "cloth");
		set("apply_armor/cloth", ([
			"armor": 2,
		]));
	}

	setup();
}
