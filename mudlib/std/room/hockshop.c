/*  hockshop.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

inherit ROOM;

static object storage;

void remove()
{
    if( storage ) destruct(storage);
}

void setup()
{
    ::setup();
    seteuid(getuid());
    if( !storage ) {
        storage = new(__DIR__"hockshop_storage");
        storage->set("shop_ob", this_object());
        set("storage_ob", storage);
    }
}

void init()
{
    add_action("do_value", "value");
    add_action("do_pawn", "pawn");
    add_action("do_sell", "sell");
    add_action("do_retrieve", "retrieve");
    add_action("do_list", "list");
    add_action("do_buy", "buy");
    add_action("do_store", "store");
}

string value_string(int value)
{
    if( value < 1 ) value = 1;
    else if( value < 100 )
        return chinese_number(value) + "文錢";
    else
        return chinese_number(value/100) + "兩"
            + (value%100? "又" + chinese_number(value%100) + "文錢": "");
}

void pay_player(object who, int amount)
{
    object ob;

    if( amount < 1 ) amount = 1;
    if( amount/100 ) {
        ob = new(SILVER_OB);
        ob->set_amount(amount/100);
        if( !ob->move(who) ) ob->move(environment(who));
        amount %= 100;
    }
    if( amount ) {
        ob = new(COIN_OB);
        ob->set_amount(amount);
        if( !ob->move(who) ) ob->move(environment(who));
    }
}

int do_value(string arg)
{
    object ob;
    int value;

    if( !arg || !(ob = present(arg, this_player())) )
        return notify_fail("你要拿什麼物品給當鋪估價﹖\n");

    if( ob->query("money_id") )
        return notify_fail("這是「錢」﹐你沒見過嗎﹖\n");

    value = ob->query("value");
    if( !value ) printf("%s一文不值。\n", ob->query("name"));
    if( ob->query("sinister") )
        printf("%s價值%s。\n但是這種死人身上的倒楣東西當鋪很不願意收。\n"
            "典當(pawn)或賣斷(sell)只能得到%s。\n",
            ob->query("name"), value_string(value), value_string(value / 100 ));
    else 
        printf("%s價值%s。\n如果你要典當(pawn)﹐可以拿到%s及一張當票。\n如果賣斷(sell)﹐可以拿到%s。\n",
            ob->query("name"), value_string(value),
            value_string(value * 60 / 100), value_string(value * 80 / 100));

    return 1;
}

int do_pawn(string arg)
{
    object ob, stamp;
    int value;

    if( !arg || !(ob = present(arg, this_player())) )
        return notify_fail("你要典當什麼物品﹖\n");

    if( ob->query("money_id") )    return notify_fail("你要當「錢」﹖\n");

    value = ob->query("value");
    if( !value ) return notify_fail("這樣東西不值錢。\n");

    if (ob->query("no_sell")) return notify_fail("這樣東西不能典當。\n");

    message_vision("$N把身上的" + ob->query("name") + "拿出來典當了"
        + value_string(value * 60 / 100) + "﹐換得一張當票。\n", this_player());

    if( ob->query("sinister") ) {
        int v;
        message_vision("當鋪朝奉對這種死人身上的倒楣東西很不甘願地收下了 ...\n", this_player());
        v = value / 100;
        if( v > 100 ) v = 100;
        pay_player(this_player(), v);
    }
    else
        pay_player(this_player(), value * 60 / 100);

    ob->move(storage);
    stamp = new(__DIR__"obj/pawn_stamp");
    stamp->set("pawned_item", ob);
    stamp->set("long", "茲收到\n    " + this_player()->name(1)
        + "典當" + ob->name() + "乙" + ob->query("unit")
        + "訖﹐破爛缺損不堪。\n    贖回 (retrieve) 應付" + value_string(value * 120/100)
        + "整﹐一手交錢﹐一手交貨。\n");
    stamp->move(this_player());

    return 1;
}

int sell_item(object ob)
{
    int value;

    if( ob->query("money_id") )    return notify_fail("你要賣「錢」﹖\n");
    value = ob->query("value");
    if( !value ) return notify_fail("這樣東西不值錢。\n");

    if (ob->query("no_sell")) return notify_fail("這樣東西不能賣。\n");

    message_vision("$N把身上的" + ob->query("name") + "賣掉。\n",
        this_player());

    if( ob->query("sinister") ) {
        int v;
        message_vision("當鋪朝奉對這種死人身上的倒楣東西很不甘願地收下了 ...\n", this_player());
        v = value / 100;
        if( v > 100 ) v = 100;
        pay_player(this_player(), v);
    }
    else
        pay_player(this_player(), value * 80 / 100);

    ob->delete("sinister");

    if( storage ) {
        int s;

        // Make a simple storage management, when we have more goods in
        // storage, only keeps higher price goods.
        s = sizeof(all_inventory(storage));
        if( value >= (s * 50) ) {
            ob->add("pawned_time", 1);
            ob->move(storage);
        }
        else destruct(ob);
    }
    else destruct(ob);

    return 1;
}

int do_sell(string arg)
{
    object ob;

    if( !arg ) return notify_fail("你要賣斷什麼物品﹖\n");

    if( arg=="all" ) {
        object *inv;
        
        inv = all_inventory(this_player());
        foreach(ob in inv) {
            if(!ob) continue;
            if( ob->query("keep_this") ) continue;
            sell_item(ob);
        }
        write("Ok。\n");
        return 1;
    }

    if( !(ob = present(arg, this_player())) )
        return notify_fail("你要賣斷什麼物品﹖\n");

    return sell_item(ob);
}

int do_retrieve(string arg)
{
    object stamp, ob;
    int value;

    if( !arg || arg=="" || !(stamp = present(arg, this_player())) )
        return notify_fail("你要贖回哪一張當票上的典物﹖\n");

    if( !stamp->id("pawn stamp") )
        return notify_fail("這不是當票。\n");

    if( !(ob = stamp->query("pawned_item")) ) {
        write("這張當票上的典物因為你太久沒有來贖﹐已經被當鋪賣掉了。\n");
        write("你隨手將當票撕掉扔了。\n");
        destruct(stamp);
        return 1;
    }

    if( !present(ob, storage) )
        return notify_fail("這不是這家當鋪開出的當票。\n");

    value = (int)ob->query("value") * 120 / 100;
    if( (int)this_player()->can_afford(value) != 1 )
        return notify_fail("你身上的(零)錢不夠。\n");

    message_vision("$N取出當票﹐花了" + value_string(value) + "贖回了"
        + ob->name() + "。\n", this_player());

    this_player()->pay_money(value);
    ob->move(this_player());
    ob->delete("pawned_time");
    destruct(stamp);

    return 1;
}

int do_list(string arg)
{
    int i;
    object *inv;
    string msg;

    if( !storage ) return notify_fail("當鋪裡現在沒有貨物。\n");
    if( wizardp(this_player()) )
        inv = all_inventory(storage);
    else
        inv = filter( all_inventory(storage), (: $1->query("pawned_time") :));
    if( !sizeof(inv) ) return notify_fail("當鋪裡現在沒有可供出售的貨物。\n");
    if( arg ) inv = filter( inv, (: $1->id($(arg)) :) );

/*
    msg = "當鋪裡現在有以下貨物出售﹕\n  ";
    msg += implode( map(inv, (: sprintf("%-50s%s", $1->short(),
        value_string((int)$1->query("value") * 2)) :)), "\n  ");
    msg += "\n";
    this_player()->start_more(msg);
    return 1;
*/

    msg = "當鋪裡現在有以下貨物出售﹕\n\n";
    for ( i = 0; i < sizeof(inv); i++)
        msg += sprintf("     %-5d  %-50s%s\n",
        i+1,
        inv[i]->short(),
        value_string(inv[i]->query("value") * 2));
    this_player()->start_more(msg);
    return 1;
}

int do_buy(string arg)
{
    int i, index;
    object *inv, ob;

    if( !arg ) return notify_fail("你要買什麼東西﹖\n");

    if( !storage ) return notify_fail("當鋪裡現在沒有貨物。\n");
    inv = filter_array( all_inventory(storage), (: $1->query("pawned_time") :));
    if( !sizeof(inv) ) return notify_fail("當鋪裡現在沒有貨物。\n  ");

// This is a crasher! Dunno why .... Annihilator(11/01/95)
//    if( !parse_command(arg, inv, "%o", ob) )
//        return notify_fail("你要買什麼東西﹖\n");

    if( sscanf(arg, "%s %d", arg, index) != 2 ) index = 1;
    index--;
    for(i=0; i<sizeof(inv); i++)
        if( inv[i]->id(arg) && !(index--)) break;

    if( i==sizeof(inv) )
        return notify_fail("你要買什麼東西﹖\n");
    ob = inv[i];

    if( (int)this_player()->can_afford((int)ob->query("value") * 2) != 1 )
        return notify_fail("你的(零)錢不夠。\n");
    message_vision("$N掏出錢買下一" + ob->query("unit") + ob->name() + "。\n",
        this_player() );
    this_player()->pay_money((int)ob->query("value") * 2);
    ob->delete("pawned_time");
//     fix if pay money....and player can't get....drop in environment
    if(!ob->move(this_player()))
        ob->move(environment(this_player()));
    else
        ob->move(this_player());

    return 1;
}

int do_store(string arg)
{
    if( !wizardp(this_player()) ) return 0;
    if( !storage ) return notify_fail("倉庫物件不存在。\n");
    this_player()->move(storage);
    write("Ok.\n");
    return 1;
}
