// Room: /d/snow/store.c

inherit ROOM;

void create()
{
	set("short", "貨棧");
	set("long", @LONG
這裡是一間貨棧﹐也是旅行客商們儲放貨物﹐運送、轉交的地方﹐
雪亭鎮因為位於通往天邪國的重要道路﹐所以貨棧往來的貨物相當的多
﹐因此成為少數幾個設有貨棧的村鎮﹐天朝各地的貨棧大多數都互相連
繫﹐因此商業十分發達﹐忙碌的貨棧也常常是缺錢的人打零工的地方﹐
因為貨棧的工作量不固定﹐有大批貨物進來的時候往往需要平常十幾倍
的人手﹐但是貨棧的利潤又雇不起長期的工人。
LONG
	);
	set("exits", ([ /* sizeof() == 1 */
		"east" : __DIR__"sstreet1",
]));
	set("objects", ([
		__DIR__"npc/foreman": 1,
		__DIR__"npc/obj/wagon": 1,
		__DIR__"npc/obj/crate": 3,
	]));
set("no_fight", 1);



	setup();
	replace_program(ROOM);
}
