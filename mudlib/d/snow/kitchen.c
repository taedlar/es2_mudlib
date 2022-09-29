// Room: /d/snow/kitchen.c

inherit ROOM;

void create()
{
	set("short", "廚房");
	set("long", @LONG
你現在正在一戶有錢人家的廚房裡﹐不過和你想像中的有錢人家廚
房好像不太一樣﹐並沒有堆滿了雞鴨魚肉跟山珍海味﹐想來有錢人也是
省吃儉用才能有錢的﹐廚房的門在你的南邊﹐不過鎖住了﹐東邊則是你
進來的圍牆。
LONG
	);

	set("objects", ([
		"/obj/food/dumpling" : 2
	]) );
	setup();
}

void init()
{
	add_action("do_climb", "climb");
}

int do_climb(string arg)
{
	if( arg!="圍牆" ) return 0;

	message_vision("$N用手攀上圍牆﹐蹬了幾下翻了過去。\n", this_player());
	this_player()->move(__DIR__"epath");
	message("vision", this_player()->name() + "從圍牆的另一頭爬了出來。\n",
		environment(this_player()), this_player());
	return 1;
}
