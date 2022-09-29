// shovel.c

#include <weapon.h>

inherit F_PIKE;

void create()
{
	set_name("鐵鏟", ({ "shovel" }) );
	set_weight(6000);
	setup_pike(2, 9, 70, 0);

	if( !clonep() ) {
		set("wield_as", ({ "twohanded pike" }));
		set("unit", "把");
		set("value", 700);
		set("rigidity", 20);
		set("long", "一把挖土用的鐵鏟。\n");
	}
	setup();
}
