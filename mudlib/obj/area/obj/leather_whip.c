// leather_whip.c

#include <weapon.h>

inherit F_WHIP;

void create()
{
	set_name("皮鞭", ({ "leather whip", "whip" }) );
	set_weight(2000);
	setup_whip(4, 5, 70, 0);

	if( !clonep() ) {
		set("wield_as", "whip");
		set("unit", "條");
		set("value",1800);
		set("long", "一條約九尺多長的皮鞭，可以用來當作兵刃使用。\n");
		set("rigidity", 15);
	}
	setup();
}
