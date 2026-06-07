/*---
descrition: 巫師公會
author: Annihilator <taedlar@gmail.com>
---*/
inherit ROOM;

void create() {
    set ("short", "巫師公會");
    set ("long", @LONG
這裡就是巫師公會了﹐一個半球型的大浮島漂浮在七彩的雲間，平坦的地面鋪著一層碧綠草皮。
草地北端盡頭是一棟純白的大型建築﹐希臘式的圓柱跟波斯風格的圓頂﹐營造出一種莊嚴的氣氛。
你的東邊是露天的巫師學院﹐往西則是通往世界各地的傳送魔法陣。
LONG
    );
    set ("outdoors", "guild");
    set ("exits", ([
        "east" : __DIR__"academy",
        "west" : __DIR__"portal",
        "north" : __DIR__"meetingroom",
    ]));

    setup();
    // load_object("/daemon/board/wizard");
    replace_program (ROOM);
}
