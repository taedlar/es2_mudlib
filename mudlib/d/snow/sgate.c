// Room: /d/snow/sgate.c

inherit ROOM;

void create()
{
	set("short", "草棚");
	set("long", @LONG
這裡是雪亭鎮南邊的入口﹐往北穿過草棚通往雪亭鎮的街道﹐出草
棚往鎮外有兩條路﹐往東南的路穿過雪亭南方的老松林﹐通往縣城喬陰
﹐這條路雖然比較近﹐但是近來一群自稱老松寨的土匪常常在這一帶出
沒﹐往西的一條沿著老松林外的大路通往鄰近的五堂鎮﹐從喬陰縣城往
天邪國的商旅現在大多繞道走這一條路﹐因為往五堂鎮的路上有一處北
軍府訓練新兵的軍營﹐常有軍士巡邏﹐攔路強人多半不敢滋事。
LONG
	);
	set("outdoors", "snow");
	set("connect-p", 1);
	set("exits", ([ /* sizeof() == 1 */
		"north" : __DIR__"sstreet1",
		"southeast" : "/d/oldpine/road1",
		"west" : "/d/newcamp/gate",
	]));

	setup();
}
