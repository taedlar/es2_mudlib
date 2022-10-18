// patrol_garrison.c

#include <npc.h>

inherit F_VILLAGER;

void create()
{
    set_name("巡邏官兵", ({ "patrol garrison", "patrol", "garrison" }) );
    set_race("human");
    set_class("commoner");
    set_level(15);
    set_skill("sword", 60);
    set_skill("parry", 60);
    set_stat_maximum("gin", 150);
    set_stat_maximum("kee", 150);

    set("pursuer", 1);
    set("long",
        "一位負責維護地方治安的官兵﹐他們受過相當的訓練﹐有時候\n"
        "也加入軍隊作戰。\n");
    set("vendetta_mark", "authority");
    setup();
    carry_object( STOCK_ARMOR("cloth") )->wear();
    // Wiled the sword when protecting someone. (Just for cool :P)
    carry_object( STOCK_WEAPON("broadsword"));
}


int accept_fight(object ob)
{
    do_chat((: command, "emote 瞪了瞪眼﹐說道﹕滾開滾開﹗少在這裡囉唆﹗":));
    return 0;
}

void protect(object ob)
{
    command("emote 大喝﹕反了反了﹗光天化日之下竟敢殺人逞凶﹖納命來﹗");
    command("wield sword");
    guard_ob(ob);
}
