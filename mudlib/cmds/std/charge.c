// charge.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object ob;

	if( !arg || arg=="" ) {
		if( ob = me->query_charging() ) {
			write("你現在以" + ob->name() + "為首要攻擊目標。\n");
			return 1;
		}
		write("你現在並沒有選定首要攻擊目標。\n");
		return 1;
	}

	if( arg=="cancel" ) {
		write("Ok.\n");
		me->remove_charge();
		return 1;
	}

	ob = present(arg, environment(me));
	if( !ob ) return notify_fail("這裡沒有這個人。\n");

	if( !me->is_fighting(ob) )
		return notify_fail("你現在並沒有在跟" + ob->name() + "戰鬥。\n");
	me->charge_ob(ob);
	write("Ok.\n");

	return 1;
}

int help(object me)
{
	write(@TEXT
指令格式﹕charge [<戰鬥目標>|cancel]

這個指令讓你選定一個正在和你戰鬥中的敵人為首要攻擊目標﹐所謂首要攻擊
目標是指當這個目標還在跟你戰鬥的時候﹐會忽略其他敵人而專挑這個目標攻
擊﹐如此一來你可以先集中全力消滅比較可怕的敵人﹐但是相對的﹐你對其他
敵人的攻擊防禦率就只有平常的 2/3。

要取消 charge 模式﹐可以用 charge cancel﹐不加任何參數則顯示你目前集
中攻擊的對象。
TEXT
	);
	return 1;
}
