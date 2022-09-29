// fill.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object ob, from_ob, ob_liq ,from_liq, new_liq;
	string source;
	int new_volume, empty_flag = 0;

	seteuid(getuid());
	if( me->is_busy() ) return notify_fail("請先停止你正在忙的事。\n");

	if( !me->query_stat_maximum("kee") )
		return notify_fail("你沒有形體﹐無法做這個動作。\n");

	notify_fail("指令格式：fill <容器> with <液體>|<容器>\n");
	if( !arg || arg=="" ) return 0;

	if( sscanf(arg, "%s with %s", arg, source)!=2 )	return 0;

	if( arg == source )
		return notify_fail("你這樣作是行不通的﹗\n");

	// find ob
	ob = present(arg, me);
	if( !ob ) ob = present(arg, environment(me));
	if( !ob ) return notify_fail("這裡沒有 " + arg + " 這樣東西。\n");
	if( !ob->query("liquid_container") )
		return notify_fail( ob->name() + "並不是裝水的容器。\n");
	// return fail if ob_liq exist 
	foreach(ob_liq in all_inventory(ob))
		return notify_fail(
		"請把" + ob->name() + "裏的" + ob_liq->name() + "喝乾淨再來裝。\n");

	// find from_ob
	from_ob = present(source, me);
	if( !from_ob ) from_ob = present(source, environment(me));
	if( !from_ob ) {
		// resource of environment, set("resource/<source>") in room
		if( environment(me)->query("resource/" + source) ) {
			if( !(new_liq = new("/obj/" + source)) )
				return notify_fail("這附近的 " + source + " 有點問題, 請向巫師反應。\n");
//  marked by dragoon since still found bug and change to return fail if
//  found ob_liq
/*
			foreach(ob_liq in all_inventory(ob))	// dest liq of ob, keep solid
				if( function_exists("query_volume", ob_liq) ) {
					destruct(ob_liq);
					empty_flag++;
				}
			new_volume = to_int( ob->query_max_encumbrance() - ob->query_encumbrance()/new_liq->query("density") );
			new_liq->set_volume(new_volume);

			if( !ob->accept_object(me, new_liq) )
				return notify_fail(ob->name() + " 不能裝入" + new_liq->name() + "。\n");

			new_liq->move(ob);

			message_vision("$N將" + ob->name(), me);
			if(empty_flag)
				tell_room(environment(me), "中的液體倒空, 並且");
			tell_room(environment(me), "裝滿了附近的" + new_liq->name() + "。\n");

			return 1;
*/
		}
		return notify_fail("你身上, 地上, 及這附近都沒找到像 " + source + " 那樣的液體或容器。\n");
	}

	// check from_ob and from_liq
	if( !from_ob->query("liquid_container") )
		return notify_fail( from_ob->name() + "並不是裝水的容器。\n");
	if( from_ob->hold_object() )
		return notify_fail( from_ob->name() + "現在緊緊閉著。\n");
	foreach(from_liq in all_inventory(from_ob))
		if( function_exists("query_volume", from_liq) ) break;
	if( !from_liq )
		return notify_fail(from_ob->name() + "裡沒有一滴水。\n");

	// check ob and ob_liq
	if( !ob->accept_object(me, from_liq) ) return 0;

	// set new_liq, move to ob
	new_liq = clone_object(base_name(from_liq));
	new_volume = to_int( (ob->query_max_encumbrance() -
	    ob->query_encumbrance()) / new_liq->query("density") );
	if( new_volume > from_liq->query_volume() )
		new_volume = from_liq->query_volume();
	if( new_volume > 0 ) {
		new_liq->set_volume(new_volume);
		new_liq->move(ob);
		from_liq->add_volume(-new_volume);
	}
	message_vision("$N將" + from_ob->name() + "中的" + new_liq->name() +
		"倒入" + ob->name() + "中。\n", me );
	return 1;
}

int help(object me)
{
write(@TEXT
指令格式：fill <容器一> with <容器二>|<液體>

將容器二中的液體倒入容器一中。但目前不能把不同之液體混在一起。

有些地方如溪邊、池塘等有水之處，或者房間敘述有提到液體源的地方，你也可以試
試fill <容器> with <液體源> (如：water、tea、oil、 ... 請參考你發現的提示)
例如:
fill skin with pot
fill bottle with water
TEXT
	);
	return 1;
}
