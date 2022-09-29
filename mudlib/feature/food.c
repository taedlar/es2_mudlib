/*  food.c

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

int stuff_ob(object me)
{
    int stuff, heal;

    stuff = query("food_stuff");
    if( !stuff ) return 0;

    if( stuff + me->query_stat("food") > me->query_stat_maximum("food") )
        return notify_fail("你的肚子已經裝不下這" + query("unit") + name() + "了。\n");

    me->supplement_stat("food", stuff);
    message_vision("$N吃下一" + query( this_object()->query_amount() ? "base_unit" : "unit") + name() + "。\n", me);

    if( heal = query("heal_gin") 
    &&    me->supplement_stat("gin", heal) )
        write("你吃了營養的食物﹐恢復了一些體力。\n");

    if( heal = query("heal_kee") 
    &&    me->supplement_stat("kee", heal) )
        write("你吃了營養的食物﹐覺得氣力恢復了一些。\n");

    if( heal = query("heal_sen") 
    &&    me->supplement_stat("sen", heal) )
        write( "你覺得清醒多了。\n");

    if( this_object()->query_amount() )
        this_object()->add_amount(-1);
    else
        destruct(this_object());
    return 1;
}
