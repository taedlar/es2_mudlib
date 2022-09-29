/*  garrison.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <npc.h>

inherit F_SOLDIER;

void create()
{
    set_name("官兵", "garrison", "guard");

    set_attr("con", 22);
    set_attr("str", 24);
    set_attr("dex", 18);
    set_attr("cor", 18);
    set_race("human");
    set_class("fighter");
    set_level(25);
    advance_stat("gin", 300);
    advance_stat("kee", 300);

    set_skill("twohanded sword", 60);
    set_skill("blunt", 100);
    set_skill("sword", 100);
    set_skill("parry", 90);
    set_skill("unarmed", 80);
    set_skill("dodge", 90);
    set_skill("blade", 70);
    set_skill("berserk", 150);

    set("long",
        "一位負責維護地方治安的官兵﹐他們受過相當的訓練﹐有時候\n"
        "也加入軍隊作戰。\n");
    set("vendetta_mark", "authority");
    set("chat_chance_combat", 50);
    set("chat_msg_combat", ({
	(: command, "berserk" :)
    }));

    setup();

    set_temp("apply", ([
	"attack": 100,
	"defense": 100,
	"intimidate": 100,
	"wittiness": 100,
    ]));
    carry_object(STOCK_WEAPON("rod"))->wield();
}

int accept_fight(object ob)
{
return 1;

    do_chat((: command, "emote 瞪了瞪眼﹐說道﹕滾開滾開﹗少在這裡囉唆﹗":));
    return 0;
}
