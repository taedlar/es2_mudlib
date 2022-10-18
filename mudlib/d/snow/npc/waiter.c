// waiter.c

#include <npc.h>

inherit F_VILLAGER;
inherit F_VENDOR;

int on_service = 0;

void create()
{
	set_name("店小二", ({ "waiter", "snow_inn_waiter" }) );
	set_race("human");
	set_class("commoner");
	set_level(2);

	set("age", 21);
	set("long",
		"一個約二十出頭的年輕小夥計﹐正忙進忙出招呼客人﹐你可以用\n"
		"list 看可以點什麼菜﹐用 buy 跟店小二點菜。\n");
	set("chat_chance", 1);
	set("chat_msg", ({
		"店小二閒著無聊﹐哼起小曲兒來。\n",
		"店小二探頭出去望了望天色﹐說道﹕這幾天天氣不錯﹐真想出去走走。\n",
		"店小二說道﹕那說書的李先生不知道好點了沒﹐少了說書的這裡實在氣悶。\n",
		"店小二說道﹕客倌今天怎麼有空﹐最近可有新鮮事能說來聽聽嗎﹖\n",
	}));
	set("merchandise", ([
		"/obj/food/dumpling": 50,
		"/obj/food/manto": 50,
		"/obj/food/pork": 30,
		__DIR__"obj/roast_chicken": 30,
	]));

	setup();
}

void init()
{
	::init();
	add_action("do_vendor_list", "list");
}


int accept_fight(object ob)
{
	do_chat("店小二扮了個鬼臉﹐說道﹕客倌要打架的話﹐小的我可不是對手。\n");
	return 0;
}

mixed affirm_merchandise(object customer, string item_name)
{
	if( on_service )
		return notify_fail("店小二正忙著招呼其他客人﹐請你等一會兒。\n");
	return ::affirm_merchandise(customer, item_name);
}

private void handover(object customer, mixed item)
{
	on_service = 0;
	if( !customer || environment(customer)!=environment() ) {
		command("say 咦﹖剛剛那位客倌人呢﹖怎麼付了錢人又不見了 ...");
		say("店小二把端出來的" + item->name() + "又端了回去。\n");
		return;
	}
	command("say 客倌﹐您的" + item->name() + "來啦﹗");
	message_vision("$N把剛從廚房端出來的" + item->name()
		+ "送到$n面前。\n", this_object(), customer);
	::deliver_merchandise(customer, item);
}

void deliver_merchandise(object customer, mixed item)
{
	command("say " + item->name() + "是吧﹖馬上來﹗");
	say("店小二對著廚房喊道﹕" + item->name() + "一份﹗\n");
	on_service = 1;
	do_chat((: handover, customer, item :));
}
