

#include <ansi.h>
#include <npc.h>

inherit F_SOLDIER;

private void
create()
{
    set_name( HIW "謝必安將軍" NOR, "hell general white", "general", "white");
    set("nickname", HIW "白無常" NOR);
    set("title", HIW "勾魂使者" NOR);
    set("long", @LONG
你看到一個又高又瘦的白臉將軍，一根長長的舌頭直吐到胸前，兩隻細眼瞇著往你
這邊直瞧。
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

    set_skill("twohanded sword", 60);
    set_skill("blunt", 100);
    set_skill("sword", 100);
    set_skill("parry", 90);
    set_skill("unarmed", 80);
    set_skill("dodge", 90);
    set_skill("blade", 70);
    set_skill("berserk", 150);

    setup();

    add_temp("apply/attack", 100);
    add_temp("apply/defense", 100);
    add_temp("apply/intimidate", 100);
    add_temp("apply/wittiness", 100);
    add_temp("apply/damage", 50);
    add_temp("apply/armor", 100);
}


