// Room: /d/snow/square_w.c

#include <room.h>
inherit ROOM;

void create()
{
    set("short", "廣場");
    set("long", @LONG
這裡是雪亭鎮廣場的西邊﹐往東可以看見廣場中央的大榕樹﹐一家
小客棧座落在西邊不遠處﹐客棧前的一串燈籠上寫著「飲風客棧」四個
大字﹐廣場上鋪著整齊的石板。
LONG
    );
    set("exits", ([ /* sizeof() == 3 */
	"south" : __DIR__"square_sw",
	"west" : __DIR__"inn_hall",
	"east" : __DIR__"square",
	"north" : __DIR__"bug_room",
    ]));
    set("objects", ([
	"/d/snow/obj/stone" : 1,
    ]));
    set("outdoors", "snow");
    set("no_fight", 1);	// prevent pk when post

    setup();
    load_object("/daemon/board/common");
    create_door("west", "店門", "east", 0);
}
