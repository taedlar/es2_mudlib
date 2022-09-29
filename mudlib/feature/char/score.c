/*  score.c

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
#include <login.h>

string query_class() { return query("class"); }
string query_race() { return query("race"); }
int query_level() {    return query("level"); }

void set_class(string new_class)
{
    int lvl;

    set("class", new_class);
    if( query_level() && query_race() ) {
	delete("target_score");
	RACE_D(query_race())->initialize(this_object());
	CLASS_D(new_class)->initialize(this_object());
    }
}

void set_race(string new_race)
{
    int lvl;

    set("race", new_race);
    if( query_level() && query_class() ) {
	delete("target_score");
	RACE_D(new_race)->initialize(this_object());
	CLASS_D(query_class())->initialize(this_object());;
    }
}

void set_level(int lvl)
{
    string s;

    if( lvl < 1 ) error("character level must be at least 1.\n");
    if( lvl > 999 ) error("character level must less than 999.\n");
    set("level", lvl);
    if( query_race() && query_class() ) {
	delete("target_score");
	RACE_D(query_race())->initialize(this_object());
	CLASS_D(query_class())->initialize(this_object());
    }
}

int query_score(string course)
{
    return query("score/" + course);
}

static mapping score_gain = ([]);
mapping query_score_gain() { return score_gain; }
void reset_score_gain() { score_gain = ([]); }

varargs void
gain_score(string course, int xp)
{
    mapping sc, targ_sc;
    string s, save_file;
    int v;

    add("score/" + course, xp);
    if( undefinedp(score_gain[course]) ) score_gain[course] = xp;
    else score_gain[course] += xp;

    // Check if we can raise a level.
    sc = query("score");
    targ_sc = query("target_score");
    if( !mapp(sc) || !mapp(targ_sc) ) return;

    foreach(s, v in targ_sc)
	if( sc[s] < v ) return;

    receive( HIY "你的等級提昇了﹗\n" NOR );
    RACE_D(query_race())->advance_level(this_object());
    CLASS_D(query_class())->advance_level(this_object());
    delete("praise_done");
    add("level", 1);

    seteuid(getuid());
// 系統強制在玩家升級時做save及backup -Dragoon
#ifdef SAVE_USER
    this_object()->save();
    save_file = this_object()->query_save_file();
    cp(save_file, save_file+".backup");
#endif
    receive("檔案儲存及備份完畢。\n");
}

int query_target_score(string course)
{
    return query("target_score/" + course);
}

void set_target_score(string course, int xp)
{
    if( !query("score/" + course) ) set("score/" + course, 0);
    if( xp < (int)query("target_score/" + course) ) return;
    set("target_score/" + course, xp);
}

