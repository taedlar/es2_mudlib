/*  path.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

string user_cwd(string name)
{
    return sprintf("/u/%c/%s", name[0], name);
}

string user_home(string name)
{
    return sprintf("/u/%c/%s", name[0], name);
}

string user_path(string name)
{
    return (user_cwd(name) + "/");
}

string user_data(string name)
{
    return stringp(name) ?
	sprintf("%suser/%c/%s.o", DATA_DIR, name[0], name) : 0;
}

string login_data(string name)
{
    return stringp(name) ?
	sprintf("%slogin/%c/%s.o", DATA_DIR, name[0], name) : 0;
}

string mail_data(string name)
{
    return stringp(name) ?
	sprintf("%smail/%c/%s.o", DATA_DIR, name[0], name) : 0;
}

string
resolve_path(string cwd, string new_path)
{
    int i;
    string username, *comp;
    string t1,t2,t3,t4;

    // Shortcuts for fast resolve
    if( this_player() ) {
	switch(new_path) {
	case "here" : return file_name(environment(this_player())) + ".c";
	case "it" : return this_player()->query("cwf");
	default: break;
	}
    }
    if( !cwd ) cwd = "/";
    if( !new_path || new_path=="." ) return cwd;
    if( new_path[0]=='/' ) return new_path;

    comp = explode(new_path, "/");

    // User home prefixed.
    if( comp[0][0]=='~' ) {
	if( comp[0]=="~" ) return user_home( geteuid(this_player()) );
	cwd = user_home( geteuid(this_player()) );
	comp = comp[1..];
    }

    comp = explode(cwd, "/") + comp;
    comp -= ({ "" });

    for( i=0; i < sizeof(comp); i++)
	if (comp[i] == "..") {
	    if (sizeof(comp) > 2) {
		comp = comp[0..(i-2)] + comp[(i+1)..(sizeof(comp)-1)];
		i -= 2;
	    } else {
		comp = comp[2 ..(sizeof(comp)-1)];
		i = 0;
	    }
	}

     new_path = "/" + implode(comp,"/");

     if (new_path == "//") new_path = "/";
     return new_path;
}
