/*  fingerd.c - finger daemon

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#pragma save_binary

//#include <net/dns.h>
#include <origin.h>
#include <ansi.h>

inherit F_CLEAN_UP;

private object acquire_login_ob(string id);

private void create() { seteuid(getuid()); }

string
age_string(int time)
{
    int month, day, hour;

    time /= 3600;
    hour = time % 24;
    time /= 24;
    day = time % 30;
    month = time / 30;
    return (month ? month + "m" : "") + (day ? day + "d" : "") + hour + "h";
}

string
finger_all()
{
    object *ob;
    string msg;
    int i;

    ob = sort_array( users(), (: strcmp(query_ip_number($1), query_ip_number($2)) :) );
    msg = "    NAME   |    ID     |              DNS             |   AGE  | LEVEL\n";
    for(i=0; i<sizeof(ob); i++) {
	if( this_player() && !ob[i]->visible(this_player()) ) continue;
	msg = sprintf("%s%-12s%+12s %+30s %+8s %2d\n",
	    msg,
	    ob[i]->query("name"),
	    ob[i]->query("id"),
	    query_ip_name(ob[i]),
	    ob[i]->link() ? age_string( ob[i]->link()->query("time_aged")) : "n/a",
	    ob[i]->query("level"),
	);
    }
    return msg;
}

string
finger_user(string name)
{
    object ob, body;
    string msg, mud;
    int invis, age, wiz_mode = 0;

#if 0
    if( sscanf(name, "%s@%s", name, mud)==2 ) {
	GFINGER_Q->send_gfinger_q(mud, name, this_player(1));
	return "網路指令傳送過程可能需要一些時間﹐請稍候。\n";
    }
#endif

    ob = acquire_login_ob(name);
    if( !ob ) return "沒有這位使用者。\n";
    body = find_player(name);

    if( this_player(1)
    && (wizardp(this_player(1)) || body==this_player(1)) ) wiz_mode = 1;

    age = ob->query("time_aged");
    invis = ob->query("invis") && (!body || !this_player()
	|| wiz_level(body) > wiz_level(this_player()));

    // higher level wiz can use invis.c to be invisible to lowers
    msg =  sprintf("英文代號：%-16s姓    名：%s\n權限等級：%-16s"
	"電子郵件：%s\n",
	capitalize(ob->query("id")),
	ob->query("name"),
	wizhood(name),
	wiz_mode ? ob->query("email") : "<限巫師查詢>" );

    if( body && !invis && objectp(body) ) {
	if( interactive(body) ) {
	    msg += sprintf("\n%s正從 %s 連線中，連線時數%s。\n",
		gender_pronoun(body),
		wiz_mode ? query_ip_name(body) : "<某處>",
		chinese_period(time() - ob->query_temp("login_time")) );
	    msg += sprintf("角色狀態：等級 %d %s\n",
		body->query_level(), body->rank() );
	}
	else msg += "\n" + gender_pronoun(body) + "現在斷線中。\n";
    }

    msg += sprintf("\n上次連線：%s (%s)\n",
	ob->query("last_from"), ctime(ob->query("last_on")));

    if( invis ) age -= time() - ob->query_temp("login_time");
    msg += sprintf("時數總和：%s\n", chinese_period(age));

    if( !ob->body() ) destruct(ob);

    return msg;
}

varargs string
remote_finger_user(string name, int chinese_flag)
{
    object ob, body;
    string msg;

    ob = acquire_login_ob(name);
    if( !ob ) return chinese_flag ? "沒有這個玩家。\n" : "No such user.\n";
    if( chinese_flag ) msg =  sprintf(
	"\n英文代號﹕\t%s\n姓    名﹕\t%s\n權限等級﹕\t%s\n"
	"電子郵件地址﹕\t%s\n上次連線地址﹕\t%s( %s )\n\n",
	ob->query("id"),
	ob->query("name"),
	SECURITY_D->get_status(name),
	ob->query("email"),
	ob->query("last_from"),
	ctime(ob->query("last_on"))
    );
    else msg =  sprintf(
	"\nName\t: %s\nStatus\t: %s\nEmail\t: %s\nLastOn\t: %s( %s )\n\n",
	capitalize(ob->query("id")),
	SECURITY_D->get_status(name),
	ob->query("email"),
	ob->query("last_from"),
	ctime(ob->query("last_on"))
    );
    if( body = find_player(name) ) {
	if( !this_player() || this_player()->visible(body) )
	    msg += chinese_flag ?
		("\n" + ob->query("name") + "目前正在線上。\n"):
		("\n" + capitalize(name) + " is currently connected.\n");
    }

    if( !ob->body() ) destruct(ob);
    return msg;
}

object
acquire_login_ob(string id)
{
    object ob;

    if( origin()==ORIGIN_CALL_OTHER ) {
	if( geteuid(previous_object())!=ROOT_UID
	&&  base_name(previous_object())!=MAILBOX_OB ) return 0;
    }

    if( ob = find_player(id) ) {
	// Check if the player is linkdead
	if( ob->link() ) return ob->link();
    }

    ob = new(LOGIN_OB);
    ob->set("id", id);
    return ob->restore() ? ob : 0;
}
