// Room: /d/snow/tree.c

inherit ROOM;

void create()
{
	set("short", "榕樹上");
	set("long", @LONG
你現在正在一株巨大的榕樹上﹐茂密的枝葉搔得你有些發癢﹐前面
一根粗大的樹枝上有人放著一塊墊子。
LONG
	);
	set("exits", ([ /* sizeof() == 1 */
		"down" : __DIR__"square",
]));
	set("objects", ([
		__DIR__"npc/child": 1
	]) );
	setup();
	replace_program(ROOM);
}
