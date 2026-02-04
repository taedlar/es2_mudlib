// vim: set syntax=lpc :

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
