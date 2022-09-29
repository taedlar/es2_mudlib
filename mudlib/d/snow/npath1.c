// Room: /d/snow/npath1.c

inherit ROOM;

void create()
{
	set("short", "小路");
	set("long", @LONG
這裡是一條黃土小路﹐往西可以回到雪亭鎮的主要街道﹐北邊是一
片草地﹐草地盡頭則是一片樹林﹐往東你可以聽到淙淙的水聲﹐不遠處
可以望見一條山溪。
LONG
	);
	set("outdoors", "snow");
	set("exits", ([ /* sizeof() == 2 */
		"east" : __DIR__"npath2",
		"west" : __DIR__"nstreet2",
]));

	setup();
	replace_program(ROOM);
}
