/*  traveler.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

// 這個 daemon 用來管理一些會隨機在各區域間「旅行」的 NPC 及相關事務。

inherit F_CLEAN_UP;
inherit F_SAVE;

mapping travel_site = ([]);

private void
create()
{
    seteuid(getuid());
    restore();
}

string query_save_file() { return DATA_DIR + "daemon/traveler"; }

void remove()
{
    save();
}

void register(string* neighbor)
{
    object new_site = previous_object();

    if( clonep(new_site)
    ||	geteuid(new_site) != DOMAIN_UID ) return;

    travel_site[base_name(new_site)] = neighbor;
}

void unregister()
{
    map_delete(travel_site, base_name(previous_object()));
}

string request_travel_site()
{
    object traveler;
    string where_am_i;
    string *destination;

    traveler = previous_object();
    if( ! environment(traveler) ) return 0;

    where_am_i = base_name( environment(traveler) );

    if(	!arrayp(travel_site[where_am_i]) )
        destination = keys(travel_site);
    else
        destination = travel_site[where_am_i];

    destination -= ({ where_am_i });

    return sizeof(destination) ? destination[random(sizeof(destination))] : 0;
}
