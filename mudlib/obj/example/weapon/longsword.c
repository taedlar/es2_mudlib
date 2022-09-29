// longsword.c

#include <weapon.h>

inherit F_SWORD;

void create()
{
	set_name("長劍", ({ "longsword", "sword" }) );
	set_weight(5000);
	setup_sword(3, 7, 50, 0);

	if( !clonep() ) {
		set("wield_as", "sword" );
		set("unit", "把");
		set("value", 4000);
		set("rigidity", 25);
		set("long", "一把精鋼鑄成的長劍﹐是武林中人常用的武器。\n");
		set("wield_msg", "$N「唰」地一聲抽出一把長劍握在手中。\n");
	}
	setup();
}
