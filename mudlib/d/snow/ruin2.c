// Room: /d/snow/ruin2.c

inherit ROOM;

void create()
{
	set("short", "破舊大宅正廳");
	set("long", @LONG
這裡是一棟破舊大宅的正廳﹐北面的牆壁跟屋頂已經整個坍塌了﹐
涼颼颼的冷風從北邊的缺口直灌進來﹐另你覺得意外的是﹐這裡清掃得
相當整潔﹐地上用乾草整整齊齊地鋪著一個床舖﹐說是流浪漢在這裡棲
宿卻也不像﹐哪裡會有這麼愛整潔的流浪漢﹖大廳倒塌的樑柱擋住了往
西的出口。
LONG
	);
	set("detail", ([ /* sizeof() == 3 */
		"樑柱" : "一跟粗大的樑柱倒了下來﹐正好擋住了正廳的出口。
",
		"床舖" : "一個用乾草鋪成的床舖﹐雖然簡陋﹐但是卻乾乾淨淨﹐當你靠近的
時候竟還聞到一股淡淡的脂粉香氣。
",
		"缺口" : "從缺口往外望去﹐只見大宅的其他部份都已經成為瓦礫堆了﹐如果
你想過去﹐勉強可以從缺口爬出去。
",
]));

	set("exits", ([
		"north": __DIR__"riverbank"
	]));
	set("objects", ([
		__DIR__"npc/girl" : 1
	]) );
	setup();
}

void init()
{
	add_action("do_climb", "climb");
}

int do_climb(string arg)
{
	if( arg != "樑柱" ) return 0;

	message_vision("$N爬過倒塌的樑柱﹐往大宅的前院離去。\n", this_player());
	this_player()->move(__DIR__"ruin1");
	message("vision", this_player()->name() + "從倒塌樑柱的另一頭爬了出來。\n",
		environment(this_player()), this_player());
	return 1;
}

