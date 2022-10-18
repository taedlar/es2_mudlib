// cane.c

#include <weapon.h>

inherit F_STAFF;

void create()
{
	set_name("拐杖", ({ "cane" }) );
	set_weight(2400);
	setup_staff(1, 19, 130, 0);

	if( !clonep() ) {
		set("wield_as", ({ "staff", "twohanded staff" }) );
		set("unit", "根");
		set("value", 600);
		set("rigidity", 12);
		set("long",
			"一根硬木拐杖﹐因為經年使用而顯得光滑。\n");
	}
	setup();
}
