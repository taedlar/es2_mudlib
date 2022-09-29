// fighter.c

#include <attack.h>
#include <statistic.h>

// prototypes
mixed do_chat(mixed chat);
varargs string name(int raw);
void kill_ob(object ob);
varargs int is_fighting(object ob);
int query_level();
string rank(string politeness);

int accept_fight(object ob)
{
    if( is_fighting() ) {
        int differ;

        differ = query_level() - (int)ob->query_level();
        if( differ > 10 ) {
            do_chat( name() + "笑著說道﹕嘿嘿 ... 來了幫手嗎﹖\n");
            return 1;
        } else if( differ > 5 ) {
            do_chat( name() + "說道﹕嘿嘿 ... " + rank("self") + "今天要來個以寡擊眾﹗\n");
            return 1;
        } else {
            do_chat( name() + "說道﹕這位" + ob->rank("respectful") + "難道是想倚多為勝﹖\n");
            return 0;
        }
    }

    if( ob->is_fighting() ) {
	do_chat( name() + "說道：切磋武藝，還是單打獨鬥的好，我先在一旁掠陣吧。\n");
	return 0;
    }

    if( query_stat_current("kee") < query_stat_maximum("kee") ) {
	do_chat( name() + "說道：等會兒，讓我喘口氣。\n");
	return 0;
    }

    do_chat( name() + "說道﹕不敢﹐請賜招﹗\n");
    return 1;
}

void accept_kill(object ob)
{
    do_chat( name() + "說道﹕" + rank("self") + "與這位" + ob->rank("respectful")
        + "好像無冤無仇吧﹖怎麼 ...﹖\n");
}

void accept_negotiation(object who, string key)
{
    switch(key) {
    case "peace":
        if( !is_killing(who) ) {
            do_chat( name() + "躬身還禮，說道：好說，好說。\n");
            remove_enemy(who);
            who->remove_enemy(this_object());
        }
        else do_chat( name() + "說道：廢話少說，今日不是你死便是我活！\n");
        return;
    case "mercy":
        if( is_killing(who) ) {
            do_chat( name() + "哈哈大笑，說道：既然如此，今天就饒了你。\n");
            remove_killer(who);
            remove_enemy(who);
            who->remove_killer(this_object());
            who->remove_enemy(this_object());
	    // add by dragoon
	    if( who->query_class() == "fighter" )
                who->gain_score("martial fame", - who->query_level() * 10);
        }
    }
}
