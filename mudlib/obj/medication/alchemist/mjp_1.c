// basic setup of major_heal_pill 
#include <ansi.h>

inherit COMBINED_ITEM;

void create()
{
	set_name(HIY"大還丹"NOR, ({"major heal pill", "major", "pill" }));
	if( !clonep() ) {
		set("unit", "些");
		set("long", 
"大還丹是一種極神奇的仙丹, 具強大的療效可以讓服用者的\n"
"內外傷在瞬間癒合, 不過煉製大還丹所需的藥材除了小還丹\n"
"所需的幾樣藥材外, 還必需找尋雪蓮, 千年人蔘等數種罕見\n"
"的珍貴藥材。因此, 大還丹除了燒煉困難外, 藥材之價格亦\n"
"極昂貴。這顆丹外表呈淡淡的金黃色, 乃鍊製過程中火候十\n"
"足之表徵, 為百中選一之極品小還丹。\n");
		set("base_unit", "粒");
		set("base_value", 10000);
		set("base_weight", 5);
		set("mixture_value", 6000);
	}
	set_amount(1);
	setup();
}

int stuff_ob(object me)
{
	if (me->query_stat_maximum("food") - me->query_stat("food") < 10)
		return notify_fail("你的肚子已經撐得連這粒藥丸也吞不下去了。\n");
	message_vision("$N吞下一粒大還丹。\n", me);
	me->consume_stat("water", 18);
	me->supplement_stat("food", 7);
	me->heal_stat("gin", 60);
	me->supplement_stat("gin", 80);
	me->heal_stat("kee", 150);
	me->supplement_stat("kee", 180);
	me->heal_stat("HP", 25);
	me->supplement_stat("HP", 40);
	add_amount(-1);

	return 1;
}
