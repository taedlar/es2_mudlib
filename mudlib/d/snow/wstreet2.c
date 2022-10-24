// vim: syntax=lpc

inherit ROOM;

private void
create()
{
  set("short", "街道");
  set("long", @LONG
這裡是雪亭鎮西邊的街道﹐北邊是一間看起來像是客棧的大屋，圍
牆上曬著一些陳舊卻潔白的衣服。南邊有間青瓦屋，幾株大樹從圍牆內
探出頭來。
LONG
  );
  set("exits", ([
	"east" : __DIR__"wstreet1",
	"west" : __DIR__"wstreet3",
  ]));
  set("outdoors", "snow");

  setup();
  replace_program(ROOM);
}
