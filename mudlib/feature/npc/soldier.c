

mixed do_chat(mixed chat);
varargs string name(int raw);
int is_fighting(object);
void kill_ob(object);

int
accept_fight(object ob)
{
    switch(random(5)) {
        case 0: do_chat((: command, "shake" :));    break;
        case 1: do_chat(name() + "說道﹕走開走開﹗\n");        break;
        case 2: do_chat((: command, "hehe" :));    break;
    }
    return 0;
}

void
accept_kill(object ob)
{
    if( is_fighting(ob) ) return;
    do_chat( name() + "說道﹕咦﹖造反啦﹖去死吧 ...\n");
    all_inventory(environment())->soldier_assist(this_object(), ob);
}

void
soldier_assist(object alert, object ob)
{
    if( alert==this_object()
    ||    !living(this_object())
    ||    is_fighting(ob) )
        return;

    switch(random(5)) {
    case 0: do_chat(name() + "說道﹕大膽刁民﹗竟敢公然逞兇﹗\n"); break;
    case 1: do_chat(name() + "喝道﹕反了反了﹗兄弟們﹐拿下這個匪徒﹗\n"); break;
    case 2: command("say 喂﹗你想幹什麼﹖"); break;
    case 3: command("say 來人哪﹗拿下那個可疑的傢伙﹗"); break;
    }
    kill_ob(ob);
    ob->fight_ob(this_object());
}

