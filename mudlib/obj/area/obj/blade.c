// blade.c

#include <weapon.h>

inherit F_BLADE;

void create()
{
	set_name("單刀", ({ "blade" }) );
	set_weight(8000);
	setup_blade(2, 10, 60, 0);

	if( !clonep() ) {
		set("wield_as", "blade" );
		set("unit", "把");
		set("value", 3500);
		set("rigidity", 25);
		set("long", "一把鋼鑄單刀﹐是武林中人常用的武器。\n");
	}
	setup();
}

void varify_template(object owner)
{
	int i = owner->query_level();
	
	ENHANCE_D->enhance_weapon(this_object(), i*i + 10, "刀");
}
