// axe.c

#include <weapon.h>

inherit F_AXE;

void create()
{
	set_name("斧頭", ({ "axe" }) );
	set_weight(10000);
	setup_axe(2, 12, 80, 0);

	if( !clonep() ) {
		set("wield_as", ({ "axe", "twohanded axe" }) );
		set("unit", "把");
		set("value", 2700);
		set("long", "這是一把尋常的斧頭，多半用來砍樹劈柴用。\n");
		set("rigidity", 22);
	}
	setup();
}
