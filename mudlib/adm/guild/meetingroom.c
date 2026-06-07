/*---
description: 會議廳
author: Annihilator <taedlar@gmail.com>
---*/
inherit ROOM;

void create() {
    set ("short", "會議廳");
    set ("long", @LONG
這裡是巫師公會的會議廳，圓形的明亮大廳被一圈高大的圓柱包圍著，從地面一直延伸到半空中，彷彿支撐著整個天空。
高聳的圓頂浮現出明亮的星星，但顯現的似乎是古老的星空並且以極快的速度演化中，你意識到這個大廳本身就是一個時空的紀錄儀器。
巫師們商討重要事情時都會在這裡進行。
LONG
    );
    set("exits", ([
        "south" : "/adm/guild/guildhall"
    ]));
    setup();
    // load_object("/daemon/board/wizard_talk");
    replace_program (ROOM);
}
