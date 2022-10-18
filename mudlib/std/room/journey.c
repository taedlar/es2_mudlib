// journey.c

inherit ROOM;

static int on_journey = 0;

private void confirm_journey(string arg);

varargs int
do_look(object me, string arg)
{
    if( on_journey==1 )
        write(query("long") + "如果你想回頭﹐可以打 return。\n");
    else if( on_journey==-1 )
        write("你正往回走向原來出發的地方。\n");
    return 1;
}

void init()
{
    if( this_player() ) {
        if( userp(this_player()) ) {
            //write("你確定要離開這個區域嗎﹖[y/n]");
            //input_to("confirm_journey");
            confirm_journey("yes");
            add_action("do_return", "return");
        } else
            confirm_journey("no");
    }
}

int
receive_object(object ob, int from_inventory)
{
    return sizeof(all_inventory())==0;
}

int
check_arrive(object ob, object who, string how)
{
    if( how=="arrive" ) return 1;
    return notify_fail("你正在旅途中﹐先到達目的地再說吧。\n");
}

int check_journey() { return 1; }

// Confirm if this_player() really want make a journey.
private void
confirm_journey(string arg)
{
    if( arg[0]=='y' ) {
        on_journey = 1;
        call_out("arrive_destination", query("distance"), this_player());
//        do_look(this_player());
        this_player()->start_busy((: check_journey :), (: check_arrive :));
    } else if( arg[0]=='n' ) {
        this_player()->move(query("from_room"));
        destruct(this_object());
//    } else {
//        write("你確定要離開這個區域嗎﹖[y/n]");
//        input_to("confirm_journey");
    }
}

private void
arrive_destination(object me)
{
    int res;

    if( !objectp(me) ) return;

    me->interrupt(this_object(), "arrive");
    if( on_journey == 1 ) res = me->move(query("to_room"));
    else  res = me->move(query("from_room"));
    if( !res ) {
        call_out("arrive_destination", 5, me);
        return;
    }
    destruct(this_object());
}

private int
do_return(string arg)
{
    on_journey = -1;
    write("你開始往回走。\n");
    return 1;
}
