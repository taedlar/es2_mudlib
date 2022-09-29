// Room: /d/snow/fireplace.c

inherit ROOM;

void create()
{
    set("short", "大灶內");
    set("long", @LONG
你現在正蜷縮著擠在一個大灶之中, 頭頂上方隱隱可看到煙囪的出
口, 不過顯然沒有辦法爬出去, 這裡的牆壁因為長年燒火而變得烏黑, 
地面上積了一層厚厚的柴灰.
LONG
    );
    set("exits", ([ /* sizeof() == 1 */
	"out" : __DIR__"inn_kitchen",
    ]));

    set("objects", ([
	__DIR__ "npc/obj/woodsword": 1,
	__DIR__ "npc/obj/woodblade": 1
    ]));

    setup();
    replace_program(ROOM);
}
