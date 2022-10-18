// vim: syntax=lpc

inherit ROOM;

private void
create()
{
  set("short", "溪邊");
  set("long", @LONG
這裡是溪流旁的亂石灘，溪流兩岸都有一丈多高，只有此處因為溪
流彎道沖積，形成一處小石灘，水流衝激的聲音經過溪谷的回音震耳欲
聾，往南一條小路可以回到岸邊。
LONG
  );
  set("resource", ([ /* sizeof() == 1 */
	"water" : "1",
  ]));
  set("outdoors", "snow");
  set("exits", ([ /* sizeof() == 1 */
	"south" : __DIR__"npath3",
//	"north" : "/d/chixiao/river2",
  ]));

  setup();
  replace_program(ROOM);
}
