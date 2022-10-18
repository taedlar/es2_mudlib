// Room: /d/snow/square_s.c

inherit ROOM;

void create()
{
	set("short", "廣場");
	set("long", @LONG
這裡是雪亭鎮廣場的南邊﹐往北可以望見一株高大的榕樹﹐廣場以
榕樹為中心形成一個十多丈見方的大空地﹐街道往南通往鎮外﹐東邊不
遠處有間打鐵鋪子。
LONG
	);
	set("outdoors", "snow");
	set("exits", ([ /* sizeof() == 4 */
		"west" : __DIR__"square_sw",
		"east" : __DIR__"smithy",
		"north" : __DIR__"square",
		"south" : __DIR__"sstreet1",
]));

	setup();
	replace_program(ROOM);
}
