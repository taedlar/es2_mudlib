// tummy_wear.c

#include <armor.h>

inherit F_CLOTH;

void create()
{
	set_name("肚兜", ({ "tummy cover", "tummy", "cover" }) );
	set_weight(1000);
	setup_cloth();

	if( !clonep() ) {
		set("unit", "件");
		set("value", 400);
		set("long", "一塊兩尺見方的粗布﹐繫上兩條繩兒用來給小孩穿的衣物。\n");
		set("wear_as", "cloth");
		set("apply_armor/cloth", ([
			"armor": 1,
		]));
	}

	setup();
}
