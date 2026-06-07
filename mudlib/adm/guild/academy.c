/*---
descrition: 巫師學院
author: Annihilator <taedlar@gmail.com>
---*/
inherit ROOM;

void create() {
    set ("short", "巫師學院");
    set ("long", @LONG
你來到一片平坦的草地，幾株大樹和樹下的大理石桌與舒適木椅提供了舒適的閱讀環境。
草地外是看不見邊際的雲海，以及雲海上燦爛的星空。
這裡是巫師們研究魔法與世界萬物的地方﹐如果你是新手巫師﹐歡迎來到這裡和其他人互相討論﹐如果你是老手﹐也請不吝回答新手的問題。
LONG
    );
    set ("outdoors", "guild");
    set ("exits", ([
        "west" : __DIR__"guildhall",
    ]));

    setup();
    // load_object("/daemon/board/lpc");
    replace_program (ROOM);
}
