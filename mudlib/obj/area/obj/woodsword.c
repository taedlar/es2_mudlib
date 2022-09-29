// woodsword.c : an example weapon

#include <weapon.h>

inherit F_SWORD;

void create()
{
	set_name("木劍", ({ "wooden sword", "sword" }) );
	set_weight(2000);
	setup_sword(1, 14, 40, 0);

	if( clonep() )
		set_default_object(__FILE__);
	else {
		set("wield_as", "sword");
		set("unit", "把");
		set("value", 400);
		set("rigidity", 10);
		set("long", "一把用硬木削成的長劍﹐被炭火燻得黑黑的。\n");
	}
	setup();
}
