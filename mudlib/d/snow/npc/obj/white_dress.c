// white_dress.c

#include <armor.h>

inherit F_CLOTH;

void create()
{
	set_name("白綢羽衫", ({ "white dress", "dress" }) );
	set_weight(2000);
	setup_cloth();

	if( !clonep() ) {
		set("unit", "件");
		set("value", 3200);
		set("long", "一件白色的絲綢女子衣衫﹐摸起來輕柔無比。\n");
		set("wear_as", "cloth");
		set("apply_armor/cloth", ([
			"armor": 3,
			"spi": 1
		]));
	}

	setup();
}
