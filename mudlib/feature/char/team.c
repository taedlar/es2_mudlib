/*  team.c

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
#include <dbase.h>
#include <origin.h>
#include <command.h>

void do_chat(mixed);
object query_charging();
varargs int is_fighting(object);
void clean_up_enemy();
void reset_time_unit();

static object leader, *team;

int set_leader(object ob)
{
    if( ob==this_object() )
        error("set_leader: Cannot set this_object() as leader.\n");
    leader = ob;
}

object query_leader() { return leader; }

// follow_me()
//
// this function is called by "go" command when a character leaves a room.
int follow_me(object ob, string dir)
{
    if( ((ob != leader) && (ob != query_charging()))
    ||	!living(this_object())
    ||	!living(ob) )
	return 0;

    clean_up_enemy();

    if( command("go " + dir) ) return 1;
    tell_object(this_object(), HIG "你跟丟" + ob->name() + "了。\n" NOR);
    return 0;
}

int add_team_member(object ob)
{
    object member;

    if( ob==this_object() ) return 0;

    if( arrayp(team) ) {
	if( member_array(ob, team)==-1 ) team += ({ ob });
    } else
        team = ({ this_object(), ob });

    foreach(member in team) member->set_team(team);

    return 1;
}

int
is_team_leader()
{
    return arrayp(team) && team[0]==this_object();
}

int
set_team(mixed t)
{
    team = t;
}

int
dismiss_team()
{
    int i;
    object member;

    if( !arrayp(team) ) return 0;
    
    if( team[0]==this_object() ) {
	/* 這個物件是領隊：解散整個隊伍 */
	foreach(member in team) member->set_team(0);
	team = 0;
    } else {
	/* 這個物件不是領隊：離開隊伍 */
	team -= ({ this_object() });
	foreach(member in team) member->set_team(team);
	team = 0;
    }

    return 1;
}

object*
query_team()
{
    return team;
}

