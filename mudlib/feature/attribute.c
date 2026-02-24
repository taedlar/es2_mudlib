/*
 *  Package: Attributes
 *  Summary: An entity's attributes, such as strength, intelligence, etc.
 *
 *      If the entity is a character (living object), then the attributes will be used to
 *      calculate the character's abilities, such as attack, defense, magic power, etc.
 *      Character's attributes can be modified by various means, such as conditions, equipments,
 *      magical effects, etc.
 *
 *      If the entity is not a character, the attributes will be used for calculation when
 *      a character interacts with it.
 */

#include <dbase.h>
#include <attribute.h>

mapping attribute = ([]);

mapping query_attribute() { return attribute; }

varargs int query_attr (string what, int raw) {
    int a;
    if (!mapp(attribute) || undefinedp(a = attribute[what]))
        return 0;

    if (raw)
        return a;

    return a + (int)query_temp ("apply/" + what);
}

int set_attr (string what, int value) {
    if (!mapp(attribute))
        attribute = ([]);

    // For user characters, the attribute must be initialized in init_attribute() and
    // the value must be between ATTRVAL_MIN and ATTRVAL_MAX.
    if (userp(this_object())
    && (undefinedp(attribute[what]) || value < ATTRVAL_MIN || value > ATTRVAL_MAX))
        return 0;

    return (attribute[what] = value);
}

varargs void init_attribute (mapping base) {
    mapping attr;
    string name;
    int value;

    if (mapp (attr = query("attribute"))) {
        attribute = attr;
        delete("attribute");
    }

    if (!mapp(attribute))
        attribute = allocate_mapping (NUM_ATTRIBUTES);

    if (!mapp(base) || !sizeof(base))
        return;

    foreach (name, value in base)
        if (undefinedp(attribute[name]))
            attribute[name] = value;
}
