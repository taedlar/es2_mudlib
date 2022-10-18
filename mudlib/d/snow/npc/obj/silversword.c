// ssilversword.c

#include <weapon.h>

inherit F_SWORD;

void create()
{
	set_name("洗銀劍", ({ "silver sword", "sword" }) );
	set_weight(7000);
	setup_sword(2, 15, 40, 4);

	if( !clonep() ) {
		set("wield_as", ({ "sword" }));
		set("unit", "把");
		set("value", 19000);
		set("rigidity", 40);
		set("long", "一把銀製的短劍﹐鑄造得十分精美。\n");
		set("wield_msg", "$N抽出一把閃閃發亮的短劍當作武器。\n");
	}
	setup();
}
