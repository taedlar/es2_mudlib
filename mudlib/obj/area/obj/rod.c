/*  rod.c

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

inherit F_BLUNT;

void create()
{
    set_name("鐵鞭", ({ "rod", "rod" }) );
    set_weight(12000);
    setup_blunt(3, 8, 90, 2);

    if( !clonep() ) {
        set("wield_as", ({ "blunt" }));
        set("unit", "把");
        set("value", 5200);
        set("rigidity", 25);
        set("long", "一把沉重的鐵鞭，武林中常見的兵器。\n");
    }
    setup();
}

void varify_template(object owner)
{
    int i = owner->query_level();
    
    ENHANCE_D->enhance_weapon(this_object(), i*i + 10, "鞭");
}
