// gammer.c

#include <npc.h>

inherit F_VILLAGER;

void create()
{
	set_name("瞎眼老太婆", ({ "blind gammer", "gammer", "__ID_GAMMER__" }) );
	set_attr("dex", 29);
	set_attr("con", 21);
	set_attr("cor", 22);
	set_attr("str", 19);
	set_race("human");
	set_class("commoner");
	set_level(44);

	set_skill("staff", 40);
	set_skill("stealing", 70);
	set_skill("dodge", 90);
	set_skill("blade", 30);
	set_skill("sword", 30);
	set_skill("unarmed", 50);
	set_skill("dagger", 70);
	set_skill("secondhand dagger", 70);
	set_skill("killerhood", 60);
	set_skill("force", 90);
	set_skill("regular force", 100);
    map_skill("force", "regular force");

	advance_stat("gin", 270);
	advance_stat("kee", 180);
	advance_stat("sen", 90);

	set("gender", "female");
	set("age", 66);
	set("long",
		"一個瞎了眼睛的老太婆﹐滿臉皺紋﹐但是卻給人一種親切的感覺。\n");

	set("chat_chance", 8);
	set("chat_msg", ({
		"瞎眼老太婆喃喃地說道﹕阿寶﹖是你嗎﹖\n",
		"瞎眼老太婆側著耳朵似乎在傾聽什麼。\n",
		(: random_move :)
	}) );

	set("chat_chance_combat", 10);
	set("chat_msg_combat", ({
		"瞎眼老太婆叫道﹕阿寶﹗有惡人在欺負婆婆哪﹗快出來啊﹗\n",
		(: random_move :),
	}) );
	set_stat_notify("kee", 40);

	setup();
	carry_object("/obj/area/obj/cloth")->wear();
	carry_object(__DIR__"obj/cane")->wield();
	carry_object(__DIR__"obj/syndicator")->wield("secondhand dagger");
}

private void give_coin(object who)
{
	object ob;

	if( environment(who) != environment() ) return;
	ob = new(__DIR__"obj/coin");
	if( !ob->move(who) )
		ob->move(environment());
}

void relay_say(object ob, string arg)
{
	int tmp = ob->query_temp("try/fon");
	// communicate with arbao
	if( !userp(ob) ){
		if( ob->id("__ID_ARBAO__") ){
			this_object()->delete("chat_msg");	// stop chat_msg
			this_object()->delete("chat_chance");
			if( strsrch(arg,"那太好了")>= 0 ) {
				do_chat((:command(
"say 胳臂外彎啊阿寶, 妳是喜歡上柳淳風那小子了吧."):));
				return;
			}
			if( strsrch(arg,"是")>= 0 ){
				do_chat(({
(:command("say 他想娶妳, 可以, 但條件是他父子要先幫你爺爺報了仇.
這樣他家與咱們的恩怨才算一筆勾消."):),
				}));
				call_out((:message_vision("瞎眼老太婆說完, 轉過身走了.\n",this_player()):),4);
				call_out((:destruct(this_object()):),6);
				return;
			}
		}
	}
	if( !tmp || tmp==0 ){
		if( arg=="祖奶奶" && ob->query_temp("thief_qualified") ) {
			command("giggle");
			command("pat " + ob->query("id"));
			if( (string)ob->query_class() != "commoner" ) return;
			ob->set_class("thief");
			tell_object(ob, "你正式成為一名盜賊了。\n");
			ob->improve_skill("sneak", 20);
			ob->improve_skill("stealing", 20);
			do_chat(({
				"瞎眼老太婆說道：對了，這個給你。",
				(: give_coin, ob :),
				"瞎眼老太婆說道：如果你想學本事，拿這東西給對方看。\n",
			}));
			return;
		}
		if( strsrch(arg, "阿寶") >= 0 )
			do_chat((: command, "say 阿寶﹖你說阿寶在哪裡﹖" :));
		else switch(random(10)) {
			case 0: do_chat((: command, "shake" :));
			case 1: do_chat((: command, "?" :));
			case 2: do_chat((: command, "steal coin from " + ob->query("id") :));
		}
		return;
	}
	switch(tmp) {
		case 8:
			if( strsrch(arg,"柳")>=0 ) {
				do_chat(({
"瞎眼老太婆怒不可遏, 喝道: 滾, 滾, 滾, 我不想聽到這個姓.\n",
"瞎眼老太婆喃喃自語著離開了, 你隱隱約約聽到甚麼死鬼, 阿寶, 甚麼仇的.\n",
"你想到也許\老太婆的孫女兒知道死鬼的意思.\n",
				}));
				ob->set_temp("try/fon",10);
			}
			break;
		case 16:
			if( strsrch(arg,"趙丰")>=0 ) {
				do_chat(({
"瞎眼老太婆說道: 我那死鬼早在三年多前被柳東蘆給害死了, 你問他做啥?\n",
"瞎眼老太婆又喋喋不休的說道: 他要不是被柳東蘆傷了, 憑他天下第一的輕功\絕不會
被人殺死, 這不是柳東蘆那廝害的嗎.\n",
"問了半天還是不清不楚, 你不禁疑惑, 不知柳東蘆跟這件事有何瓜葛.\n"
				}));
				ob->set_temp("try/fon",18);
			}
			break;
		case 45:
			if( strsrch(arg,"小豬豬")>=0 ) {
				do_chat(({
"瞎眼老太婆驚道: 這..這.. 好久沒有人這樣叫我了.. 看來這真的是老頭子寫的了.\n",
"你詳細的說明趙丰的死因.\n", 
"瞎眼老太婆沉吟了一下.\n",
(:command("say 小子, 幫老婆子一個忙, 去告訴姓柳的, 老婆子不會再去找他尋仇
了, 但緝兇之事, 他可別想脫得了責任."):),
				}));
				ob->set_temp("try/fon",48);
			}
			break;
		default:        break;
	}
}

int accept_fight()
{
	do_chat((: command, "?" :));
	return notify_fail("瞎眼老太婆對你視若無睹 ...。\n");
}

void init()
{
    if( environment(this_object())->query("connect-p") ) {
	command("say 口渴了, 來喝口水...\n");
        message_vision("$N緩緩走回大水缸邊。\n", this_object());
        this_object()->move("/d/snow/square");
        return;
    }
	// Dont call original init() to check autofight.
	if( !is_chatting() && this_player()->query_temp("try/fon")==43 ){
		do_chat(({
			"你見到了老太婆, 提出破布一事.\n",
			"老太婆喝道: 甚麼破布, 拿來.\n",
		}));
	}
}

private void ask_join(object player)
{
	if( !player || environment(player)!=environment() ) return;

	if( player->query_class() != "commoner" )
		command("say 唉 ... 人老了﹐連隨身的梳子都常常丟掉。\n");
	else {
		command("whisper " + player->query("id") + 
			" 好傢伙﹐竟然給你偷去了﹖想成為小偷是不是﹖叫聲「祖奶奶」我就讓你入這行 ...");
		player->set_temp("thief_qualified", 1);
	}
}

int accept_object(object player, object ob)
{
	if( player->query_temp("try/fon")!=43 ){
		if( !ob->id("annatto comb") ) return 0;
		command("say 咦﹖這 ... 這是 ... 我的梳子﹗");
		do_chat((: ask_join, player :));
		return 1;
	}
	if( ob->id("__ID_CHALLIE__") )	{
		do_chat(({
			"瞎眼老太婆怒道: 你這是欺我眼瞎看不見, 想來騙我嗎?\n",
			"你想到老太婆的暱稱, 也許\可以使她相信那件破布.\n"
		}));
		player->set_temp("try/fon", 45);
		return 1;
	}
}
