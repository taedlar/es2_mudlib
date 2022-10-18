// Room: /d/snow/nstreet2.c

inherit ROOM;

void create()
{
	set("short", "街道");
	set("long", @LONG
這裡是雪亭鎮北邊的主要街道﹐往南稍遠處便是一處廣場﹐往北可
見一條蜿蜒的山路穿過長滿芒草的山坡通往野羊山上﹐東邊是一條小路
通往溪邊﹐西邊則是一間驛站。
LONG
	);
	set("no_clean_up", 0);
	set("objects", ([ /* sizeof() == 1 */
		"/obj/area/man" : 1,
]));
	set("exits", ([ /* sizeof() == 4 */
		"west" : __DIR__"post_office",
		"east" : __DIR__"npath1",
		"south" : __DIR__"nstreet1",
		"north" : __DIR__"ngate",
]));
	set("outdoors", "snow");

	setup();
	replace_program(ROOM);
}
