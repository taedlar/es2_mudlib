//blue_jacket.c
#include <armor.h>

inherit F_CLOTH;

void create()
{
	set_name("藍衣套", ({ "blue jacket", "jacket" }) );
	set_weight(2000);
	setup_cloth();
	if( !clonep() ) {
		set("unit", "件");
		set("value", 500);
		set("long", "一件深藍色的大衣﹐上面繡了老大一個\"魚\"字。\n");
		set("wear_as", "cloth");
		set("apply_armor/cloth", ([
			"armor": 2,
		]));
	}
	setup();
}