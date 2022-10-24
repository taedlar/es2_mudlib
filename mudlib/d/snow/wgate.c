// vim: syntax=lpc

inherit ROOM;

private void
create()
{
  set("short", "雪亭鎮");
  set("long", @LONG
這裡是雪亭鎮西邊的官道﹐雖然雪亭驛設在鎮上的北邊﹐但是官道
卻是從西邊入雪亭﹐因為原來修官道時利用了一段從前中陽侯征奐族亂
軍的棧道﹐因此上這條官道便從西邊穿過山谷接古中陽棧道﹐出黃石隘
﹐往東通往雪亭鎮的街道﹐往西則出鎮。
LONG
  );
  set("outdoors", "snow");
  set("exits", ([ /* sizeof() == 1 */
	"east" : __DIR__"wstreet3",
//	"northwest" : "/d/graveyard/grave1",
  ]));

  setup();
  replace_program(ROOM);
}
