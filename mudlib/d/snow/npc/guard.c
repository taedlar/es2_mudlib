/*  guard.c

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

inherit F_VILLAGER;

void create()
{
    set_name("青衣漢子", ({"guard"}));
    set_attr("str", 20);
    set_attr("con", 20);
    set_race("human");
    set_class("commoner");
    set_level(20);

    set_skill("sword", 70);
    set_skill("blade", 50);
    set_skill("parry", 50);
    set_skill("unarmed", 40);
    set_skill("dodge", 70);

    set("age", 26);
    set("long", "一個身穿藏青勁裝的漢子﹐雙目炯炯有神地盯著你。\n");
    setup();
    carry_object( STOCK_ARMOR("cloth") )->wear();
    carry_object( STOCK_WEAPON("longsword") )->wield();
    carry_money("coin", 160);
}


void init()
{
    ::init();
    add_action("do_go", "go");
}

// This overrides standard "go" command to emulate the effect of guarding
// an entry.
int do_go(string arg)
{
    // Only checks eastward movement, returning 0 to let standard command
    // process it.
    if( (arg!="east")
    || this_player()->query_class()=="fighter"
    || !this_player()->visible(this_object())
    )
        return 0;

    do_chat((: command, "say 對不起﹐莊主有令﹐只有持英雄帖的貴客才能入內。":));
    write( name() + "攔住了你的去路。\n");
    return 1;
}


void greet_visitor(object ob, object invitation)
{
    destruct(invitation);
    if( environment(ob)!=environment() ) return;

    message_vision(name() + "引領$N過了橋﹐進入雪吟莊。\n", ob);
    ob->move("/d/snowkeep/entrance");
}

int accept_object(object who, object ob)
{
    if( !ob->id("invitation") ) return 0;

    command("say 小人失敬﹗敝莊主已經恭候多時了。\n");
    do_chat((: greet_visitor, who, ob :));
    return 1;
}
