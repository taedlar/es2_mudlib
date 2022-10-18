// pawn_stamp.c

inherit ITEM;

void create()
{
	set_name("當票", ({ "pawn stamp", "stamp" }));
	set_weight(5);
	if( !clonep() ) {
		set("unit", "張");
		set("long", "一張空白當票。\n");
	}
	setup();
}
