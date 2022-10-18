// Room: /d/snow/inn_kitchen.c

inherit ROOM;

void create()
{
	set("short", "廚房");
	set("long", @LONG
這裡是一間寬敞的廚房﹐一座大灶佔據了東邊半個牆壁﹐南邊的櫃
子整整齊齊地放著許多鍋碗瓢盆﹐這裡平常是相當忙碌的﹐如果你要在
這裡東張西望的話﹐最好小心別被店小二端著的熱湯給燙到了﹐往東可
以回到客棧裡。
LONG
	);
	set("detail", ([ /* sizeof() == 2 */
		"櫃子" : "這些櫃子裡放的是給客人用的碗盤碟筷﹐分門別類裝在幾個大櫃子\n"
				 "裡﹐櫃子的門都拆了﹐想是方便取用。\n",
		"大灶" : "一座燃燒著熊熊炭火的大灶﹐灶口可以容納一個漢子爬進去﹐不過\n"
				 "白天炭火燒得正熱﹐當然不會有人幹這種傻事。\n"
	]));
	set("exits", ([ /* sizeof() == 1 */
		"east" : __DIR__"inn_hall",
	]));

	setup();
	create_door("east", "木板門", "west", 0);
}

void init()
{
	add_action("do_enter", ({"enter", "climb"}));
}
	
int do_enter(string arg)
{
	if( arg != "大灶" ) return 0;
	message_vision("$N一矮身, 從大灶的灶口鑽了進去.\n", this_player());
	if( this_player()->move(__DIR__"fireplace") )
		message("vision", this_player()->name() + "從外面鑽了進來, 跟你擠成一團.\n",
			environment(this_player()), this_player());
	return 1;
}	
	