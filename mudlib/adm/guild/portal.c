/*---
description: 巫師傳送門
author: Annihilator <taedlar@gmail.com>
---*/
inherit ROOM;

void create() {
    set ("short", "傳送門");
    set ("long", @LONG
這裡是巫師公會的傳送門，平坦的草地上有一片鋪著地磚的空地。
空地中央有一個巨大的綠色魔法陣，正發出閃爍不定的微弱光芒。
LONG
    );
    set ("outdoors", "guild");
    set ("detail", ([
        "魔法陣" : "這個魔法陣是巫師公會的傳送門，巫師可以從這裡快速抵達世界上的任何地方。",
    ]));
    set ("exits", ([
        "east" : __DIR__"guildhall",
        "inn": "/d/snow/inn_hall",
    ]));

    setup();
    replace_program (ROOM);
}