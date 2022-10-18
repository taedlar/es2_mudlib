// Room: /d/snow/herb_shop.c

inherit ROOM;

void create()
{
	set("short", "小藥鋪");
	set("long", @LONG
這裡是一間小藥鋪﹐連像樣一點的招牌都沒有﹐只在門邊掛著一塊
不甚起眼的木牌﹐上面寫著「順生堂」三個字﹐老舊的櫃臺後面陳列著
一列藥櫃﹐藥櫃的每個抽屜按藥鋪的規矩都沒有任何標籤﹐西面的牆上
吊著幾束曬乾的甘草﹐像這種小藥鋪沒有錢燒檀香祛除藥味﹐只好用甘
草替代。
LONG
	);
	set("exits", ([ /* sizeof() == 1 */
		"north" : __DIR__"epath",
	]));
	set("objects", ([
		__DIR__"npc/herbalist": 1,
	]));

	setup();
	replace_program(ROOM);
}
