

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

