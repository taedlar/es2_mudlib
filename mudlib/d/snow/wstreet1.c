// Room: /d/snow/wstreet1.c

inherit ROOM;

void create()
{
	set("short", "街道");
	set("long", @LONG
這裡是雪亭鎮廣場西邊的街道﹐往北是一間客棧﹐不過這個方向沒
有入口﹐南邊一間青瓦屋﹐門口灑掃得乾乾淨淨﹐似是家私塾模樣﹐往
西的街道十分寬敞﹐路上還有車馬經過壓出來的溝痕。
LONG
	);
	set("exits", ([ /* sizeof() == 3 */
		"south" : __DIR__"school",
		"east" : __DIR__"square_sw",
		"west" : __DIR__"wstreet2",
]));
	set("outdoors", "snow");

	setup();
	replace_program(ROOM);
}
