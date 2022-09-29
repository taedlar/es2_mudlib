// ghostbaby.c

inherit ITEM;

void create()
{
        set_name("鬼娃娃", ({ "ghostbaby" }));
        set_weight(1);
        if( !clonep() ) {
                set("unit", "個");
                set("long",
                        "這是一個鬼娃娃, 可以代替你已死。\n");
        set("no_sell", 1);
//	set("no_drop",1);
        set("value", 1000000000);
        }
        set("keep_this", 1);
        setup();
}

int query_autoload() { return 1; }
