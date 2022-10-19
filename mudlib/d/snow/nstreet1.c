// vim: syntax=lpc

inherit ROOM;

private void
create()
{
  set("short", "街道");
  set("long", @LONG
這裡是雪亭鎮北邊的主要街道﹐往南通往鎮上最熱鬧的廣場﹐往北
則通往鎮北的山路﹐你的西邊都是一些賣雜貨的店鋪﹐這一帶除了市集
的日子﹐並不常有太多人走動。你注意到東邊有一間不太顯眼的土地公
廟。
LONG
  );
  set("detail", ([ /* sizeof() == 1 */
	"店鋪" : "各式各樣的店鋪都有﹐不過看起來不像是你會用得著的地方。",
  ]));
  set("exits", ([ /* sizeof() == 2 */
	"south" : __DIR__"square_n",
	"north" : __DIR__"nstreet2",
	"east": __DIR__"small_temple",
  ]));
  set("outdoors", "snow");

  setup();
  replace_program(ROOM);
}
