// herbalist.c

#include <npc.h>

inherit F_VILLAGER;
inherit F_VENDOR;

void create()
{
	set_name("藥鋪掌櫃", ({"herbalist"}));
	set_race("human");
	set_class("commoner");
	set_level(3);
	set_skill("unarmed", 5);
	set_skill("parry", 5);
	set_skill("dodge", 5);
	set_skill("dagger", 5);

	set("age", 44);
	set("long",
		"藥鋪掌櫃的是位胖胖的中年男子﹐雖然年紀有四五十歲了﹐不過看起\n"
		"來依然紅光滿面、精神煥發﹐你可以用 list 問他店裡有哪些藥。\n");

	set("merchandise", ([
		STOCK_MEDICATION("black_pill") : 50,
		STOCK_MEDICATION("boar_berry") : 40,
		STOCK_MEDICATION("wild_ginseng") : 30,
		STOCK_MEDICATION("dragon_saliva") : 20,
	]));

	setup();
	carry_object("/obj/area/obj/cloth")->wear();
	carry_object(__DIR__"obj/fur_hat")->wear();
}

void init()
{
	::init();
	add_action("do_vendor_list", "list");
}
