// oldman.c

#include <npc.h>

inherit F_BANDIT;

void create()
{
	set_name("黑衣老人", ({ "oldman" }) );
	set_race("human");
	set_class("commoner");
	set_level(7);
	set_stat_maximum("gin", 70);
	set_stat_maximum("sen", 50);
	set_skill("sword", 25);
	set_skill("parry", 15);
	set_skill("dodge", 20);
	set_skill("dagger", 40);
	set_skill("stealing", 35);
	set_stat_notify("gin", 40);
	set_stat_notify("kee", 40);

	set("age", 65);
	set("long",
		"一個身穿黑衣的老人﹐雖然年紀很大了﹐卻臉色紅潤﹐眼睛炯炯有神。\n");
	set("pursuer", 1);

	setup();
	carry_object("/obj/area/obj/cloth")->wear();
	carry_object("/obj/area/obj/shortsword")->wield();
}

int notify_stat(mapping flag)
{
	object ob;

	if( is_killing() ) {
		command("say ...");
		return ::notify_stat();
	}
	command("halt");
	do_chat("黑衣老人哈哈大笑﹐說道﹕年輕人﹐你的武藝不錯﹐小老兒服了你啦﹗\n");
	if( ob = query_temp("last_attacked_target") )
		command("steal something from " + ob->query("id"));
	return 1;
}
