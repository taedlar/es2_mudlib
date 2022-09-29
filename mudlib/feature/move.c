/*  move.c - supply moving ability for an object

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#pragma optimize

#include <dbase.h>
#include <type.h>

static int weight = 0;
static int encumb = 0, max_encumb = 0, max_inventory = -1;

nomask int weight() { return weight + encumb; }
nomask int query_weight() { return weight; }
nomask int query_encumbrance() { return encumb; }
nomask int query_max_encumbrance() { return max_encumb; }
nomask int query_max_inventory() { return max_inventory; }
nomask void set_max_encumbrance(int e) { max_encumb = e; }
nomask void set_max_inventory(int i) { max_inventory = i; }

nomask void
add_encumbrance(int w)
{
    encumb += w;
    if( encumb < 0 ) error("move: encumbrance underflow.\n");
    if( environment() ) environment()->add_encumbrance(w);
}

nomask void
set_weight(int w)
{
    if( !environment() ) {
	weight = w;
	return;
    }
    if( w!=weight ) environment()->add_encumbrance( w - weight );
    weight = w;
}

int
receive_object(object ob, int from_inventory)
{
    if( !from_inventory
    &&	(encumb + ob->weight() > max_encumb) )
	return notify_fail(ob->name() + "太重了。\n");

    if( max_inventory > 0 && sizeof(all_inventory()) >= max_inventory )
	return notify_fail(this_object()->name() + "裝不下了。\n");

    return 1;
}
 
// move()
//
// This function actually moves this_object into another object, after
// checking validity of destination and maintain weight/encumbrance.
varargs int
move(mixed dest, int silently)
{
    mixed err;
    object env;
    int w;

    // If we are equipped, unequip first.
    if( query("equipped") && !this_object()->unequip() )
	return notify_fail("你沒有辦法取下這樣東西。\n");

    // Check validity of dest.
    switch( typeof(dest) ) {
	case OBJECT:
	    break;
        case STRING:
	    err = catch(dest = load_object(dest));
	    if( err ) error("move: error loading " + dest + ":\n" + err);
		break;
        default:
	    error( sprintf("move: Invalid destination, Expected: object or string, Got: %O.\n", dest));
    }

    // Check if the destination is our environment ( or environment of
    // environment ..., recursively ). If so, encumbrance checking is omited.
    env = this_object();
    while( objectp(env = environment(env)) ) if( env==dest ) break;

    if( ! dest ) return 0;
    if( ! dest->receive_object(this_object(), objectp(env)) ) return 0;

    // Move the object and update encumbrance
    if( environment() ) environment()->add_encumbrance( - weight() );
    move_object(dest);

    // The destination might self-destruct in init(), check it before we
    // do environment maintains.
    if( !environment() ) return 0;

    environment()->add_encumbrance( weight() );

    return 1;
}

void
remove(string euid)
{
    /* 把這個物件所有的 call_out 拿掉，因為函數型態的物件在 Owner 消滅
     * 之後就沒有辦法 evaluate 了。
     */
    remove_call_out();

    if( query("equipped") ) this_object()->unequip();
    if( environment() ) environment()->add_encumbrance( - weight() );
}

void inventory_burning(object ob, int heat)
{
    all_inventory()->receive_heat(ob, heat);
}
