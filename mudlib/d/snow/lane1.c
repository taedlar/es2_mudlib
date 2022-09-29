// File: /d/snow/lane1.c
// Updated by annihilator (Wed Mar 15 17:20:06 2000)

inherit ROOM;

void create()
{
	  set("short", "僻靜小巷");
	  set("long", @LONG
這裡是條僻靜小巷的轉角處﹐小巷往北可以通往溪邊﹐從這裡就可
以聽到溪流轟轟的聲音﹐東邊是一棟破舊的大宅院﹐門口的一的破匾寫
著「長樂侯府」﹐不過裡面已經是斷垣殘璧了﹐小巷南邊有一座小土地
公廟。
LONG
	  );
	  set("outdoors", "snow");
	  set("exits", ([ /* sizeof() == 3 */
	"north" : __DIR__"lane2",
	"west" : __DIR__"epath",
	"east" : __DIR__"ruin1",
]));

	setup();
	  replace_program(ROOM);
}
