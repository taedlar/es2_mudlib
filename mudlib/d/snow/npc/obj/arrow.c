inherit ITEM;
void create()
{
        set_name("洛鐵箭", ({ "iron arrow", "arrow" }));
        set_weight(300);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "根");
                set("value", 4000);
                set("long", "一根洛鐵製成的箭, 手工精細.\n");
        }
        setup();
}
