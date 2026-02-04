// vim: set ts=4 sw=4 syntax=lpc

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

