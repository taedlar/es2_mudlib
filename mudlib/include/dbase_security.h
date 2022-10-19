// dbase_security.h

#include <origin.h>

nomask mixed
set(string prop, mixed data)
{
    if( origin()==ORIGIN_CALL_OTHER
    &&	(geteuid(previous_object()) != ROOT_UID)
    &&	(geteuid(previous_object()) != geteuid(this_object())) )
	error("security violation by " + geteuid(previous_object())
		+ ", Root or owner euid required.\n");

    return ::set(prop, data);
}

nomask mixed
add(string prop, mixed data)
{
    if( origin()==ORIGIN_CALL_OTHER
    &&	(geteuid(previous_object()) != ROOT_UID)
    &&	(geteuid(previous_object()) != geteuid(this_object())) ) 
	error("security violation by " + geteuid(previous_object())
		+ ", Root or owner euid required.\n");

    return ::add(prop, data);
}

nomask mixed
delete(string prop)
{
    if( origin()==ORIGIN_CALL_OTHER
    &&	(geteuid(previous_object()) != ROOT_UID)
    &&	(geteuid(previous_object()) != geteuid(this_object())) ) 
	error("security violation by " + geteuid(previous_object())
		+ ", Root or owner euid required.\n");

    return ::delete(prop);
}

nomask varargs mixed
query(string prop, int raw)
{
    mixed data;

    data = ::query(prop, raw);

    // protect from direct access to pointer semantic data to
    // eliminate security violation of side effect.
    
    if( (mapp(data) || arrayp(data) )
    &&	origin()==ORIGIN_CALL_OTHER
    &&	(geteuid(previous_object()) != ROOT_UID)
    &&	(geteuid(previous_object()) != geteuid(this_object())) ) 
	error("security violation by " + geteuid(previous_object()) + ", Root or owner euid required.\n");

    return data;
}

