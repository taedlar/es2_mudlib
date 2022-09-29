// fur_hat.c

#include <armor.h>

inherit F_HEAD_EQ;

void create()
{
	set_name("毛皮帽", ({"fur hat", "hat"}));
	set_weight(1000);
	setup_head_eq();

	if( !clonep() ) {
		set("unit", "頂");
		set("long", "一頂不知是鹿皮還是麈皮縫製的小帽。\n");
		set("value", 500);
		set("wear_as", "head_eq");
		set("apply_armor/head_eq", ([
			"armor" : 1,
		]) );
	}
	setup();	
}