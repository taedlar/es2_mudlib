// Room: /d/snow/school.c

inherit ROOM;

void create()
{
	set("short", "私塾");
	set("long", @LONG
這裡是一間私塾﹐簡單的陳設與幾張排得整整齊齊的桌椅﹐就是這
間屋子的所有傢具﹐屋角一個大書櫃﹐裡面放著許多手抄本的古書﹐不
過書櫃的門是鎖著的﹐最前面的桌上放著文房四寶﹐與一盆小巧的盆栽
柏樹﹐私塾的出口在北邊。
LONG
	);
	set("exits", ([ /* sizeof() == 1 */
		"north" : __DIR__"wstreet1",
	]));
	set("objects", ([
		__DIR__"npc/teacher": 1,
		__DIR__"npc/alchemist": 1,
		__DIR__"npc/child3.c": 2,
	]));
	set("no_fight", 1);

	setup();
	replace_program(ROOM);
}
