/*  http.c - user object for HTTP request

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

    /* 取得 client 端的 request */
    if( ! request ) {
	if( strlen(str) < 1 ) return 1;	/* as specified in RFC#2608 */
	if( str[<1]=='\r' ) str = str[0..<2];
	request = str;
	return 1;
    }

    /* 取得 headers */
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
