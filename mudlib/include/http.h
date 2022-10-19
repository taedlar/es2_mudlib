/*  http.h

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#ifndef HTTP_H
#define HTTP_H

#define CRLF		"\n"
#define HTTP_VERSION	"HTTP/1.0"

#define HTTP_STAT_OK		(HTTP_VERSION " 200 Ok" CRLF)
#define HTTP_STAT_NOTFOUND	(HTTP_VERSION " 404 File not found" CRLF)
#define HTTP_STAT_TIMEOUT	(HTTP_VERSION " 408 Request Time-out" CRLF)

/* Timeout to wait for the client to send its request */
#define HTTP_TIMEOUT	5

#endif	/* HTTP_H */
