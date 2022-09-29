// vagabond.c

#include <npc.h>

inherit F_BANDIT;

void create()
{
	set_name("市井無賴", ({ "vagabond" }));
	set_race("human");
	set_class("fighter");
	set_level(2);
	set_skill("unarmed", 5);
	set_skill("dodge", 3);
	set_skill("dagger", 7);

	set("age", 24);
	set("long", "一個滿臉兇相，惡行惡狀的無賴漢，最好別靠他太近。\n");
	set("chat_chance", 6);
	set("chat_msg", ({
		"無賴漢「嘿嘿」地怪笑著，一邊不懷好意地盯著你。\n",
		"無賴漢裝作若無其事的樣子挨近你身邊，然後出其不意地撞你一下。\n",
		"無賴漢用很噁心的方式摳了摳鼻屎，然後往你這邊一彈 ... 快躲！\n",
	}));

	setup();
	carry_object(__DIR__"obj/knife")->wield();
	carry_object(__DIR__"obj/dice");
}
