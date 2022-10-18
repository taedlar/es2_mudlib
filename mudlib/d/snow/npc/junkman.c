// junkman.c

#include <npc.h>

inherit F_VILLAGER;

void create()
{
	set_name("拾荒老頭", ({ "junkman" }));
	set_race("human");
	set_level(2);

	set("age", 66);
	set("long",
		"一個痀僂的拾荒老頭﹐靠撿拾廢物維生﹐如果你願意幫助這樣一個\n"
		"老人過好一點的生活﹐或者是願意替辛苦的巫師們節省一點記憶體\n"
		"﹐請把你身上多餘的東西給他﹐可以得到一點的 civism。\n");
	set("bounty/mortal sin", 5);
	set("chat_chance", 8);
	set("chat_msg", ({
		(: random_move :),
		(: command, "get all" :),
	}));
	setup();

	// Always flee.
	set_stat_notify("gin", 100);
	set_stat_notify("kee", 100);
	set_stat_notify("sen", 100);
}

int accept_fight()
{
	do_chat("拾荒老頭露出害怕的神情﹐說道﹕這 ... 咳 ... 饒了小人吧 ...\n");
	return 0;
}
/* 暫時取消 d-Dragoon
void junk_ob()
{
	object ob;

	command("emote 不住地點頭﹐說道﹕謝謝 ... 咳 ... 謝謝 ... ");
	foreach(ob in all_inventory()) {
		if( function_exists("stuff_ob", ob) )
			command("eat " + ob->query("id"));
		else if( ob->query("wear_as") )
			ob->wear();
		else if( ob->query("wield_as") )
			ob->wield();
		if( ob && !ob->query("equipped") ) destruct(ob);
	}
}

int accept_object(object me, object ob)
{
	do_chat((: junk_ob :));
//	if( !ob->query_amount() )
//		me->gain_score("civism", 1);

	return 1;
}
*/
