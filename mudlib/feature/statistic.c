/*
 *  Package: Statistics
 *  Summary: An entity's statistics, such as health, energy, etc.
 *
 *      If the entity is a character (living object), then the statistics will be used to
 *      calculate the character's current health, such as whether it is exhausted, or even
 *      dead. A living character usually "regenerates" its statistics over time, up to a
 *      heal value. By receiving medical treatment or magical healing, the statistics can
 *      be healed up to the maximum value. By receiving damage, the statistics can be consumed
 *      or damaged, which reduces the current value or the heal value of the statistic.
 *
 *      If the entity is not a character, the statistics will be used for calculation when
 *      a character interacts with it.
 */
#include <ansi.h>
#include <dbase.h>
#include <login.h>
#include <move.h>
#include <statistic.h>

// variables

static int regenerating = 1;

mapping stats_max = ([]); // maximum value of statistics
mapping stats_heal = ([]); // maximum value of statistics that can heal by regeneration
mapping stats_current = ([]); // current value of statistics
mapping stats_notify = ([]); // notification thresholds for statistics

static mapping stats_regenerator = ([]);
static object last_from_ob;
static mapping f_exhausted = ([]);
static mapping f_destroyed = ([]);
static mapping f_notified = ([]);

// implementations

string* query_stat_name() { return mapp(stats_max) ? keys(stats_max) : ({}); }
object last_damage_giver() { return last_from_ob; }
mapping query_exhausted() { return f_exhausted; }
mapping query_destroyed() { return f_destroyed; }
mapping query_notified() { return f_notified; }

void clear_statistic_flags() {
    f_exhausted = ([]);
    f_destroyed = ([]);
    f_notified = ([]);
}


int query_stat (string what) {
    int val;

    if (mapp(stats_current) && !undefinedp(val = stats_current[what]))
        return val;
    if (mapp(stats_heal) && !undefinedp(val = stats_heal[what]))
        return val;
    if (mapp(stats_max) && !undefinedp(val = stats_max[what]))
        return val;
    return 0;
}

void delete_stat(string what) {
    if( mapp(stats_current) )	map_delete(stats_current, what);
    if( mapp(stats_heal) )	map_delete(stats_heal, what);
    if( mapp(stats_max) )	map_delete(stats_max, what);
    if( mapp(stats_notify) )	map_delete(stats_notify, what);
    if( mapp(stats_regenerator) )	map_delete(stats_regenerator, what);
}

mixed query_stat_current(string what)
    { return mapp(stats_current) ? stats_current[what] : 0; }

mixed query_stat_effective(string what)
    { return mapp(stats_heal) ? stats_heal[what] : 0; }

mixed query_stat_maximum(string what)
    { return mapp(stats_max) ? stats_max[what] : 0; }

mixed query_stat_notify(string what)
    { return mapp(stats_notify) ? stats_notify[what] : 0; }

mixed query_stat_regenerate(string what)
    { return mapp(stats_regenerator) ? stats_regenerator[what] : 0; }


int set_stat_current(string what, int val)
    { return mapp(stats_current) ? stats_current[what] = val : 0; }

int set_stat_effective(string what, int val)
    { return mapp(stats_heal) ? stats_heal[what] = val : 0; }

int set_stat_maximum(string what, int val)
    { return mapp(stats_max) ? stats_max[what] = val : 0; }

int set_stat_notify(string what, int val)
    { return mapp(stats_notify) ? stats_notify[what] = val : 0; }

int advance_stat (string what, int val) {
    if (!mapp(stats_max) || undefinedp(stats_max[what]))
        return 0;

    return (stats_max[what] += val);
}

mixed set_stat_regenerate (string what, mixed val) {
    int max;

    if( !(max = query_stat_maximum(what)) ) return 0;

    if (functionp(val))
        return (stats_regenerator[what] = val);

    switch (val) {
    case TYPE_STATIC:
        return (stats_regenerator[what] = 0);

    case TYPE_HEALTH:
        if( undefinedp(query_stat_current(what)) )
            set_stat_current(what, max);
        if( undefinedp(query_stat_effective(what)) )
            set_stat_effective(what, max);
        return (stats_regenerator[what] = (: health_regenerator :) );

    case TYPE_WASTING:
        if( undefinedp(query_stat_current(what)) )
            set_stat_current(what, max);
        if( undefinedp(query_stat_effective(what)) )
            set_stat_effective(what, max);
        return (stats_regenerator[what] = (: wasting_regenerator :) );

    default:
        error("undefined regenerate method.\n");
    }
    return 0;
}

void init_statistic (mapping base) {
    mapping st;
    string s;
    int val;

    if (!mapp(stats_max))
        stats_max = allocate_mapping(NUM_STATISTICS);
    if (!mapp(stats_heal))
        stats_heal = allocate_mapping(NUM_STATISTICS);
    if (!mapp(stats_current))
        stats_current = allocate_mapping(NUM_STATISTICS);
    if (!mapp(stats_notify))
        stats_notify = allocate_mapping(NUM_STATISTICS);
    if (!mapp(stats_regenerator))
        stats_regenerator = allocate_mapping(NUM_STATISTICS);

    if (mapp(base) && sizeof(base)) {
        foreach (s, val in base)
            if (undefinedp(stats_max[s]))
                stats_max[s] = val;
    }
}

// consume_stat()
//
// This is the formal method to let a character 'consume' its statistic
// current value. Once a statistic has been exhausted (i.e. current value
// drop below zero), a flag is set (to the object caused the last consumption)
// and it should be checked in heart_beat to make proper change.
//
// This function also starts regeneration automatically.

varargs int consume_stat(string type, int damage, object who) {
    if (damage < 0)
        error ("damage less than zero.\n");
    if (damage == 0)
        return 0;

    if (!objectp(who)) {
        if (this_player())
            who = this_player();
        else if (previous_object() && previous_object()->is_character())
            who = previous_object();
        else
            who = this_object();
    }

    if (!mapp(stats_current) || undefinedp(stats_current[type]) || f_exhausted[type])
        return 0;

    last_from_ob = who;
    stats_current[type] -= damage;

    if (stats_current[type] < 0) {
        stats_current[type] = 0;
        f_exhausted[type] = who;
    }

    if (who && mapp(stats_notify) && !undefinedp(stats_notify[type])) {
        if (mapp(stats_max) && !undefinedp(stats_max[type])
        &&  stats_max[type] > 0
        &&  stats_current[type]*100/stats_max[type] < stats_notify[type])
            f_notified[type] = who;
    }

    start_regenerate();
    return damage;
}

// damage_stat()
//
// This is the formal method to let a character receive 'damage', which
// reduces the heal value of a statistic. Once the heal value
// of a statistic drops below zero, the statistic is 'destroyed', and a
// flag is set (to the object that causes the last damage). This flag
// should be checked in heart_beat and make proper changes.
//
// This function also starts regeneration automatically.

varargs int damage_stat (string type, int damage, object who) {
    if (damage < 0)
        error("damage less than zero.\n");
    if (damage == 0)
        return 0;

    if (!objectp(who)) {
        if (this_player())
            who = this_player();
        else if (previous_object() && previous_object()->is_character())
            who = previous_object();
        else
            who = this_object();
    }

    if (!mapp(stats_heal) || undefinedp(stats_heal[type]) || f_destroyed[type])
        return 0;

    last_from_ob = who;
    stats_heal[type] -= damage;

    if (mapp(stats_current) && stats_current[type] > stats_heal[type])
        stats_current[type] = stats_heal[type];

    if (stats_heal[type] < 0) {
        stats_heal[type] = 0;
        f_destroyed[type] = who;
    }
    else if (who && mapp(stats_notify) && !undefinedp(stats_notify[type])) {
        if (mapp(stats_max) && !undefinedp(stats_max[type])
        &&  stats_max[type] > 0
        &&  stats_heal[type]*100/stats_max[type] < stats_notify[type])
            f_notified[type] = who;
    }

    start_regenerate();
    return damage;
}

// supplement_stat()
//
// This is the formal method to supplement statistics of a character.
// Note supplement can fill a statistic's current value up to its
// heal value.
//
// This function also starts regenerating automatically.

int supplement_stat(string type, int heal) {
    int old_stat;

    if (heal < 0)
        error ("heal less than zero.\n");

    if (!mapp(stats_current) || undefinedp(stats_current[type]))
        return 0;

    old_stat = stats_current[type];
    stats_current[type] += heal;

    if (mapp(stats_max) && stats_current[type] > stats_max[type])
        stats_current[type] = stats_max[type];

    if (mapp(stats_heal) && stats_current[type] > stats_heal[type])
        stats_current[type] = stats_heal[type];

    start_regenerate();
    return stats_current[type] - old_stat;
}

// heal_stat()
//
// This is the formal method to heal a character's statistic. Healing
// a statistic can fill its heal value up to its maximum.
//
// This function also starts regenerating automatically.

int heal_stat(string type, int heal) {
    int old_stat;

    if (heal < 0)
        error("heal less than zero.\n");

    if (!mapp(stats_heal) || undefinedp(stats_heal[type]))
        return 0;

    old_stat = stats_heal[type];
    stats_heal[type] += heal;

    if (mapp(stats_max) && stats_heal[type] > stats_max[type])
        stats_heal[type] = stats_max[type];

    start_regenerate();
    return stats_heal[type] - old_stat;
}

// start_regenerate()
//
// This function starts regenerating manually. The regenerating is on by
// default when characters inherits F_STATISTIC was created. It can be
// turned off by regenerate() eventually. If you want force regenerating
// to start, call this function.

void start_regenerate() { regenerating = 1; }

int regenerate() {
    string st;
    mixed regenerator;
    int n_updated = 0;

    if (!regenerating)
        return 0;

    if (!mapp(stats_regenerator) ||	!mapp(stats_max))
        return 0;

    foreach (st, regenerator in stats_regenerator) {
        if (evaluate (regenerator,
            this_object(),
            st,
            stats_max[st],
            mapp(stats_heal) ? stats_heal[st] : 0,
            mapp(stats_current) ? stats_current[st] : 0)) {
            n_updated++;
        }
        if (mapp(stats_heal) && stats_heal[st] < 0 && !f_destroyed[st])
            f_destroyed[st] = this_object();
        if (mapp(stats_current) && stats_current[st] < 0 && !f_exhausted[st])
            f_exhausted[st] = this_object();
    }

    return regenerating = (n_updated > 0);
}

// The following functions are default regenerator of statistics. You
// can either use them, or just took them as examples and write your
// own.

int health_regenerator (object me, string stat, int max, int heal, int cur) {
    if (heal <= 0)
        return 0;

    // Generation is stopped when the character is fighting, or has no water,
    // or is over encumbranced.
    if (userp(me)) {
        if (me->is_fighting()
        ||  (int)me->query_stat("water") < 1
        ||  me->over_encumbranced())
            return 0;
    }

    if (cur < heal) {
        switch (stat) {
        case "gin":
            return me->supplement_stat(stat, (int)me->query_attr("dex") );
        case "kee":
            return me->supplement_stat(stat, (int)me->query_attr("con") );
        case "sen":
            return me->supplement_stat(stat, (int)me->query_attr("spi") );
        case "HP":
            return me->supplement_stat(stat, 1 );
        default:
            return me->supplement_stat(stat, (int)me->query_level() );
        }
    }

    // Slowly heal if the character is well fed.
    if (userp(me) && ((int)me->query_stat("food") < 1))
        return 0;

    if (heal < max) {
        switch (stat) {
        case "gin":
            return me->heal_stat(stat, (int)me->query_attr("dex") / 6 + 1 );
        case "kee":
            return me->heal_stat(stat, (int)me->query_attr("con") / 6 + 1);
        case "sen":
            return me->heal_stat(stat, (int)me->query_attr("spi") / 6 + 1);
        case "HP":
            return me->heal_stat(stat, 1 );
        default:
            return me->heal_stat(stat, (int)me->query_level() / 6 + 1);
        }
    }

    return 0;
}

int wasting_regenerator(object me, string stat, int max, int heal, int cur) {
    if (!userp(me))
        return 0;
    return me->consume_stat (stat, 1 + (me->is_fighting() ? 2 : 0), me);
}


