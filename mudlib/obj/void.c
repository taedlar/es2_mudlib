/*  void.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <login.h>

inherit F_DBASE;

string out_path()
{
    string from;

    if( this_player()
    &&	stringp(from = this_player()->query_temp("last_location")) )
        return from;

    return START_ROOM;
}

private void
create()
{
    set("short", "最後樂園");
    set("long",
        "恭喜您來到這個世界的第一個房間﹗這裡也是在您或某人搞砸了\n"
        "某樣東西之後﹐用來當作避難所的地方。如果您是正在嚐試架設一個\n"
        "使用 ES2 mudlib 為基礎的 MUD﹐請參考 /doc/admin/README﹐如果\n"
        "您是無辜的玩家﹐請打 'out' 離開。\n");
    set("exits/out", (: out_path :) );
    set("no_fight",1);
}

int
do_look(object me, string arg)
{
    printf( "%s - %s\n    %s",
	query("short"),
	wizardp(me)? file_name(this_object()) : "",
	query("long") );
    return 1;
}

int
receive_object(object ob)
{
    return userp(ob);
}
