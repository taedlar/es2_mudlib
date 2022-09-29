/*  dagger.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

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
