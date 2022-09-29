/*  unique.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
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


/* 這段可設 timer, 當 unique object 與 user 相遇時, 開始 call_out, 兩秒後消失

void init()
{
    if( userp(this_player()) ){
        message("test","\nOk, this_player is "+this_player()->name()+",obj is "+this_object()->query("id")+".\n",find_player("grain"));
        call_out( (: destruct(this_object()) :), 2 );
    }
    else message("test","\nHmm, this_player is "+this_player()->name()+".\n",find_player("grain"));
}
*/
