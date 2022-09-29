// Room: /d/snow/ruin1.c

#include <localtime.h>

inherit ROOM;

int flag = 0;

void create()
{
	set("short", "破舊大宅");
	set("long", @LONG
這裡是一間破舊大宅的前院﹐大廳已經崩塌了一半﹐焦黑的樑柱擋
住了你的去路﹐庭院裡雜草叢生﹐看來已經很久沒有人住了﹐據說這裡
晚上有鬼魂出現﹐但是倒也從沒聽說過有人被害﹐因此附近的居民仍然
照常生活﹐往西可以經由大宅的正門回到街上。
LONG
	);
	set("detail", ([ /* sizeof() == 2 */
		"雜草" : "這裡的雜草已經足足有及腰的長度了﹐你忽然發現南邊的草叢有什
麼東西發出金屬的閃光﹐不過也有可能是你的錯覺。
",
		"樑柱" : "從樑柱焦黑的痕跡來看﹐這座大宅的主人大概是因為火災的關係才
離開的吧。
",
]));
	set("outdoors", "snow");
	set("exits", ([ /* sizeof() == 2 */
		"west" : __DIR__"lane1",
		"east" : __DIR__"egate",
]));

	setup();
}

void init()
{
	add_action("do_climb", "climb");
	add_action("do_search", "search");
}

int do_climb(string arg)
{
	if( arg != "樑柱" ) return 0;

	message_vision("$N小心翼翼地爬過坍塌的樑柱進入大廳。\n", this_player());
	this_player()->move(__DIR__"ruin2");
	message("vision", this_player()->name() + "從坍塌樑柱的另一頭爬了過來。\n",
		environment(this_player()), this_player());
	return 1;
}

int do_search(string arg)
{
	mixed t;
	object ob;

	if( arg != "雜草" ) return 0;

	t = NATURE_D->game_time(1);
	// Can only be found during 11:00 to 14:00
	if( flag || (t[LT_HOUR] < 10) || (t[LT_HOUR] > 13) ) {
		message_vision("$N在雜草叢裡找來找去﹐但是什麼也沒有找到。\n", this_player());
		return 1;
	}
	flag = 1;
	ob = new(__DIR__"npc/obj/hairpin");
	ob->move(this_player());
	message_vision("$N在草叢裡找來找去﹐結果發現了一支髮簪﹗\n", this_player());
	return 1;
}

void reset()
{
	flag = 0;
	::reset();
}
