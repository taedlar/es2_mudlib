// bankbond.c

inherit ITEM;

int balance = 0;

void create()
{
	set_name("錢莊金契", ({ "bankbond" }));
	set_weight(1);
	if( !clonep() ) {
		set("unit", "張");
		set("long",
			"這是一張錢莊用來記載跟客戶金錢往來的金契﹐上面有雙方的花押跟\n"
			"結餘。\n");
        set("no_sell", 1);
	        set("value", 1);
	}
	set("keep_this", 1);
	setup();
}

string query_autoload() { return balance + ""; }

void autoload(string param)
{
	if( sscanf(param, "%d", balance) )
		set("owner_id", geteuid(previous_object()));
}

void transact(int amount)
{
	if( environment()->query("bank_account") != balance ) return;

	balance = environment()->query("bank_account") + amount;
	environment()->set("bank_account", balance);
}

void set_balance(int b) { balance = b; environment()->set("bank_account", b); }
int query_balance() { return balance; }

string long(int raw)
{
	string str;
	int amount;

	str = ::long(raw);
	if( balance > 0 ) {
		str += "你現總共結餘﹕";
		amount = balance;
	} else if( balance < 0 ) {
		str += "你現在還賒欠錢莊﹕";
		amount = - balance;
	}
	if( balance != 0 ) {
		if( amount >= 100 ) {
			str += chinese_number(amount/100) + "兩";
			if( amount%100 ) str += "又";
		}
		if( amount%100!=0 )
			str += chinese_number(amount%100) + "文錢。";
		str += "\n";
	}

	return str;	
}
