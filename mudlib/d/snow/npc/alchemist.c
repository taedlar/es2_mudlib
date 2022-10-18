// alchemist.c

#include <npc.h>

inherit F_VILLAGER;

void create()
{
	set_name("陳維俠", ({ "chen weisha", "weisha", "chen" }));
	set("nickname", "牙山居士");
	set_attr("dex", 18);
	set_attr("int", 24);
	set_attr("wis", 25);
	set_attr("spi", 22);
	set_attr("cps", 19);
	set_attr("con", 20);
	set_race("human");
	set_level(30);
	set_class("commoner");
	set_skill("unarmed", 60);
	set_skill("sword", 60);
	set_skill("parry", 80);
	set_skill("dodge", 70);
	set_skill("force", 70);
	set_skill("kee anatomy", 100);
	set_skill("dragon force", 60);
	set_skill("alchemy-medication", 140);
	set_skill("alchemy-wealth", 40);
	set_skill("alchemy-magic", 80);
	set_skill("alchemy-immortality", 30);
	map_skill("force", "dragon force");

	advance_stat("gin", 180);
	advance_stat("kee", 180);
	advance_stat("sen", 180);

	set("age", 55);
	set("long",
		"陳維俠是個著名的方士﹐經常來往於天邪國與喬陰縣之間採藥及尋覓\n"
		"珍奇礦物﹐他和雪亭鎮的王員外是舊日同窗﹐這幾日原本要往野羊山\n"
		"採藥﹐但是因為盜賊肆虐所以暫時在王員外家盤桓數日。\n"
		"陳維俠本名陳度﹐維俠是他的學生們送給他的稱號﹐意思是說他相當\n"
		"有俠氣﹐喜歡打抱不平﹐若不是給王員外的獨子纏住了講養生之術﹐\n"
		"只怕早幾日前就上野羊山剷除盜匪去了。\n");
	setup();
	carry_object(__DIR__"obj/silversword")->wield();
	carry_object("/obj/area/obj/cloth")->wear();
}

void set_flag(object me)
{
	if( me && present(me, environment()) ) {
		if( me->query_class() != "commoner"
		||	me->query("title") )
			return;
		me->set_temp("pending/alchemist", 1);
	}
	else
		do_chat("陳維俠說道﹕嘿 ... 人都走了﹐我這老頭還在自言自語 ...。\n");
}

int accept_apprentice(object me)
{
	if( is_chatting() ) return 0;
	if( me->query_temp("pending/alchemist") ) return 1;
	if( me->query("title") ) {
		do_chat("陳維俠道: 年輕人, 有空多多燒火採藥, 別這般四處閒耍。\n");
		return 0;
	}
	do_chat(({
		"陳維俠說道﹕拜我為師﹖呵呵 ... 我可不是什麼武林高手呀。\n",
		"陳維俠望著屋外出了會神﹐說道﹕我只是個喜歡煉丹採藥、打抱不平的老頭兒。\n",
		"陳維俠忽然笑了起來﹐說道﹕「煉丹採藥、打抱不平」... 哈哈哈...。\n",
		"陳維俠說道﹕我看你這年輕人倒還順眼﹐如果你不怕做些燒火採藥的粗活 ...\n",
		"陳維俠說道﹕老夫門徒不下千人﹐便多你一個也不妨。\n",
		(: set_flag, me :)
	}));
	return 0;
}

int init_apprentice(object me)
{
	// add by dragoon
	if( me->query_class() != "commoner" ) {
		do_chat(({
			"陳維俠說道﹕你既已入別派﹐豈有餘力再習我丹術?\n",
			"陳維俠揮揮手﹐說道﹕去吧.....。\n"
		}));
		return 1;
	}
	if( ::init_apprentice(me) ) {
                seteuid(getuid());
		me->set_class("alchemist");
		me->set("title", "龍圖丹派弟子");
		do_chat(({
			"陳維俠點了點頭﹐說道﹕你雖然入我門下﹐但是得先燒火採藥十年。\n",
			"陳維俠說道﹕十年之後﹐如果為師看你還堪造就﹐這才傳你丹術。\n",
			"陳維俠擺\了擺\手﹐說道﹕好了﹐你先下去吧。\n"
		}));
	}
}

int acquire_skill(object ob, string skill)
{
	if( is_chatting() )
		return notify_fail("你最好等他老人家的話說完再插嘴 ...。\n");
	if( !ob->is_apprentice_of(this_object()) ) return 0;

	switch(skill) {
	case "alchemy-medication":
		if( !ob->query_learn(skill)
		&&	ob->query_score("alchemy") >= 1000 ) {
			say("陳維俠點了點頭，說道：為師這就傳你，聽好了。\n");
			message_vision("$N將龍圖丹道中的神農之術大要傳授給$n。\n", this_object(), ob);
			ob->improve_skill(skill, random(ob->query_attr("wis")) + 1);
		}
		break;
	case "alchemy-wealth":
	case "alchemy-magic":
	case "alchemy-immortality":
	default:
		return 0;
	}

	do_chat(({
		"陳維俠說道：凡我方士中人，無不已濟世救人為己任。\n",
		"陳維俠說道：丹道入門雖易，但是往後的修練才剛開始 ...\n",
		"陳維俠說道：你要切記不可淪入江湖行騙的郎中之流，有幾分本領才說幾分話。\n",
		"陳維俠說道：希望你好自為之。\n",
	}));

	return 1;
}
