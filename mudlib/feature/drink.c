// vim: set ts=4 sw=4 syntax=lpc

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
// vim: set ts=4 sw=4 syntax=lpc
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

