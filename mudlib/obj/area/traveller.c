//traveller.c

#include <npc.h>

inherit F_VILLAGER;

void create()
{
	set_name("過路客", ({ "traveller" }) );
	set("unit", "位");
	set("gender", "female");
	set_race("jiaojao");
	set_class("thief");
	set_attr("con",36);
	set_level(20);
	set("age", 19);
	set_skill("parry",50);
	set_skill("dodge",50);
	set_skill("blade",60);
	advance_stat("kee",100);
	set("long",
"一位行色匆匆, 滿臉風霜的過路客, 一身勁裝, 似乎隨時又要上路去.\n");
	set("chat_chance", 4);
	set("chat_msg", ({
"過路客喘了幾口氣道: 呼 呼 .. 好累, 真遠.\n",
"過路客似乎累的講不出話來了.\n",
		(: command,"sweat":),
	}));
	setup();
	carry_money("silver", 30);
	carry_object(__DIR__"obj/cloth")->wear();
	carry_object(__DIR__"obj/whip")->wield();
}

int accept_fight(object ob)
{
	do_chat("過路客擦了擦脖子上的汗水, 笑道: 打啥, 都快累翻了還打!\n");
	return 0;
}
