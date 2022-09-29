// bank.c

inherit ROOM;

void init()
{
    add_action("do_convert", "convert");
    add_action("do_deposit", "deposit");
    add_action("do_withdraw", "withdraw");
    add_action("do_new_account", "open");
}

int do_new_account(string arg)
{
    int bal;
    object bankbond;

    if( !arg || arg!="account" ) return 0;
    if( present("bankbond", this_player()) )
        return notify_fail("你已經有一個戶頭了。\n");
    seteuid(geteuid(this_player()));
    bankbond = new("/obj/bankbond");
    if( !bankbond->move(this_player()) ) {
        write("你身上的東西太多了﹐帶不動錢莊金契。\n");
        destruct(bankbond);
        return 1;
    }
    bankbond->set("owner_id", geteuid(this_player()));
    bal = this_player()->query("bank_account");
    if( bal>0 ) {
        write("錢莊查對帳簿﹐扣掉一成的保證金﹐重新發給你一張新的金契。\n");
        bankbond->set_balance(bal*9/10);
    } else if( bal==0 ) {
        write("你和錢莊共同畫了個花押﹐作為以後金錢往來的憑據。\n");
        write("錢莊給你一張金契﹐上面記著你現在戶頭還餘下多少錢。\n");
        bankbond->set_balance(0);
    } else {
        write("你的帳簿出了點問題, 請找巫師反應。\n");
        destruct(bankbond);
    }
    return 1;
}

int do_convert(string arg)
{
    string from, to;
    int amount, bv1, bv2;
    object from_ob, to_ob;

    if( !arg || sscanf(arg, "%d %s to %s", amount, from, to)!=3 )
        return notify_fail("指令格式﹕convert <數量> <貨幣種類> to <貨幣種類>\n");

    seteuid(getuid());
    from_ob = present(from + "_money", this_player());
    to_ob = present(to + "_money", this_player());
    if( !to_ob && file_size("/obj/money/" + to + ".c") < 0 )
        return notify_fail("你想兌換哪一種錢﹖\n");

    if( !from_ob )        return notify_fail("你身上沒有這種貨幣。\n");
    if( amount < 1 )    return notify_fail("兌換貨幣一次至少要兌換一個。\n");
        
    if( (int)from_ob->query_amount() < amount )
        return notify_fail("你身上沒有那麼多" + from_ob->name() + "。\n");

    bv1 = from_ob->query("base_value");
    if( !bv1 ) return notify_fail("這種東西不值錢。\n");

    bv2 = to_ob ? to_ob->query("base_value") : call_other("/obj/money/" + to, "query", "base_value" );
    if( !bv2 ) return notify_fail("你要兌換哪一種貨幣﹖\n");

    if( bv1 < bv2 ) amount -= amount % (bv2 / bv1);
    if( amount==0 )    return notify_fail("這些" + from_ob->name() + "的價值太低了﹐換不起。\n");

    if( !to_ob ) {
        to_ob = new("/obj/money/" + to);
        to_ob->move(this_player());
        to_ob->set_amount(amount * bv1 / bv2);
    } else
        to_ob->add_amount(amount * bv1 / bv2);

    message_vision( sprintf("$N從身上取出%s%s%s﹐換成%s%s%s。\n",
        chinese_number(amount), from_ob->query("base_unit"), from_ob->name(),
        chinese_number(amount * bv1 / bv2), to_ob->query("base_unit"), to_ob->name()),
        this_player() );

    from_ob->add_amount(-amount);

    return 1;
}

int do_deposit(string arg)
{
    int amount;
    string money;
    object money_ob, bond;

    seteuid(getuid());
    if( !bond = present("bankbond", this_player()) )
        return notify_fail("請你先開一個戶頭。\n");

    if( geteuid(this_player()) != bond->query("owner_id") ) {
        write("錢莊發現你的花押不對﹐沒收了你的金契。\n");
        destruct(bond);
        return 1;
    }

    if( bond->query_balance() != this_player()->query("bank_account") ) {
        write("錢莊發現金契上的帳目不對﹐沒收了你的金契。\n");
        destruct(bond);
        return 1;
    }

    if( !arg || sscanf(arg, "%d %s", amount, money)!=2 )
        return notify_fail("指令格式﹕deposit <數量> <貨幣種類>。\n");

    if( amount < 0 )
        return notify_fail("你不能存入零以下的錢幣。\n");
        
    if( !money_ob = present(money + "_money", this_player()) )
        return notify_fail("你身上沒有這種錢幣。\n");

    if( money_ob->query_amount() < amount )
        return notify_fail("你身上沒有這麼多的" + money_ob->name() + "。\n");

    bond->transact(amount * money_ob->query("base_value"));
    money_ob->add_amount( - amount );
    write("你將" + chinese_number(amount) + money_ob->query("base_unit")
        + money_ob->name() + "交割給錢莊﹐錢莊重新發給你一張新的金契。\n");
    this_player()->save_autoload();
        this_player()->save();
    return 1;
}

int do_withdraw(string arg)
{
    int amount;
    string money;
    object money_ob, bond;

    seteuid(getuid());
    if( !bond = present("bankbond", this_player()) )
        return notify_fail("請你先開一個戶頭。\n");

    if( geteuid(this_player()) != bond->query("owner_id") ) {
        write("錢莊發現你的花押不對﹐沒收了你的金契。\n");
        destruct(bond);
        return 1;
    }

    if( bond->query_balance() != this_player()->query("bank_account") ) {
        write("錢莊發現金契上的帳目不對﹐沒收了你的金契。\n");
        destruct(bond);
        return 1;
    }

    if( !arg || sscanf(arg, "%d %s", amount, money)!=2 )
        return notify_fail("指令格式﹕withdraw <數量> <貨幣種類>。\n");

    if( amount < 0 )
        return notify_fail("你不能提領零以下的貨幣。\n");

    if( amount > 30000) 
        return notify_fail("你不能一次領太多。\n");    

    if( file_size("obj/money/" + money + ".c") < 0 )
	return notify_fail("你要提領哪一種錢﹖\n");

    if( catch(money_ob = new("/obj/money/" + money)) ) return 0;

    money_ob->set_amount(amount);
    if( bond->query_balance() < money_ob->value() ) {
        destruct(money_ob);
        return notify_fail("你的戶頭裡沒有這麼多錢。\n");
    }

    bond->transact(- money_ob->value());
    if( !money_ob->move(this_player()) ) {
        bond->transact(money_ob->value());
        destruct(money_ob);
        return notify_fail("你身上帶不了這許\多錢﹐提少一點吧。\n");
    }

    write("錢莊將" + chinese_number(amount) + money_ob->query("base_unit")
        + money_ob->name() + "交割給你﹐並重新發給你一張新的金契。\n");
        this_player()->save_autoload();
        this_player()->save();
    return 1;
}
