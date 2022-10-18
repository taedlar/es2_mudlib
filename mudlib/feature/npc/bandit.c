// villager.c

// prototypes
mixed do_chat(mixed chat);
varargs string name(int raw);
void kill_ob(object ob);
varargs int is_fighting(object ob);

int
accept_fight(object ob)
{
	if( is_fighting() ) {
		do_chat( name() + "說道﹕哼 ... 倚多為勝不是好漢﹐待會兒再陪你玩﹗\n");
		return 0;
	}
	do_chat( name() + "說道﹕哈﹗看來你是活得不耐煩了﹐讓我來教訓教訓你﹗\n");
	kill_ob(ob);
	return 1;
}

void
accept_kill(object ob)
{
	do_chat( name() + "「哼」地一聲﹐說道﹕那得瞧你有沒有這個本事﹗\n");
}

