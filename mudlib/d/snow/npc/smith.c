// smith.c

#include <npc.h>

inherit F_VILLAGER;
inherit F_VENDOR;

void create()
{
        set_name("鐵匠", ({ "smith" }) );
        set_race("human");
        set_level(13);
        set_stat_maximum("kee", 80);
        set_attr("str", 19);
        set_attr("cor", 17);
        set_attr("cps", 15);
        set_attr("dex", 18);

        set("long",
                "一個身材粗壯的鐵匠﹐正忙碌地工作著。\n"
                "如果你想買一些鐵器﹐可以用 list 看有哪些東西﹐用 buy 向鐵匠購買。\n");

        set("merchandise", ([
                __DIR__"obj/hoe": 3,
                __DIR__"obj/shovel": 5,
                "/obj/area/obj/dagger": 10,
                "/obj/area/obj/shortsword": 5,
        ]));

        set("chat_chance", 5);
        set("chat_msg", ({
                "鐵匠將一塊燒紅的鐵塊「嗤」地一聲浸入一旁的水箱中。\n",
                "鐵匠用手中的鐵鎚在鐵砧上用力敲打。\n",
                "鐵匠說道﹕站開點﹐小心別燙著了。\n",
                "鐵匠說道﹕唉 ... 真把我給忙壞了。\n",
        }));
        setup();
        carry_object(__DIR__"obj/smock")->wear();
}

int accept_fight()
{
        do_chat("鐵匠瞪了瞪眼﹐說道﹕走開﹐我正忙著。\n");
        return 0;
}

void init()
{
        ::init();
        add_action("do_vendor_list", "list");
}
int accept_object(object who, object ob)
{
        object thing;
        if ( !ob->id("lo iron") ) return 0;
        call_out((:command("say 喔！要製成箭嗎, 那簡單, 馬上就可以做好.\n"):),1);
        thing=new(__DIR__"obj/arrow");
        thing->move(who);
call_out((:message_vision("鐵匠把剛做好的箭交給了$N。\n",this_player(),) :),2);
        return 1;
}



