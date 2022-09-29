/*  drink.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <dbase.h>
#include <name.h>

// Any drink should either inherit LIQUID or define these functions.
int query_volume();
void add_volume(int);

int drink_ob(object me)
{
    int stuff, heal;
    float alcohol;

    stuff = me->query_stat_maximum("water") / 3;
    if( stuff + me->query_stat("water") > me->query_stat_maximum("water") )
        stuff = me->query_stat_maximum("water") - me->query_stat("water");
    if( !stuff ) return notify_fail("你已經漲得喝不下一滴水了。\n");
/*
    if( stuff > query_volume()/10 )
        stuff = query_volume()/10;
*/
    if( !stuff ) return 0;

    me->supplement_stat("water", stuff);

    message_vision("$N從" + environment()->name() + "喝了幾口" + name() + "。\n", me);

    if( (alcohol = query("alcohol")) > 0 )
        CONDITION_D("drunk")->apply_drunk( me, to_int( alcohol * stuff ));

    // add_volume could destruct ourself once volume becomes zero, call it
    // last! (by Annihilator 01/18/2000)
    add_volume(-stuff*10);
    return 1;
}
