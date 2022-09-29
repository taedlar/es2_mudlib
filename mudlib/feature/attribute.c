/*  attribute.c

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

    /* 如果有設定 attribute 欄位，用其中的值作為屬性值 */
    if( mapp(attr=query("attribute")) ) {
	attribute = attr;
	delete("attribute");
    }

    if( !mapp(attribute) ) attribute = allocate_mapping(NUM_ATTRIBUTES);

    if( !mapp(base) || !sizeof(base) ) return;

    /* 根據所給的預設值「補齊」未設定的屬性 */
    foreach(name, value in base)
        if( undefinedp(attribute[name]) )
            attribute[name] = value;
}
