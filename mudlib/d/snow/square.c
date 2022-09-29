// Room: /d/snow/square.c

inherit ROOM;

void create()
{
	set("short", "廣場中央");
	set("long", @LONG
這裡是雪亭鎮廣場的中央﹐一株巨大的老榕樹盤根錯結地站在中央
﹐一些孩童常常爬上這株老榕嬉戲﹐榕樹下七橫八豎地放著幾張長凳供
人歇息聊天﹐樹旁還有一個水缸供路人取水解渴。
LONG
	);
	set("objects", ([ /* sizeof() == 1 */
		__DIR__"npc/obj/pot" : 1,
		__DIR__"npc/gammer" : 1,
		__DIR__"npc/junkman": 1,
	]));
	set("detail", ([ /* sizeof() == 2 */
		"榕樹" : "這株榕樹少說也有兩三百歲了﹐一條條長長的鬚根幾乎垂到地面﹐
樹幹因為經常被人撫摸而顯得光滑。
",
		"長凳" : "十分普通常見的長凳﹐如果你累了﹐不必客氣﹐盡管做下來休息。
",
]));
	set("exits", ([ /* sizeof() == 4 */
		"east" : __DIR__"square_e",
		"north" : __DIR__"square_n",
		"west" : __DIR__"square_w",
		"south" : __DIR__"square_s",
	]));

	setup();
}

void init()
{
	add_action("do_climb", "climb");
}

int do_climb(string arg)
{
	if( arg != "榕樹" ) return 0;

	message_vision("$N攀著榕樹的樹幹爬了上去。\n", this_player());
	if( this_player()->move(__DIR__"tree") )
		message("vision", this_player()->name() + "從樹下爬了上來。\n",
		environment(this_player()), this_player());
	return 1;
}

