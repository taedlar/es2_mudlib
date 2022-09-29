/*  vendor.c

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
#include <name.h>

static mapping stock;

private void
reset()
{
    if( !environment() ) return;
    stock = copy(query("merchandise"));
}

// affirm_merchandise()
//
// This is an apply function interfacing with the standard 'buy' command
// which is called when an player attemp to 'buy xxx from yyy' while xxx
// is passed via the parameter what and yyy is this_object() (must be
// present to the player). If we affirmed that we have such item for sell,
// buy_ob should return an string, or int, or anything that identifies
// this item we have in later bargain. Else, 0 is returned if we are not
// selling such item.

mixed
affirm_merchandise(object buyer, string what)
{
    mapping list;
    string item;
    int i, index, count;

    if( sscanf(what, "%s %d", what, index) != 2 )
	index = 1;

    if( !mapp(list = query("merchandise")) )
	return notify_fail(name() + "並沒有賣 " + what + " 這種東西。\n");

    if( !mapp(stock) ) stock = copy(list);

    foreach(item, count in stock)
    {
	if( (!item->id(what)) || (--index) ) continue;
	if( stock[item] < 1 ) {
	    write(item->name() + "已經賣完了﹐待會兒再來吧。\n");
	    return 0;
	}
	return item;
    }

}

int
query_trading_price(string handle)
{
    mapping list;

    if( !mapp(list = query("merchandise")) )
	return 0;

    if( undefinedp(list[handle]) ) return 0;

    return handle->query("value");
}

void
deliver_merchandise(object me, string what)
{
    mapping list;
    string ob_file;
    object ob;

    if ( !living(this_object()) ) return;

    list = query("merchandise");
    if( !mapp(list) || undefinedp(list[what]) ) return;

    ob = new(what);

    // Special process for liquid merchandise.
    if( ob->query_volume() ) {
	object container_ob;
	string container;
	if( !(container = ob->query("default_container")) )
	    container = "/obj/bottle";
	container_ob = new(container);
	ob->move(container_ob);
	container_ob->move(me);
	write("你向" + name() + "買下一"
		+ container_ob->query("container_unit") + ob->query("name") + "。\n");
    } else {
	ob->move(me);
	write("你向" + name() + "買下一"
		+ ob->query("unit") + ob->query("name") + "。\n");
    }

    // Reduce stock account;
    if( !mapp(stock) ) stock = copy(query("merchandise"));
    stock[what]--;
}

private string
price_string(int v)
{
    if( v%10000 == 0 ) return chinese_number(v/10000) + "兩黃金";
    if( v%100 == 0 ) return chinese_number(v/100) + "兩銀子";
    return chinese_number(v) + "文錢";
}

int
do_vendor_list(string arg)
{
    mapping goods;
    string list, item;
    int count;

    if( !mapp(goods = query("merchandise")) ) return 0;
    if( arg && !id(arg) ) return 0;

    list = "";
    foreach(item, count in goods) {
	if( count < 1 ) continue;
	list += sprintf("  %-30s：%s\n",
	    item->short(1),
	    price_string(item->query("value")) );
    }
    if( list=="" ) {
	write( name() + "的貨物已經全部賣光了，下次早一點來吧！\n");
	return 1;
    }

    write("你可以購買下列這些東西：\n-------------------------\n" + list);
    return 1;
}
