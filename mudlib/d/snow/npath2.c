// Room: /d/snow/npath2.c

inherit ROOM;

void create()
{
	set("short", "溪邊小路");
	set("long", @LONG
這裡是一條溪邊的小路﹐從這裡可以望見北邊的山間一道瀑布從崖
上垂下﹐穿過山坡上的一片樹林﹐以及不遠處的草地﹐往你的東南邊流
過﹐溪水衝激著溪床的岩石﹐發出轟轟的響聲﹐往西沿著小路可以回到
雪亭鎮的大街﹐小路向東南可以通往溪邊。
LONG
	);
	set("exits", ([ /* sizeof() == 2 */
		"southeast" : __DIR__"npath3",
		"west" : __DIR__"npath1",
		"north" : "/d/month/a1",
	]));
	set("objects", ([
		__DIR__"npc/woman": 1
	]) );
	set("outdoors", "snow");

	setup();
	replace_program(ROOM);
}
