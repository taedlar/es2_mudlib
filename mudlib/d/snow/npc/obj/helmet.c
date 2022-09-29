// helmet.c

#include <armor.h>

inherit F_HEAD_EQ;

void create()
{
	set_name("銅盔", ({ "brass helmet", "helmet"}));
	set_weight(4000);
	setup_head_eq();

	if( !clonep() ) {
		set("unit", "頂");
		set("long", "這是一頂銅製的頭盔﹐屬於軍隊中的低級武官頂戴。\n");
		set("value", 900);
		set("wear_as", "head_eq");
		set("apply_armor/head_eq", ([
			"armor" : 3,
			"dodge" : -3,
		]) );
	}
	setup();	
}