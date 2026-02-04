// vim: set ts=4 sw=4 syntax=lpc

mixed do_chat(mixed chat);
varargs string name(int raw);

int accept_fight(object ob)
{
    switch(random(5))
    {
    case 0: do_chat((: command, "shake" :)); break;
    case 1: do_chat((: command, "?" :)); break;
    case 2: do_chat((: command, "smile" :)); break;
    case 4: do_chat( name() + "武功我可不會，失禮了。\n");
    }

    return 0;
}

void accept_kill(object ob)
{
    do_chat( name() + "說道﹕咦﹖怎麼動起粗來了﹖救命哪 ...\n");
}

