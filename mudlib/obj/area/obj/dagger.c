// vim: set ts=4 sw=4 syntax=lpc

#include <weapon.h>

inherit F_DAGGER;

void create()
{
    set_name("匕首", ({ "dagger" }));
    set_weight(3000);
    setup_dagger(1, 16, 40, 0);

    if( !clonep() ) {
	set("wield_as", ({ "dagger", "secondhand dagger" }));
	set("unit", "把");
	set("value", 500);
	set("rigidity", 25);
	set("long", "一把鋒利的短匕首﹐握在手裡重量剛剛好。\n");
	set("wield_msg", "$N抽出一把匕首握在手中。\n");
    }
    setup();
}

