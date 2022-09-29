/*  httpd.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <http.h>

private void create() { seteuid(getuid()); }

void
logon(object ob)
{
    int q = query_ip_port();
    write(HTTP_VERSION + " ");
    CHANNEL_D->do_channel(this_object(), "sys",
	sprintf("Request from %s\n", query_ip_name(ob)), 0);
    write("request:");
    input_to("get_request", ob);
}

private void
get_request(string req, object ob)
{
    CHANNEL_D->do_channel(this_object(), "sys",
	sprintf("Request = \"%s\"\n", req), 0);
    write("Your request is " + req + CRLF);
    destruct(ob);
}

string
query(string prop)
{
    if( prop=="channel_id" ) return "WWW 伺服器";
    return 0;
}
