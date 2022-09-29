// Room: /d/snow/epath.c

inherit ROOM;

void create()
{
	set("short", "僻靜小巷");
	set("long", @LONG
這裡是條僻靜的小巷子﹐往西不遠處通往雪亭鎮的廣場﹐小巷的北
面是一些瓦屋﹐圍繞著高高的圍牆﹐看來是有錢人的屋子﹐南邊有家小
藥鋪﹐門口掛了好些曬乾的藥草﹐小巷往東是個轉角。
LONG
	);
	set("detail/圍牆",
		"這間屋子的圍牆看起來相當高﹐不過如果你跳起來的話勉強可以搆得著。\n");
	set("outdoors", "snow");
	set("exits", ([ /* sizeof() == 2 */
		"east" : __DIR__"lane1",
		"west" : __DIR__"square_e",
		"south" : __DIR__"herb_shop",
	]));

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
	this_player()->move(__DIR__"kitchen");
	message("vision", this_player()->name() + "從圍牆的另一頭爬了過來。\n",
		environment(this_player()), this_player());
	return 1;
}
