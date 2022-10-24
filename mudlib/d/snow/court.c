// vim: syntax=lpc

inherit ROOM;

private void
create()
{
  set("short", "鄉校");
  set("long", @LONG
這裡是雪亭鎮的鄉校﹐平常鎮民對官府的施政有什麼不滿的可以到
這裡來向校老申訴﹐所謂的校老就是地方上有德行與聲望的老人﹐今年
擔任校老的是開磨坊的楊老爹﹐但是他最近身體不是很好﹐並不常到鄉
校裡來﹐往南是街道﹐北邊是一間衙堂﹐當縣裡派師爺來這裡斷事就會
在那裡開堂。
LONG
  );
  set("outdoors", "snow");
  set("exits", ([ /* sizeof() == 1 */
	"south" : __DIR__"wstreet3",
  ]));
  set("objects", ([
	__DIR__"npc/oldman": 1
  ]));

  setup();
  replace_program(ROOM);
}
