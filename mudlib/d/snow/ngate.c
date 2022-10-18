// vim: syntax=lpc

inherit ROOM;

private void
create()
{
  set("short", "山路");
  set("long", @LONG
你現在正走在一條蜿蜒的山路上﹐往南不遠處可以看見許多人家和
房舍﹐往北則通往山上﹐山路兩旁長滿雜草﹐靠西的一面是山壁﹐東邊
是平緩的山坡﹐山坡盡頭是一片草地。
LONG
  );
  set("outdoors", "snow");
  set("connect-p", 1);
  set("exits", ([ /* sizeof() == 1 */
	"south" : __DIR__"nstreet2",
//	"north" : "/d/goat/sroad1",
  ]));
  set("objects", ([
	__DIR__"npc/garrison": 4,
	__DIR__"npc/lieutenant": 1,
  ]) );

  setup();
  replace_program(ROOM);
}
