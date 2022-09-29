// broadsword.c

#include <weapon.h>

inherit F_SWORD;

void create()
{
	set_name("闊劍", ({ "broadsword", "sword" }) );
	set_weight(10000);
	setup_sword(2, 12, 70, 1);

	if( !clonep() ) {
		set("wield_as", ({ "sword", "twohanded sword"}) );
		set("unit", "把");
		set("value", 5500);
		set("rigidity", 25);
		set("long", "一把約三尺長的闊劍﹐份量不輕，需要不小的膂力才能揮舞這種武器。\n");
		set("wield_msg", "$N「唰」地一聲抽出一把又長又重的闊劍握在手中。\n");
	}
	setup();
}

void varify_template(object owner)
{
	int i = owner->query_level();
	
	ENHANCE_D->enhance_weapon(this_object(), i*i + 10, "劍");
}
