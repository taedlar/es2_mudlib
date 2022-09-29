// kris.c

#include <weapon.h>

inherit F_BLADE;

void create()
{
	set_name("短刀", ({ "kris" }) );
	set_weight(4000);
	setup_blade(1, 17, 40, 1);

	if( !clonep() ) {
		set("wield_as", ({ "blade", "secondhand blade" }));
		set("unit", "把");
		set("value",600);
		set("long", "這是一種適合近身肉搏戰的短刀﹐刃長只有七寸﹐刀柄向內彎曲。\n");
		set("rigidity", 22);
	}
	setup();
}
