/*  post_office.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

inherit ROOM;

void create()
{
    set("short", "雪亭驛");
    set("long", @LONG
這裡是雪亭鎮的驛站﹐雖然驛站主要是替官府遞送文書﹐不過由於近年來
土匪強盜出沒頻繁﹐也不少百姓會花一些錢請驛站的官員幫忙送信收信，喬陰
縣的縣令認為土匪擾民是官府的施政缺失，驛站若是還收百姓的錢，實在是受
之有愧，索性下令縣內驛站對百姓的郵遞委託一律不得收費。

如果你想要收發信，可以用 request 向驛站官員領取收發信的信札。
LONG
    );
    set("exits", ([ /* sizeof() == 1 */
        "east" : __DIR__"nstreet2",
    ]));
    set("post_office", 1);    // mailbox checks this flag.
    set("no_fight", 1);

    setup();
}

void init()
{
    add_action("do_request", "request");
}

int do_request(string arg)
{
    object mbox;

    if( objectp(mbox = this_player()->query_temp("mbox_ob"))
    &&	base_name(mbox)==MAILBOX_OB
    &&	environment(mbox)==this_player() )
        return notify_fail("你已經領有一個信札了。\n");

    mbox = new(MAILBOX_OB);
    mbox->move(this_player());

    write("驛站的官員將一個寫著你名字的信札遞了給你。\n");
    return 1;
}

int
valid_leave(object ob)
{
    object mbox;

    if( objectp(mbox=ob->query_temp("mbox_ob")) ) {
	mbox->retire();
	write("你將信札交回給驛站官員保管。\n");
	ob->delete_temp("mbox_ob");
    }

    return 1;
}
