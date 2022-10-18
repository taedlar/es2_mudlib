// Room: /d/snow/mill.c

inherit ROOM;

void create()
{
	set("short", "磨坊");
	set("long", @LONG
這裡是一間以磨麵粉為業的磨坊﹐屋子裡一大半的空間被一架水車
佔據﹐水車轉動橫過屋樑上的木軸﹐木軸推動幾個絞盤轉動屋子中央的
石磨﹐房子的另一角堆放著一些空的麻袋。
LONG
	);
	set("exits", ([ /* sizeof() == 1 */
		"out" : __DIR__"lane2",
	]));
	set("objects", ([
		__DIR__"npc/miller": 1
	]));

	setup();
	replace_program(ROOM);
}
