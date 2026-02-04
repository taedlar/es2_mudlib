// vim: set syntax=lpc :

#ifndef USER_H
#define USER_H

#define F_AUTOLOAD	"/feature/user/autoload.c"
#define F_ALIAS		"/feature/user/alias.c"
#define F_EDIT		"/feature/user/edit.c"
#define F_MORE		"/feature/user/more.c"
#define F_FLAG		"/feature/user/flag.c"

#define USER_PROTECT()	if( origin()==ORIGIN_CALL_OTHER\
			&&  geteuid(previous_object())!=ROOT_UID\
			&&  geteuid(previous_object())!=DOMAIN_UID\
			&&  geteuid(previous_object())!=MUDLIB_UID\
			&&  geteuid(previous_object())!=geteuid(this_object())\
			) {\
			log_file("protection", sprintf("[%s] %s %O(%s)\n", ctime(time()), geteuid(this_object()), previous_object(), geteuid(previous_object())));\
			error("User protection violated by " + geteuid(previous_object()) + "\n");\
			}

// Anti-spam limit
#define MAX_COMMAND_PER_SECOND		10

// Signatures to tell user_dump() the reason to dump a user.
#define DUMP_NET_DEAD			1
#define DUMP_IDLE			2

// Kick out net-dead users in 15 minutes
#define NET_DEAD_TIMEOUT		900

// Kick out idle users in 20 minutes
#define IDLE_TIMEOUT			1200

#endif	/* USER_H */
