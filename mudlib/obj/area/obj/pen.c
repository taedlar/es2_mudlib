
// pen.c 判官筆
#include <weapon.h>

inherit F_DAGGER;

void create()
{
	set_name("判官筆", ({ "pen" }));
	set_weight(3000);
	setup_dagger(3,8,80,3);

	if( !clonep() ) {
		set("wield_as", ({ "dagger", "secondhand dagger" }));
		set("unit", "隻");
		set("value", 3000);
		set("rigidity", 30);
		set("long", 
		"一把用精剛鑄造的判官筆，筆身成黑色。\n"
		);
		set("wield_msg",
		"$N拿出一隻判官筆握在手上。\n"
		);
	}
	setup();
}
