// Room: /d/snow/sstreet1.c

inherit ROOM;

void create()
{
	set("short", "街道");
	set("long", @LONG
這裡是雪亭鎮南邊連接鎮外的街道﹐西邊有一間貨棧﹐東邊則是一
家雜貨鋪﹐街道往北通往廣場南邊﹐往南穿過一個草棚出鎮外。
LONG
	);
	set("outdoors", "snow");
	set("exits", ([ /* sizeof() == 3 */
		"south" : __DIR__"sgate",
		"north" : __DIR__"square_s",
		"west" : __DIR__"store",
		"east" : __DIR__"bazar",
]));

	setup();
	replace_program(ROOM);
}
