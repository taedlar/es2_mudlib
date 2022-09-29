// child2.c

#include <npc.h>

inherit F_VILLAGER;

void create()
{
	set_name("小孩", ({ "child" }) );
	set_race("human");
	set_stat_notify("kee", 100);
	set_level(1);

	set("long", "一個約七、八歲大的男孩﹐笑瞇瞇地望著你。\n");
	set("chat_chance", 10);
	set("chat_msg", ({
		"小孩叫道﹕呀 ～ 呀 ～\n",
		"小孩咭咭咯咯地笑著﹐繞著你跑來跑去。\n",
		"小孩叫道﹕咿 ～ 咿 ～\n"
	}) );
	setup();
	carry_money("coin", 1);
	carry_object(__DIR__"obj/woodsword")->wield();
}

int accept_fight(object ob)
{
	do_chat("小孩興奮地跳著﹕好呀﹗來比武﹗叱﹗\n");
	return 1;
}

int notify_stat(mapping flag)
{
	command("emote 「哇」地一聲哭了起來﹕「你打我﹗你是壞人﹗」");
	return ::notify_stat(flag);
}

