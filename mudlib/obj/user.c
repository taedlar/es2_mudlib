/*  user.c - the user body object

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <ansi.h>
#include <origin.h>
#include <user.h>
#include <action.h>

inherit CHARACTER;
inherit F_AUTOLOAD;
inherit F_EDIT;
inherit F_SAVE;
inherit F_MORE;
inherit F_FLAG;

// private prototypes

private void user_dump(int type);

// variables

static object my_link;
static int last_age_set;

// implementations

void
create()
{
    object ob;

    // Let LOGIN_D export proper uid to us.
    seteuid(0);

    if( clonep(this_object()) )
	set_name("無名氏", "noname");
}

private int
move_or_destruct( object dest )
{
    if( dest ) move(dest);
    else {
	message("system", "一陣時空的扭曲將你傳送到另一個地方....\n", this_object());
	set_temp("last_location", base_name(environment()));
	move(VOID_OB);
    }
}

private void
reset()
{
    // To prevent the master copy of user object error out by security
    // violation.
    if( !userp(this_object()) ) return;

    gain_score("survive", (int)query("age") / 2 + 1 );
}

int set_link(object link)
{
    if( origin()==ORIGIN_CALL_OTHER
    &&	(geteuid(previous_object()) != ROOT_UID)
    &&	(geteuid(previous_object()) != geteuid(this_object())) )
	return 0;
    my_link = link;
    return 1;
}

object link() { return my_link; }

void terminal_type(string term_type)
{
    set_temp("terminal_type", term_type);
    message("system", "終端機型態設定為 " + term_type + "。\n", this_object());
}

// This is used by F_SAVE to determine the filename to save our data.
string query_save_file()
{
    string id;

    id = geteuid();
    if( !id ) id = getuid();

    return user_data(id);
}

int save()
{
    int res;

    if( objectp(my_link) ) my_link->save();
    save_autoload();
    res = ::save();
    clean_up_autoload();        // To save memory
    return res;
}

// This function updates player's age, called by heart_beat()
private void
heart_beat()
{
    ::heart_beat();
    if( !this_object() ) return;

    // Make us older. Note this should not rely on fixed period between
    // each call to heart_beat bcz heart_beat could be stoped for various
    // reason.                                           - Annihilator

    if( !last_age_set ) last_age_set = time();
    add("time_aged", time() - last_age_set);
    last_age_set = time();
    if( (int)query("time_aged") >= 86400 ) {
	add("age", 1);
	delete("time_aged");
    }

    if( objectp(my_link) ) my_link->update_age();

    if( interactive(this_object())
    &&	query_idle(this_object()) >= IDLE_TIMEOUT )
	user_dump(DUMP_IDLE);
}

void
setup()
{
    if( ! query("id") ) set("id", getuid());
    ::setup();

    /* 以下是讓 F_STATISTIC 檢查這個使用者是否在上次 quit 之前已經死亡，但
     * 還沒有被 heart_beat 抓到( 俗稱 quit 大法 )
     */
    damage_stat("gin", 0);
    damage_stat("kee", 0);
    damage_stat("sen", 0);
    damage_stat("HP", 0);

    restore_autoload();
    restore_condition();
}

private void
user_dump(int type)
{
    switch(type)
    {
    case DUMP_NET_DEAD:
	// change cmd: quit to quit ! -Dragoon
	command("quit !");
	break;
    case DUMP_IDLE:
	// wiz don't get kicked out (Elon 10-17-95)
	if (wizardp(this_object())) return;
	tell_object( this_object(), "對不起﹐您已經發呆超過 " 
	    + IDLE_TIMEOUT/60 + " 分鐘了﹐請下次再來。\n");
	tell_room( environment(), "一陣風吹來﹐將發呆中的" + query("name")
	    + "化為一堆飛灰﹐消失了。\n", ({this_object()}));
	// change cmd: quit to quit ! -Dragoon
	command("quit !");
	break;
    default:
	return;
    }
}

/*  net_dead()
 *
 *  當使用者斷線時，driver 會呼叫這個函數。
 */
private void
net_dead()
{
    set_heart_beat(0);

    if( objectp(my_link) ) {
	my_link->save();
	destruct(my_link);
    }

    // Stop fighting, we'll be back soon.
    remove_all_enemy();
    if( environment() )
	set_temp("last_location", base_name(environment()) );

    if( is_busy() ) interrupt(this_object(), INTR_LINKDEAD);

    if( userp(this_object()) ) {
	call_out("user_dump", NET_DEAD_TIMEOUT, DUMP_NET_DEAD);
	// tell room what happen to this player -dragoon
	tell_room(environment(), "時空一陣波動, " + query("name") +
	    "瞬間由這個世界消失...\n");
	LOGIN_D->net_dead(this_object());
    } else 
	command("quit");
}

// reconnect: called by the LOGIN_D when a netdead player reconnects.
void
reconnect()
{
    string last_loc;

    if( geteuid(previous_object()) != ROOT_UID )
	error("Permission denied.\n");

    set_heart_beat(1);
    remove_call_out("user_dump");
    tell_object(this_object(), "重新連線完畢。\n");
    if( stringp(last_loc = query_temp("last_location")) )
	move(last_loc);
}

static string last_cmd;
static int last_cmd_time;
static int last_cmd_count;

string
process_input(string str)
{
#ifdef	MAX_COMMAND_PER_SECOND
    if( last_cmd_time == time() ) {
	last_cmd_count++;
	if( last_cmd_count > MAX_COMMAND_PER_SECOND ) return "";
	if( last_cmd_count == MAX_COMMAND_PER_SECOND ) {
	write("\r你下指令的速度太快了，你所控制的人物無法執行這麼多命令。\n");
	    return "";
	}
    } else {
	last_cmd_time = time();
	last_cmd_count = 1;
    }
#endif	/* MAX_COMMAND_PER_SECOND */

    if( str[0..1]=="!!" )
	return last_cmd + str[2..];

    if( objectp(my_link) )
	return last_cmd = my_link->process_alias(str);

    return 0;
}

void
lose_fight(object opponent)
{
    ::lose_fight(opponent);
    tell_object(this_object(), "這一招你無法招架，輸掉了這場比試。\n");
}

void
win_fight(object opponent)
{
    tell_object(this_object(), opponent->name() + "招架不住，你贏了這場比試。\n");
    // add to test -Dragoon
    remove_all_enemy();
}

/***********************************************************************
                            USER DATA PROTECTIONS
 ***********************************************************************/
 
nomask mixed
set(string prop, mixed data)
{
    USER_PROTECT();
    return ::set(prop, data);
}

nomask mixed
add(string prop, mixed data)
{
    USER_PROTECT();
    return ::add(prop, data);
}

nomask void
delete(string prop)
{
    USER_PROTECT();
    ::delete(prop);
}

nomask varargs mixed
query(string prop, int raw)
{
    mixed v = ::query(prop, raw);
    if( mapp(v) || arrayp(v) ) USER_PROTECT();
    return v;
}

nomask void
set_skill(string skill, int amount)
{
    USER_PROTECT();
    ::set_skill(skill, amount);
}

nomask void
advance_skill(string skill, int amount)
{
    USER_PROTECT();
    ::advance_skill(skill, amount);
}

nomask void
set_learn(string skill, int lrn)
{
    USER_PROTECT();    
    ::set_learn(skill, lrn);
}

nomask void
improve_skill(string skill, int amount)
{
    USER_PROTECT();
    ::improve_skill(skill, amount);
}

nomask int
set_attr(string what, int value)
{
    USER_PROTECT();
    return ::set_attr(what, value);
}

nomask int
set_stat_maximum(string what, int val)
{
    USER_PROTECT();
    return ::set_stat_maximum(what, val);
}
