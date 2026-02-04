// vim: set ts=4 sw=4 syntax=lpc

#include <ansi.h>
#include <npc.h>

inherit F_SOLDIER;

private void
create()
{
    set_name( HIK "范無救將軍" NOR, "hell general black", "general", "black");
    set("nickname", HIK "黑無常" NOR);
    set("title", HIK "勾魂使者" NOR);
    set("long", @LONG
你看到一個又矮又胖的黑臉將軍，頭大如甕，兩眼突出，對你怒目而視。
LONG
    );
    set_attr("str", 45);
    set_attr("con", 40);
    set_attr("dex", 40);
    set_attr("cor", 35);
    set_attr("cps", 45);
    set_attr("spi", 40);
    set_attr("wis", 35);
    set_attr("int", 30);

    set_race("human");
    set_class("soldier");
    set_level(70);
    advance_stat("gin", 300);
    advance_stat("kee", 300);

    set_skill("twohanded sword", 170);
    set_skill("sword", 140);
    set_skill("blade", 120);
    set_skill("parry", 150);
    set_skill("unarmed", 130);
    set_skill("dodge", 110);
    set_skill("berserk", 180);

    setup();

    add_temp("apply/attack", 100);
    add_temp("apply/defense", 100);
    add_temp("apply/intimidate", 100);
    add_temp("apply/wittiness", 100);
    add_temp("apply/damage", 50);
    add_temp("apply/armor", 100);
}


