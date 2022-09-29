// Room: /d/snow/ebridge.c

inherit ROOM;

void create()
{
	set("short", "木橋");
	set("long", @LONG
你現在來到一條橫過山溪的橋上﹐山溪雖不甚寬﹐但是深度卻有三
、四丈﹐因此這條橋對雪亭鎮對外的交通格外重要﹐橋下的溪水正發出
轟隆轟隆的巨響流過溪谷﹐往西是雪亭鎮﹐往東則通往著名的雪吟山莊
﹐威鎮武林的武林盟主於蘭兼毅現下正駐蹕在莊上。
LONG
	);
	set("no_clean_up", 0);
	set("objects", ([ /* sizeof() == 1 */
		__DIR__"npc/guard" : 2,
]));
	set("outdoors", "snow");
	set("exits", ([ /* sizeof() == 2 */
		"west" : __DIR__"npath3",
		"east" : "/d/snowkeep/entrance",
]));

	setup();
	replace_program(ROOM);
}
