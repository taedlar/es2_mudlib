/*---
description: 飲風客棧後院的員工休息室
author: Annihilator <taedlar@gmail.com>
---*/
#include <room.h>
inherit ROOM;

void create() {
    set ("short", "客棧人員休息室");
    set ("long", @LONG
這裡是飲風客棧後院的員工休息室，供 NPC 員工休息和放鬆。
普通玩家無法進入這裡，巫師可以在這裡休息，或者和 NPC 員工聊天，了解客棧的內部運作。
LONG
    );
    set ("no_fight", 1);
    setup();
}
