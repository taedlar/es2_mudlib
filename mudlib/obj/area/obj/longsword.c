// longsword.c

#include <weapon.h>

inherit F_SWORD;

void create()
{
	set_name("長劍", ({ "long sword", "sword" }) );
	set_weight(7500);
	setup_sword(2, 10, 50, 1);

	if( !clonep() ) {
		set("wield_as", "sword" );
		set("unit", "把");
		set("value", 4000);
		set("rigidity", 25);
		set("long", "一把約三尺長的長劍﹐是武林中人常用的武器。\n");
		set("wield_msg", "$N「唰」地一聲抽出一把長劍握在手中。\n");
	}
	setup();
}

void varify_template(object owner)
{
	int i = owner->query_level();
	
	ENHANCE_D->enhance_weapon(this_object(), i*i + 10, "劍");
}
