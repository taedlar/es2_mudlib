// hoe.c

#include <weapon.h>

inherit F_PIKE;

void create()
{
	set_name("鋤頭", ({ "hoe" }) );
	set_weight(6000);
	setup_pike(2, 10, 50, 0);

	if( !clonep() ) {
		set("wield_as", ({ "twohanded pike" }));
		set("unit", "把");
		set("value", 800);
		set("rigidity", 20);
		set("long", "一把重甸甸的鋤頭。\n");
	}
	setup();
}
