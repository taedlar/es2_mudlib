/*  villager.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

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
