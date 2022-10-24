// vim: syntax=lpc

inherit ROOM;

private void
create()
{
  set("short", "街道");
  set("long", @LONG
這裡是雪亭鎮西邊的街道﹐北邊是雪亭鎮的鄉校﹐平常會有一些鎮
上的長者在這裡談論鎮上的閒事﹐縣裡若有公爺來這裡斷事﹐也是在這
裡設的公堂﹐往西不遠處就是出鎮的官道﹐街道南邊是一家糕餅鋪。
LONG
  );
  set("exits", ([ /* sizeof() == 3 */
	"north" : __DIR__"court",
	"east" : __DIR__"wstreet2",
	"west" : __DIR__"wgate",
  ]));
  set("outdoors", "snow");

  setup();
  replace_program(ROOM);
}
