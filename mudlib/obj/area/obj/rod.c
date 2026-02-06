

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

