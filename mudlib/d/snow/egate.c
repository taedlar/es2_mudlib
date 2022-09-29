// egate.c

inherit ROOM;

void create()
{
	set("short", "河邊");
	set("long", @LONG
破舊大宅外，你眼前是兩條河流的匯流處，來自西北、東北的兩條
小河匯流後，緩緩的向西南流去。西北邊的河流上有一座小橋，過了橋
，沿著東北邊河流的小石子路可以通往李家村。　
LONG
	);
	set("detail", ([ /* sizeof() == 2 */
		"小橋" : "一座巨檜木切削搭成的木橋.\n",
	]));
	set("outdoors", "snow");
	set("exits", ([ /* sizeof() == 1 */
		"west" : __DIR__"ruin1",
		"northeast" : "/d/lee/r1",
	]));

	setup();
}
