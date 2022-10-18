// Room: /d/snow/square_e.c

inherit ROOM;

void create()
{
	set("short", "廣場");
	set("long", @LONG
這裡是雪亭鎮廣場東邊﹐往東是一條僻靜的巷子﹐西邊有一株高大
的榕樹﹐南邊是一家打鐵鋪子﹐不過店門不在這一邊。
LONG
	);
	set("exits", ([ /* sizeof() == 2 */
		"east" : __DIR__"epath",
		"west" : __DIR__"square",
]));
	set("outdoors", "snow");

	setup();
	replace_program(ROOM);
}
