// broadaxe.c

#include <weapon.h>

inherit F_AXE;

void create()
{
	set_name("板斧", ({ "broadaxe", "axe" }) );
	set_weight(10000);
	setup_axe(2, 11, 100, 0);

	if( !clonep() ) {
		set("wield_as", ({ "axe", "twohanded axe" }) );
		set("unit", "把");
		set("value", 5000);
		set("rigidity", 25);
		set("long", "一把大面板斧，份量著實沉重，不過板斧的斧刃很長，是相當厲害的兵刃。\n");
	}
	setup();
}

void varify_template(object owner)
{
	int i = owner->query_level();
	
	ENHANCE_D->enhance_weapon(this_object(), i*i + 10, "斧");
}
