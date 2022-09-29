// Room: /d/snow/lane2.c

inherit ROOM;

void create()
{
	set("short", "溪邊小路");
	set("long", @LONG
你現在來到一處巷子的入口﹐往北通往一條溪邊的小路﹐西邊是一
棟大瓦屋﹐大門深鎖著﹐東邊有一家磨坊﹐磨坊屋旁一個水車正轆轆地
轉著﹐往南通往巷子裡的一個轉角。
LONG
	);
	set("detail", ([ /* sizeof() == 3 */
		"大瓦屋" : "一間高大的瓦屋﹐在雪亭鎮這樣的山間小鎮而言﹐這樣的屋子並不
多見﹐可算得上是有錢人家。
",
		"水車" : "這架水車是磨坊動力的來源﹐平常溪水豐沛的季節就直接用水力﹐
一到秋冬枯\水季節﹐就得雇工人用腳踩。
",
		"磨坊" : "一家用水車輾麵粉的磨坊﹐磨坊的門是關著的﹐不過從裡面傳來的
聲音判斷﹐應該有工人正在裡面工作。
",
]));
	set("exits", ([ /* sizeof() == 2 */
		"south" : __DIR__"lane1",
		"north" : __DIR__"npath3",
]));
	set("outdoors", "snow");

	setup();
}

void init()
{
	add_action("do_enter", "enter");
}

int do_enter(string arg)
{
	if( !arg || arg!="磨坊") return 0;

	message_vision("$N推開磨坊的門﹐走了進去。\n", this_player());
	if( this_player()->move(__DIR__"mill") )
		message("vision", this_player()->name() + "推開門從外面走了進來。\n",
			environment(this_player()), this_player());
	return 1;
}


