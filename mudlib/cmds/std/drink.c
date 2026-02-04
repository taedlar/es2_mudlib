// vim: set ts=4 sw=4 syntax=lpc

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object ob;

    if( me->is_busy() ) return 0;

    notify_fail("你要喝什麼﹖\n");
    if( !arg || arg=="" ) return 0;

    if( !me->query_stat_maximum("kee") ) // changed by dragoon
        return notify_fail("你氣若游絲，無法喝下任何東西。\n");

    if(me->query_stat_maximum("water") <= me->query_stat("water"))
        return notify_fail("你再也喝不下東西了。\n");

    // Drink from liquid containers.
    if( sscanf(arg, "from %s", arg) ) {
        object liq;

        ob = present(arg, me);
        if( !ob ) ob = present(arg, environment(me));
        if( !ob ) return notify_fail("這裡沒有這樣東西 ...。\n");
        if( !ob->query("liquid_container") )
            return notify_fail( ob->name() + "並不是裝水的容器。\n");
        foreach(liq in all_inventory(ob))
            if( liq->query_volume()) {
                liq->drink_ob(me);
                me->start_busy(1);
                return 1; }

        return notify_fail( ob->name() + "裡沒有任何能喝的東西。\n");
    }

    return environment(me)->do_drink(arg);
}

int help()
{
    write(@TEXT
指令格式： drink from <something>, drink <something>

這個指令可以用來飲水或喝下某些液體, 如山泉、河水、酒等。Something 可能為中
文字, 看不的到容器的也許不需要用受詞 from。

TEXT
    );
    return 1;
}

