

#include <weapon.h>

inherit F_BLADE;

void create()
{
    set_name("木刀", ({ "wooden blade", "blade" }) );
    set_weight(2000);
    setup_blade(1, 14, 40, 0);

    if( !clonep() ) {
        set("wield_as", "blade");
        set("unit", "把");
        set("value", 400);
        set("rigidity", 10);
        set("long", "一把用硬木削成的單刀﹐被炭火燻得黑黑的。\n");
    }
    setup();
}

