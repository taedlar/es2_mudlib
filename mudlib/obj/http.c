

#include <http.h>

string request;
string* header;

private void
create()
{
    if( !clonep() ) return;

    seteuid(getuid());
    call_out("http_timeout", HTTP_TIMEOUT *2);
}

private void
logon()
{
}

private void
http_timeout()
{
    receive(HTTP_STAT_TIMEOUT + CRLF);
    destruct(this_object());
}

mixed
process_input(string str)
{
    string hdr, msg;


    if( ! request ) {
        if( strlen(str) < 1 ) return 1;
        if( str[<1]=='\r' ) str = str[0..<2];
        request = str;
        return 1;
    }


    if( strlen(str) > 0 && str[0] != '\r' ) {
        if( arrayp(header) ) header += ({ str });
        else header = ({ str });
        return 1;
    }

    msg = request + "\n" + implode(header, "\n");

    hdr = HTTP_STAT_OK
        + "Content-type: text/html" CRLF
        + "Connection: close" CRLF
        + CRLF;
    receive(hdr + read_file("/doc/index.html"));
    log_file("http/access.log", sprintf("[%s] %s \"%s\"\n",
        ctime(time()), query_ip_number(this_object()), request));
    destruct(this_object());
}

private void
net_dead()
{
    destruct(this_object());
}

void
remove()
{
    remove_call_out();
}

