// Room: /d/snow/square_n.c

inherit ROOM;

void create()
{
	set("short", "廣場");
	set("long", @LONG
這裡是雪亭鎮廣場北邊﹐平常是附近孩童嬉戲遊玩的地方﹐初一十
五則是市集的所在﹐往南可以看見一株大榕樹﹐往北則是一條筆直的街
道。
LONG
	);
	set("exits", ([ /* sizeof() == 2 */
		"north" : __DIR__"nstreet1",
		"south" : __DIR__"square",
	]));
	set("objects", ([
		__DIR__"npc/child1" : 1,
		__DIR__"npc/child2" : 1
	]) );
	set("outdoors", "snow");

	setup();
	replace_program(ROOM);
}
