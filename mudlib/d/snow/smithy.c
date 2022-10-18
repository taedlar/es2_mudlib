// Room: /d/snow/smithy.c

inherit ROOM;

void create()
{
	set("short", "打鐵鋪子");
	set("long", @LONG
這裡是一間打鐵鋪子﹐牆上掛了許多鐵器的半成品跟工具﹐鋪子中
間一個燒著熊熊火燄的火爐映得你在牆上的影子一晃一晃的﹐旁邊還有
一個沈重的大鐵砧﹐鋪子的出口就在東邊。
LONG
	);
	set("detail", ([ /* sizeof() == 2 */
		"鐵砧" : "這個大鐵砧看起來十分沈重﹐上面滿是敲打的痕跡。
",
		"鐵器" : "這些鐵器有鍋子、鏟子、鐵鎚、耕田用的鋤頭、犁等等。
",
]));
	set("no_clean_up", 0);
	set("objects", ([ /* sizeof() == 1 */
		__DIR__"npc/smith" : 1,
]));
	set("exits", ([ /* sizeof() == 1 */
		"west" : __DIR__"square_s",
]));

	setup();
	replace_program(ROOM);
}
