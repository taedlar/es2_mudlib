// Room: /d/snow/river1.c
/*
    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

inherit ROOM;

void create()
{
        set("short", "溪邊");
        set("long", @LONG
這裡是溪流旁的亂石灘，溪流兩岸都有一丈多高，只有此處因為溪
流彎道沖積，形成一處小石灘，水流衝激的聲音經過溪谷的回音震耳欲
聾，往南一條小路可以回到岸邊。
LONG
        );
        set("resource", ([ /* sizeof() == 1 */
                "water" : "1",
]));
        set("outdoors", "snow");
        set("exits", ([ /* sizeof() == 1 */
		"south" : "/d/snow/npath3",
		"north" : "/d/chixiao/river2",
]));

        setup();
        replace_program(ROOM);
}
