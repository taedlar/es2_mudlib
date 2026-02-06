

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string item, targ;
    object ob, owner;
    mixed handle;
    int price, afford;

    if( me->is_busy() ) return notify_fail("你現在沒有空﹗\n");

    if( !arg || sscanf(arg, "%s from %s", item, targ)!=2 )
        return notify_fail("指令格式﹕buy <某物> from <某人>\n");

    if( !objectp(owner = present(targ, environment(me))) )
        return notify_fail("你要跟誰買東西﹖\n");

    if( userp(owner) ) {
        message_vision("$N想向$n購買「" + item + "」。\n", me, owner);
        return 1;
    }

    // add by dragoon
        if( objectp(owner) && owner->query("no_fight") )
                return notify_fail("什麼??\n");

    if( !living(owner) )
        return notify_fail("看起來"+owner->name()+"已經無法買賣東西了。\n");
        
    if( owner->is_fighting() )
        return notify_fail("看起來" + owner->name() + "正忙著 ... 打架﹐沒空理你。\n");

    notify_fail("對方好像不願意跟你交易。\n");
    if( !(handle = owner->affirm_merchandise(me, item)) ) return 0;
    price = owner->query_trading_price(handle);

    switch( me->can_afford(price) ) {
        case 0: return notify_fail("你身上的錢不夠。\n");
        case 1:
            me->pay_money(price);
            owner->deliver_merchandise(me, handle);
            me->gain_score("survive", random(price/300 + me->query_attr("wis")/3) );
            return 1;
        case 2:
            return notify_fail("你沒有足夠的零錢﹐而對方也找不開...。\n");
    }

}

int help(object me)
{
   write( @HELP
指令格式: buy <something> from <someone>

這一指令讓你可以從某些人身上買到物品。
HELP
   );
   return 1;
}

