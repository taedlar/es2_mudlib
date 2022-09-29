//yu.c
#include <npc.h>
#include <ansi.h>

inherit F_VILLAGER;
inherit F_VENDOR;

void create()
{
	set_name("魚天", ({ "yu tan","yu","tan" }) );
	set_race("human");
	set_level(1);
	set("gender", "male");
	set("age", 25);
	set("long",
"濃眉扇耳聲似洪, 自幼獨孤魚家童, 不識父母識鐵山, 不愛刀槍愛錢逢。\n"
"魚天跟魚家淵源甚深, 據傳聞為魚鐵山當年誤殺一俠士時, 身旁之小男嬰\n"
", 由於不知其性氏家源, 故從小在魚家長大且名為天. 魚家對其有愧\, 而\n"
"魚天又不好武藝, 便以魚家資源, 助其在此開設錢莊兼販雜貨。\n"
	);
	set("merchandise", ([
		"/d/snow/npc/obj/blue_jacket": 5,
		"/obj/armor/fur_hat": 5,
		"/obj/bottle": 5,
	]));
	set("chat_chance", 5);
	set("chat_msg", ({
		"魚天喃喃唸道: 嗯, 上個月的利息收了二百八十兩白銀, 不錯 不錯 !\n",
		"魚天靠在左側的大櫃臺上, 低頭喃喃自語, 似乎正在算計著.\n",
	}));
	setup();
	carry_object(__DIR__"obj/blue_jacket")->wear();
}

int accept_fight()
{
	do_chat((: command("emote 道: 唉! 不練些防身技能, 乾爹那兒可是遠水救不了近火!") :));
	return 1;
}

void init()
{
	::init();
	if( !userp(this_player()) )	return;
	add_action("do_vendor_list", "list");

	do_chat(({
		"魚天笑開了大嘴道: 這位" + this_player()->rank("respectful") + "添些衣服穿吧!\n",
		"魚天向你介紹道: 要存息放款, 我魚記也是安全又實在喔!\n",
	}));
	if( this_player()->query("level")<=5 )
		// cancel call_out to test, by dragoon
		//call_out((:
		message("whisper",GRN
"魚天你的耳邊悄聲說道: 前幾天我撿到了個好東西喔, 給我點錢就送你, 夠意思吧!\n"NOR,
		//	this_player()):),5);
		this_player());
}

int accept_object(object me,object ob)
{
	int tmp;
	object tmp_ob;
	if( !ob->query("money_id") && !userp(ob) ){
		do_chat(({
"魚天說道: 這.. 雖然不是銀兩, 不過也說不定有人想買, 真是謝謝你啦.\n",
"魚天轉身將"+ob->name()+"收入了櫃台之後.\n",
		}));
		return 1;					// lv <=5, 1 coin is enough
	}								// lv >5, cost silver or gold
	if( ob->query("money_id")!="coin")
		tmp=1;
	if( me->query("level")>5 && ob->query("money_id")=="coin" )
		tmp=2;
	if( me->query("level")<=5 )
		tmp=3;
	switch(tmp){
		case 3:
		case 1:
			do_chat(({
"魚天說道: 好, 這位客官夠意思, 這玩意兒就送給你啦!\n",
"魚天將一張皮革交給了你.\n",
			}));
			tmp_ob=new(__DIR__"obj/note.c");
			if( !tmp_ob->move(me) )	ob->move(environment(me));
			return 1;;
		case 2:
			do_chat("這位客官也知道我的新收藏品? 看來這玩意挺值幾個錢的, 好, 出價銀兩就成!\n");
			return 1;;
		default:	return 1;
	}
}
