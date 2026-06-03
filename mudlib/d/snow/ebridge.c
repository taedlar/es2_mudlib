/*---
description: wooden bridge, used for crossing the mountain stream.
author: Annihilator <taedlar@gmail.com>
---*/

inherit ROOM;

void create() {
    set ("short", "木橋");
    set ("long", @LONG
你現在來到一條橫過山澗的木橋上。
山澗雖不甚寬﹐但是深度卻有三、四丈﹐是雪亭鎮上居民主要的飲用水源。
木橋連通溪邊小路和東邊的山路，橋面已經有些年頭了﹐不過看起來還算堅固。
橋下的溪水正發出轟隆轟隆的巨響流過溪谷﹐往西是雪亭鎮﹐往東則通往著名的雪吟山莊。
LONG
    );
    set ("no_clean_up", 0);
    set ("objects", ([
        __DIR__"npc/guard" : 2,
    ]));
    set ("outdoors", "snow");
    set ("exits", ([
	    "west" : __DIR__"npath3",
//	"east" : "/d/snowkeep/entrance",
    ]));

    setup();
    replace_program (ROOM);
}
