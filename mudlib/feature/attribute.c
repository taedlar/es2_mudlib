

#include <dbase.h>
#include <attribute.h>

mapping attribute = ([]);

mapping query_attribute() { return attribute; }

// query_attr()
//
// returns the value of a character's attribute.

varargs int
query_attr(string what, int raw)
{
    int a;

    if( !mapp(attribute) || undefinedp(a=attribute[what]) ) return 0;

    if( raw ) return a;

    return a + (int)query_temp("apply/" + what);
}

int
set_attr(string what, int value)
{
    if( !mapp(attribute) ) return 0;

    if( userp(this_object())
    &&(	undefinedp(attribute[what])
       ||	value < ATTRVAL_MIN
       ||	value > ATTRVAL_MAX ) )
	return 0;

    return (attribute[what] = value);
}

varargs void
init_attribute(mapping base)
{
    mapping attr;
    string name;
    int value;


    if( mapp(attr=query("attribute")) ) {
	attribute = attr;
	delete("attribute");
    }

    if( !mapp(attribute) ) attribute = allocate_mapping(NUM_ATTRIBUTES);

    if( !mapp(base) || !sizeof(base) ) return;


    foreach(name, value in base)
        if( undefinedp(attribute[name]) )
            attribute[name] = value;
}

