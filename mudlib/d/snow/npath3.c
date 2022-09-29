// Room: /d/snow/npath3.c

inherit ROOM;

void create()
{
	set("short", "溪邊小路");
	set("long", @LONG
你現在來到一條溪流邊上的小路﹐溪水流過佈滿石塊的河床發出轟
轟的聲音﹐一條木橋跨過溪流往東通往鎮外的小路﹐沿著溪邊往南不遠
處則可以望見一間磨坊。北邊一條小路可以下到溪谷中。
LONG
	);
	set("exits", ([ /* sizeof() == 4 */
		"north" : __DIR__"river1",
		"south" : __DIR__"lane2",
		"northwest" : __DIR__"npath2",
		"east" : __DIR__"ebridge",
]));
	set("outdoors", "snow");

	setup();
	replace_program(ROOM);
}
