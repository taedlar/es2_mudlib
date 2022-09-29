// cloth.c

#include <armor.h>

inherit F_CLOTH;

void create()
{
	set_name("布衣", ({ "cloth" }) );
	set_weight(2000);
	setup_cloth();

	if( !clonep() ) {
		set("unit", "件");
                set("value",60);
		set("long", "一件粗布縫製的布衣﹐看起來像是鄉下人的衣服。\n");
		set("wear_as", "cloth");
		set("apply_armor/cloth", ([
			"armor": 1,
		]));
	}

	setup();
}
