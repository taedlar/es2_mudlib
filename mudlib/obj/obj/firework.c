#include <ansi.h>

inherit ITEM;


void create()
{
	set_name("煙火", ({ "firework" }) );
	if (!clonep()) set("long", @CLONG
這是一個煙火, 點燃它(light_firework), 你將會獲得許多樂趣。
用法是：light_firework xxx
xxx 是你想要說的話囉 ....
CLONG
		);
                set("base_unit", "個");
                set("base_value", 1000);
                set("base_weight", 100);
		                set("unit", "個");
	setup();
} // create()


void init()
{
	add_action("do_light","light_firework");
} // init()


int do_light(string s)
{
	string s1, s2, cmsg;

	switch (random(7))
	{
	case 0  : s1= HIR; s2= "紅"; break;
	case 1  : s1= HIM; s2= "紫"; break;
	case 2  : s1= HIY; s2= "黃"; break;
	case 3  : s1= HIG; s2= "綠"; break;
	case 4  : s1= HIB; s2= "藍"; break;
	case 5  : s1= HIC; s2= "青"; break;
	case 6	: s1= HIW; s2= "白"; break;
	default : s1= HIR; s2= "紅";
	} // switch

	message_vision("$N將煙火點著, 然後掩住耳朵迅速地躲到一邊。\n",
		this_player() );
	if (!s) cmsg = s1 + "砰！的一聲，一枚美麗的"+ s2 +
		"色煙火在天空中迸射開來。\n" + NOR;
	else cmsg = s1 + "砰！的一聲，許\多枚美麗的"+ s2 +
		"色煙火在空中迸射開來，緩緩幻化成一排字：\n"+s+"。\n" + NOR;
	shout(cmsg);
	write(cmsg);
	destruct(this_object());
	return 1;
} // do_light()
