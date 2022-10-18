// innkeeper.c

#include <localtime.h>
#include <npc.h>

inherit F_VILLAGER;

void open_inn();
void close_inn();

int on_business = 0;

void create()
{
	mapping gt;

	set_name("掌櫃", ({ "innkeeper", "keeper" }) );
	set_race("human");
	set_level(4);

	set("age", 52);
	set("long", "這位便是這家客棧的掌櫃﹐看起來福福泰泰像是個老好人。\n");
	set("schedule", ([
		700: (: open_inn :),
		2200: (: close_inn :),
	]));
	setup();
	carry_money("coin", 160);

	gt = NATURE_D->game_time(1);
	if( gt[LT_HOUR] < 7 || gt[LT_HOUR] > 22 ) on_business = 0;
	else on_business = 1;
}

void init()
{
	::init();
	if( !is_fighting() )
		do_chat( (: command, "say 歡迎﹗歡迎﹗請裡面坐﹗" :) );
}

void relay_emote(object from_ob, string verb)
{
	command("smile");
}

int accept_fight(object player)
{
	do_chat((: command, "say 這位" + player->rank("respectful")
	 + "真愛開玩笑﹐小老兒還要做生意呢。\n":));
	return 0;
}

void accept_kill(object player)
{
	object ob;

	// 已經在打架了，有人 kill 掌櫃，不做任何事。
	if( is_fighting() ) return;

	// 開始打架，先喊救命( 這裡用 command() 會先喊出來 )
	command("emote 大叫﹕殺人哪﹐救命哪﹗");

	// 看看現場有沒有官兵在？有的話 present() 會傳回房間中第一個官兵。
	ob = present("garrison");

	// 沒有的話，叫一個出來。
	if( !ob ) {
		ob = new(__DIR__"patrol");
		ob->move(environment());

		// 交代官兵的出現，注意前面掌櫃已經用 command() 喊出聲了，如果
		// 用 do_chat() 其訊息要到下一個 heart beat 才會出來。
		tell_room(environment(), "巡邏官兵聽到叫聲﹐跑了過來。\n");
	}

	// protect() 定義在官兵身上，也是大聲嚷嚷幾句場面話。
	ob->protect(this_object());

	// 這是讓官兵先下手為強。
	ob->kill_ob(player);
}

void open_inn()
{
	command("emote 打了個呵欠﹐說道﹕睡的真好﹐開店做生意吧。");
	environment()->open_door("east");
	environment()->open_door("west");
	environment()->set("no_fight", 1);
}

void close_inn()
{
	object ob;

	command("say 嗯.. 二更了﹐打烊吧﹐各位客倌明日請早。");
	command("emote 將你送出客棧。");
	environment()->close_door("east");
	environment()->close_door("west");
	environment()->set("no_fight", 0);
	foreach(ob in all_inventory(environment()) ) {
		if( ob==this_object()
		||	!ob->is_character()
		||	ob->id("snow_inn_waiter") ) continue;
		ob->move(environment()->query("exits/east"));
	}
}
