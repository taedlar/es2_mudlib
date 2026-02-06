

#pragma save_binary

#include <login.h>

//#define DEBUG

inherit F_DBASE;

// By Annihilator@Eastern.Stories (11/25/94)

// This is the "effective" access status compare to the "status" property
// in login object that this mapping determine wizards' access rights
// directly. It is important to make the "status" property matches this
// mapping for wizards higher than regular wizard such as sage, arch and
// admin. For thoese users whose name doesnot appear in this mapping, they
// are divided into three groups (wizard),(player) and (object) using
// wizardp() and interactive() efun.
// The parenthesis is nessessary to prevend players naming themself "admin"
// to hack the security system.
// 
// 01/14/95 Annihilator - wiz_status are defined outside this daemon in the
//                      WIZLIST file.

// Note: This mapping need be initialized before any file access to let
//       valid_read() and valid_write() works correctly on boot time.
private mapping wiz_status = ([]);

private string *wiz_levels =
({
    "(player)",
    "(immortal)",
    "(apprentice)",
    "(wizard)",
    "(arch)",
    "(admin)"
});

// A valid write attempt must pass 2 checks: your uid or status must not be
// "excluded" and must be "trusted" on that directory in order to write in 
// that directory. The directories containing the file is searched backward
// from the current directory to root. And exclude is checked prior than
// trusted.



private mapping trusted_write =
([
    "/":		({ ROOT_UID, "(admin)", "(arch)" }),
    "/data/board":	({ MUDLIB_UID }),
    "/data/daemon":	({ MUDLIB_UID }),
    "/open":		({ "(wizard)", "(apprentice)" }),
#if 0

    "/d/chin_palace":	({ "cuteweca" }),
    "/d/cloudy":	({ "knomo" }),
    "/d/zoo":		({ "cuteweca" }),
    "/d/forest":	({ "winni" }),
    "/d/flower":	({ "grain" }),
    "/d/purple":	({ "trasy" }),
    "/d/month":		({ "pou" }),
    "/d/lake":		({ "winni" }),
    "/d/swaylan":	({ "pou" }),
    "/d/bridgetown":	({ "pou" }),
    "/d/fishport":	({ "trasy" }),
    "/d/lee":		({ "windeye" }),
    "/d/tree":		({ "etime" }),
    "/d/market":        ({ "kkxoo" }),
    // add by dragoon to test
    "/d/fei/sell":	({ "(player)" }),
    "/d/skysnow":	({ "wesily" }),
    "/d/blackwind":	({ "duncan" }),
    "/d/fon":		({ "newbie" }),
    "/d/wumountain":	({ "material" }),
#endif
]);

private mapping exclude_write =
([
    "/adm":		({ "(arch)" }),
    "/data":		({ "(arch)" }),
    "/log":		({ "(arch)" }),
    "/u":		({ "(arch)" }),
    "/cmds":		({ "(arch)" }),
]);



private mapping trusted_read =
([
    "/":		({ ROOT_UID, MUDLIB_UID, "(admin)", "(arch)",
			   "(wizard)", "(apprentice)" }),
    "/d":		({ DOMAIN_UID }),
    "/obj":		({ DOMAIN_UID }),
]);

private mapping exclude_read =
([
    "/arch":		({ "(wizard)", "(apprentice)" }),
    "/adm":		({ "(wizard)", "(apprentice)" }),
    "/data":		({ "(wizard)", "(apprentice)" }),
    "/u":		({ "(wizard)", "(apprentice)" }),
    "/log":		({ "(wizard)", "(apprentice)" }),
]);

private void
create()
{
    string *wizlist, entry, uid, lvl;

    seteuid(getuid());
    set("channel_id", "安全系統");

    wizlist = explode (read_file (WIZLIST) || "", "\n");
    wiz_status = allocate_mapping(sizeof(wizlist));
    foreach(entry in wizlist) {
	if( entry[0]=='#'
	||  sscanf(entry, "%s %s", uid, lvl)!=2 )
	    continue;
	if( member_array(lvl, wiz_levels)==-1 ) continue;
	wiz_status[uid] = lvl;
    }
}

string *
query_wizlist()
{
    return keys(wiz_status);
}

// This function returns the status of an uid or an object.
string
get_status(mixed ob)
{
    string euid;
        
    if( objectp(ob) ) {
	euid = geteuid(ob);
	if( !euid ) euid = getuid(ob);
    } else if( stringp(ob) ) {
	euid = ob;
	if( member_array(euid, wiz_levels)!=-1 ) return euid;
    }

    if( !undefinedp(wiz_status[euid]) ) return wiz_status[euid];
    else return "(player)";
}

int
get_wiz_level(object ob)
{
    return (int)member_array(get_status(ob), wiz_levels);
}

int
set_status(mixed ob, string status)
{
    string uid, *wiz, wizlist, old_status;
    int i;

    if( !this_player(1)
    ||	(member_array(status, wiz_levels) < 0)
    ||	(geteuid(previous_object())!= ROOT_UID) )
	return 0;

    if( objectp(ob) )		uid = getuid(ob);
    else if(stringp(ob))	uid = ob;
    else return 0;

    old_status = wiz_status[uid];
    if( !old_status ) old_status = "(player)";
    if( status == old_status ) return 0;
        
    if( status == "(player)" ) map_delete(wiz_status, uid);
    else wiz_status[uid] = status;

    wiz = keys(wiz_status);
    for(wizlist = "", i=0; i<sizeof(wiz); i++)
	wizlist += wiz[i] + " " + wiz_status[wiz[i]] + "\n";

    rm(WIZLIST);
    write_file(WIZLIST, wizlist);

    log_file( "static/promotion", capitalize(geteuid(this_player(1)))
	+ " promotes " + capitalize(uid) + " from " + old_status + " to "
	+ status + " on " + ctime(time()) + "\n" );

    return 1;
}

string *get_wizlist() { return keys(wiz_status); }

// valid_write - called by the master object to determine whether if an user
//             is allowed to write on a certain file.

int
valid_write(string file, object user, string func)
{
    string euid, status, *path, *perm, dir;
    int i;

#ifdef DEBUG
    if( this_player() && wizardp(this_player()) )
	printf("SECURITY_D: %s: %s %O(%s)\n", func, file, user,
		geteuid(user));
#endif


    if( !(euid = geteuid(user)) ) return 0;
    status = get_status(user);


    if( euid==ROOT_UID ) return 1;

    path = explode(file, "/") - ({ "" });
    if( file[0] != '/' ) file = "/" + file;


    if( sizeof(path)>=3 && euid[0]>='a' && euid[0]<='z'
    && path[0]=="u" && path[1]==euid[0..0] && path[2]==euid ) return 1;


    if( file==SIMUL_EFUN_OB && path[0]=="log" ) return 1;

#ifdef	SAVE_USER

    if( func=="save_object"
    &&  (file==user_data(euid) || file==login_data(euid)
	|| file==mail_data(euid)) )
	return 1;
#endif


    for( i=sizeof(path)-1; i>=0; i--)
    {
	dir = "/" + implode(path[0..i], "/");
	if( undefinedp(perm = exclude_write[dir]) ) continue;
	if( (member_array(euid, perm)>=0) || (member_array(status, perm)>=0) )
	    return 0;
    }


    if( (member_array(euid, trusted_write["/"])>=0)
    ||	member_array(status, trusted_write["/"])>=0 )
	return 1;

    for( i=sizeof(path)-1; i>=0; i--)
    {
	dir = "/" + implode(path[0..i], "/");
	if( undefinedp(perm = trusted_write[dir]) ) continue;
	if( (member_array(euid, perm)>=0) || (member_array(status, perm)>=0) )
	    return 1;
    }

    return 0;
}

int
valid_read(string file, mixed user, string func)
{
    string euid, status, *path, *perm, dir;
    int i;

#ifdef DEBUG
    if( this_player() && wizardp(this_player()) )
	printf("SECURITY_D: %s: %s %O(%s)\n", func, file, user,
		geteuid(user));
#endif


    if( !(euid = geteuid(user)) ) return 0;
    status = get_status(user);


    if( euid==ROOT_UID ) return 1;

    // allow operations on user data of the same euid
    if (file==user_data(euid) || file==login_data(euid) || file==mail_data(euid))
	return 1;

    path = explode(file, "/") - ({ "" });
    if( file[0] != '/' ) file = "/" + file;


    if( sizeof(path)>=3 && euid[0]>='a' && euid[0]<='z'
    && path[0]=="u" && path[1]==euid[0..0] && path[2]==euid ) return 1;


    for(i=sizeof(path)-1; i>=0; i--) {
	dir = "/" + implode(path[0..i], "/");
	if( undefinedp(perm = exclude_read[dir]) ) continue;
	if( (member_array(euid, perm)>=0) || (member_array(status, perm)>=0) )
	    return 0;
    }


    if( member_array(euid, trusted_read["/"])!=-1 ) return 1;
    if( member_array(status, trusted_read["/"])!=-1 ) return 1;
    for(i=sizeof(path)-1; i>=0; i--) {
	dir = "/" + implode(path[0..i], "/");
	if( undefinedp(perm = trusted_read[dir]) ) continue;
	if( (member_array(euid, perm)>=0) || (member_array(status, perm)>=0) )
	    return 1;
    }

    return 0;
}

int
valid_seteuid( object ob, string euid )
{
#ifdef DEBUG
    if( this_player() && wizardp(this_player()) )
	printf("SECURITY_D: seteuid: %O(%s) %s\n", ob, getuid(ob), euid);
#endif

    if( (euid==0)
    ||	(euid==getuid(ob))
    ||	(getuid(ob)==ROOT_UID)
    ||	(wiz_level(ob) >= wiz_level(euid)))
	return 1;

    return 0;
}

