#include <login.h>
#include <origin.h>
#include <user.h>

inherit F_ALIAS;
inherit F_DBASE;
inherit F_SAVE;

void update_age();

static object my_body;
static int last_age_set;

string password;
string email;

object body() {
    return my_body;
}

int clean_up() {
    if (!my_body)
        destruct(this_object());
    return 1;
}

private void logon () {
    seteuid (getuid());

    if (load_object(LOGIN_D)) {
        call_out ("time_out", LOGIN_TIMEOUT);
        set_temp ("login_time", time());
        last_age_set = time();
        seteuid (0);
        LOGIN_D->logon (this_object());
        return;
    }
    receive ("系統登入功能整修中，請稍後再嘗試連線。\n");
    destruct (this_object());
}

private void net_dead() {
    destruct (this_object());
}

private void time_out() {
    object body;

    if (objectp(my_body))
        return;

    if (interactive(this_object()))
	    write ("您花在連線進入手續的時間太久了﹐下次想好再來吧。\n");

    // If we go linkdead before we finish character creation, destruct
    // the body as well. 
    if (objectp (body = query_temp ("temp_body")) && !userp (body))
        destruct (body);

    destruct (this_object());
}

// This is used by F_SAVE to determinethe file name to save our data.
nomask string query_save_file() {
    string id;

    id = query ("id", 1);
    if (!stringp (id))
        return 0;
    return login_data (id);
}

private void receive_message (string type, string str) {
    if (!interactive (this_object()))
        return;
    if (str)
        receive (str);
}

private void terminal_type(string term_type) {
    set_temp ("terminal_type", term_type);
}

void set_body(object body) {
    if (geteuid(previous_object()) != ROOT_UID)
        return 0;

    my_body = body;
    seteuid (getuid());
}

void update_age() {
    if (!last_age_set)
        last_age_set = time();
    ::add ("time_aged", time() - last_age_set);
    last_age_set = time();
}

nomask mixed set(string prop, mixed data) {
    USER_PROTECT();

    switch (prop) {
    case "password":
	    return password = data;
    case "email":
	    return email = data;
    default:
	    return ::set (prop, data);
    }
}

nomask mixed add(string prop, mixed data) {
    USER_PROTECT();
    return ::add (prop, data);
}

nomask mixed delete(string prop) {
    USER_PROTECT();
    return ::delete (prop);
}

nomask varargs mixed query(string prop, int raw) {
    mixed data;

    switch (prop) {
    case "password":
	    if (!password)
            password = ::query ("password", 1);
	    return password;
    case "email":
	    if (!email)
            email = ::query ("email", 1);
	    return email;
    default:
	    data = ::query (prop, raw);
	    break;
    }
    
    if (mapp (data) || arrayp (data))
        return copy (data);

    return data;
}

nomask int restore() {
    seteuid (getuid());
    if (! ::restore())
        return 0;

    if (!password)
        password = ::query ("password");
    if (!email)
        email = ::query ("email");

    return 1;
}
