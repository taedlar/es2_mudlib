/*  natured.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <localtime.h>

#define TIME_TICK (time()*60)

static int current_day_phase;
mapping *day_phase;

string *weather_msg = ({
    "天空中萬里無雲",
    "幾朵淡淡的雲彩妝點著清朗的天空",
    "白雲在天空中飄來飄去",
    "厚厚的雲層堆積在天邊",
    "天空中烏雲密佈",
});

private mapping *read_table(string file);
private void init_day_phase();

private void
create()
{
    seteuid (MUDLIB_UID);
    day_phase = read_table("/adm/etc/nature/day_phase");
    init_day_phase();
}

private void
init_day_phase()
{
    mixed *local;
    int i, t;

    // Get minutes of today.
    local = localtime(TIME_TICK);
    t = local[LT_HOUR] * 60 + local[LT_MIN];	// hour * 60 + minutes

    // Find the day phase for now.
    i = 0;
    foreach(mapping phase in day_phase) {
	if( t < phase["length"] ) break;
	t -= (int)phase["length"];
	i++;
    }

    current_day_phase = (i==0? sizeof(day_phase)-1: i - 1);

    // Since in our time scale, 1 minute == 1 second in RL, so we don't need
    // any conversion before using this number as call_out dalay, if you 
    // changed the time scale, be sure to convert it.

    call_out("update_day_phase",
        (int)day_phase[(current_day_phase+1) % sizeof(day_phase)]["length"] - t);
}

private void
update_day_phase()
{
    object* rcpt;

    remove_call_out("update_day_phase");

    current_day_phase = (++current_day_phase) % sizeof(day_phase);

    rcpt = filter( users(), (: objectp($1) && interactive($1)
	&& environment($1) && environment($1)->query("outdoors") :));
    message("outdoor:vision",
	day_phase[current_day_phase]["time_msg"] + "\n", rcpt);
#if 0
    if( !undefinedp(day_phase[current_day_phase]["event_fun"]) )
	call_other(this_object(), day_phase[current_day_phase]["event_fun"]);
#endif

    call_out("update_day_phase", day_phase[current_day_phase]["length"]);
}

string outdoor_room_description()
{
    return "    " + day_phase[current_day_phase]["desc_msg"] + "。\n";
}

varargs mixed
game_time(int flag)
{
    if( flag ) return localtime(TIME_TICK);
    else return CHINESE_D->chinese_date(TIME_TICK);
}

// This function is to read a regular type of data table for day_phase and
// etc.
private mapping *
read_table(string file)
{
    string *line, *field, *format;
    mapping *data;
    int i, rn, fn;

    line = explode(read_file(file), "\n");
    data = ({});
    for(i=0; i<sizeof(line); i++) {
        if( line[i]=="" || line[i][0]=='#' ) continue;
        if( !pointerp(field) ) {
            field = explode( line[i], ":" );
            continue;
        }
        if( !pointerp(format) ) {
            format = explode( line[i], ":" );
            continue;
        }
        break;
    }

    for( rn = 0, fn = 0; i<sizeof(line); i++) {
        if( line[i]=="" || line[i][0]=='#' ) continue;
        if( !fn ) data += ({ allocate_mapping(sizeof(field)) });
        sscanf( line[i], format[fn], data[rn][field[fn]] );
        fn = (++fn) % sizeof(field);
        if( !fn ) ++rn;
    }
    return data;
}


mapping *query_day_phase() { return day_phase; }

void
remove()
{
    remove_call_out();
}
