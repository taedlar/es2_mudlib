// broadsword.c

#include <weapon.h>

inherit F_SWORD;
inherit F_BLADE;

void create()
{
	set_name("闊劍", ({ "broadsword", "sword" }) );
	set_weight(12000);
	setup_sword(2, 12, 60, 1);
	setup_blade(2, 12, 80, 0);

	if( !clonep() ) {
		set("wield_as", ({ "sword", "twohanded sword", "blade" }));
		set("unit", "把");
		set("value", 4000);
		set("rigidity", 25);
		set("long", "一把精鋼鑄成的雙股鐵劍﹐這是官府軍士常用的武器。\n");
	}
	setup();
}
