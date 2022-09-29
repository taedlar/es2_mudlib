// shortsword.c

#include <weapon.h>

inherit F_SWORD;

void create()
{
	set_name("短劍", ({ "shortsword", "sword" }) );
	set_weight(3000);
	setup_sword(3, 6, 40, 0);

	if( clonep() )
		set_default_object(__FILE__);
	else {
		set("wield_as", ({ "sword", "secondhand sword" }));
		set("unit", "把");
		set("value", 900);
		set("rigidity", 20);
		set("long", "一把粗糙的短劍﹐一般旅人帶在身邊防身用的武器。\n");
		set("wield_msg", "$N抽出一把短劍當作武器。\n");
	}
	setup();
}
