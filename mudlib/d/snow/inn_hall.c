/*---
description: 飲風客棧大堂
author: Annihilator <taedlar@gmail.com>
---*/

#include <room.h>

inherit INN;

void create() {
    set ("short", "客棧大堂");
    set ("long", @LONG
這裡是一家客棧的大堂﹐老舊的桌椅因為經年的使用而變得烏黑黝亮。
西邊隔著一層竹簾隱隱傳來一陣陣鍋鏟的聲音﹐大概是客棧的廚房。
靠北邊的牆壁邊上有一道通往二樓雅座的樓梯﹐樓梯下就是櫃臺。
東邊是客棧的出口﹐門口一串紙糊燈籠隨風搖擺﹐上面寫著「飲風客棧」：
意思是即使你沒有錢﹐也歡迎來這裡聊天聽書﹐喝西北風。
西南邊有一扇木門通往客棧後面的馬房。
LONG
    );
    set ("objects", ([
        "/d/snow/npc/waiter" : 1,
        "/d/snow/npc/innkeeper" : 1,
    ]));
    set ("valid_startroom", 1);
    set ("no_fight",1);
    set ("detail", ([
        "酒缸" : "你仔細地瞧了瞧酒缸上貼的紅紙﹐不得了﹐有延德年間的高粱﹐京城廖痲子酒坊火漆封印的「拔舌酒」﹐有四季紅的一品醉美人﹐還有好幾罈貼著西郊大鼓樓的陳年老酒。",
        "燈籠" : "一串紙糊的大燈籠高高地掛在屋前的旗杆上﹐共是五個﹐分別寫著「飲風客棧」四個大字﹐最下面的燈籠畫著一個葫蘆。",
        "桌椅" : "這些桌椅零零散散地放置在客棧的廳中﹐看來相當老舊了。",
        "樓梯" : "這道樓梯通往二樓的雅座跟客房。",
        "竹簾" : "竹簾用來隔開客人用膳跟廚房的爐灶﹐但是擋不住從廚房飄出來的香氣。",
        "櫃臺" : "櫃臺後面陳列著一罈罈貼著紅紙條的酒缸﹐這些是客棧掌櫃視如性命的陳年老酒﹐如果你有興趣﹐不妨問問掌櫃有關這些陳年老酒賣是不賣。",
    ]));
    set ("exits", ([
        "west" : __DIR__"inn_kitchen",
        "east" : __DIR__"square_w",
    ]));

    // The door is closed by default. The innkeeper will open it when he starts business.
    create_door ("east", "客棧大門", "west", DOOR_CLOSED);

    setup();
    //load_object("/daemon/board/wizard_board");
    replace_program (INN);
}
