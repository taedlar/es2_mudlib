// water.c

inherit LIQUID_ITEM;
inherit F_DRINK;

void create()
{
	set_name("清水", ({ "water" }));
	if( !clonep() ) {
		set("long", "... 水就是水﹐沒啥好看的。\n");
		set("density", 1);
	}
	setup();
	set_volume(1000);
}
