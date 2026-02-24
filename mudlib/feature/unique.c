/*
 *  Package: Unique
 *  Summary: Ensuring the uniqueness of an entity.
 */

nomask int
violate_unique()
{
    object *ob;

    if( !clonep(this_object()) ) return 0;

    // Find all the cloned instance of this_object()
    ob = filter_array( children(base_name(this_object())), (: clonep :) );

    return sizeof(ob) > 1;
}

object
create_replica()
{
    string replica;
    object obj;

    seteuid(getuid());
    if( stringp(replica = this_object()->query("replica_ob")) )
        obj = new(replica);
    else
        obj = 0;
    destruct(this_object());
    return obj;
}




