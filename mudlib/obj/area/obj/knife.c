// knife.c

#include <weapon.h>

inherit F_DAGGER;

void create()
{
	set_name("小刀", ({ "knife" }) );
	set_weight(900);
	setup_dagger(1, 14, 20, 0);

	if( !clonep() ) {
		set("wield_as", ({ "dagger", "secondhand dagger" }));
		set("unit", "把");
		set("value",70);
		set("long", "一柄粗糙的小刀，看來像是私造的凶器。\n");
		set("rigidity", 15);
	}
	setup();
}
