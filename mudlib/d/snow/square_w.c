/*---
description: 雪亭鎮廣場西邊
author: Annihilator <taedlar@gmail.com>
---*/

#include <room.h>

inherit ROOM;

void create() {
    set ("short", "廣場");
    set ("long", @LONG
這裡是雪亭鎮廣場的西邊﹐往東可以看見廣場中央的大榕樹。 
一家小客棧座落在西邊不遠處﹐客棧前的一串燈籠上寫著「飲風客棧」四個大字。
廣場上鋪著整齊的石板，在這樣的小鎮通常是因為驛站的緣故才會費事鋪設。
LONG
    );
    set("exits", ([ /* sizeof() == 3 */
        "south" : __DIR__"square_sw",
        "west" : __DIR__"inn_hall",
        "east" : __DIR__"square",
        "north" : __DIR__"bug_room",
    ]));
    create_door("west", "客棧大門", "east", DOOR_CLOSED);
    set("objects", ([
        __DIR__"obj/stone" : 1,
    ]));
    set("outdoors", "snow");
    set("no_fight", 1);	// prevent pk when post

    setup();
    // load_object("/daemon/board/common");
    replace_program (ROOM);
}
