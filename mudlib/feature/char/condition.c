#include <condition.h>

mapping conditions = ([]);

nomask void
update_condition()
{
    string cnd;
    mixed cnd_data;

    if( !mapp(conditions) || !sizeof(conditions) ) return;

    foreach(cnd, cnd_data in conditions)
	CONDITION_D(cnd)->condition_update(this_object(), cnd, cnd_data);
}

static nomask void
restore_condition()
{
    string cnd;
    mixed cnd_data;

    if( !mapp(conditions) || !sizeof(conditions) ) return;

    foreach(cnd, cnd_data in conditions)
	CONDITION_D(cnd)->condition_apply(this_object(), cnd, cnd_data);
}

nomask void
clear_condition()
{
    string cnd;
    mixed cnd_data;

    if( !mapp(conditions) || !sizeof(conditions) ) return;

    foreach(cnd, cnd_data in conditions)
	CONDITION_D(cnd)->condition_unapply(this_object(), cnd);

    conditions = ([]);
}

nomask void
set_condition(string cnd, mixed info)
{
    if( !mapp(conditions) ) conditions = ([]);


    if( conditions[cnd] )
	CONDITION_D(cnd)->condition_unapply(this_object(), cnd,
		conditions[cnd]);

    conditions[cnd] = info;
    CONDITION_D(cnd)->condition_apply(this_object(), cnd, info);
}

nomask mixed
query_condition(string cnd)
{
    return mapp(conditions) ? conditions[cnd] : 0;
}

nomask void
delete_condition(string cnd)
{
    if( !mapp(conditions) || undefinedp(conditions[cnd]) ) return;

    CONDITION_D(cnd)->condition_unapply(this_object(), cnd);
    map_delete(conditions, cnd);
}
