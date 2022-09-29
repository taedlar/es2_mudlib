// basic setup of major_heal_pill 
#include <ansi.h>

inherit COMBINED_ITEM;

void create()
{
	set_name("大還丹", ({"major heal pill", "major", "pill" }));
	if( !clonep() ) {
		set("unit", "些");
		set("long", 
"大還丹是一種極神奇的仙丹, 具強大的療效可以讓服用者的\n"
"內外傷在瞬間癒合, 不過煉製大還丹所需的藥材除了小還丹\n"
"所需的幾樣藥材外, 還必需找尋雪蓮, 千年人蔘等數種罕見\n"
"的珍貴藥材。因此, 大還丹除了燒煉困難外, 藥材之價格亦\n"
"極昂貴。\n");
		set("base_unit", "粒");
		set("base_value", 10000);
		set("base_weight", 5);
		set("mixture_value", 6000);
		set("produce_param", ([
			"min_heat_power" : 460,
			"max_heat_power" : 500,
			"min_heat" : 9500,
			"max_heat" : 10500,
		]));
		set("requirement", ([
			"literate" : 45,
			"alchemy-medication" : 150,
		]));
	}
	set_amount(1);
	setup();
}
