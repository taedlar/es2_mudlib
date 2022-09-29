// put.c
#include <login.h>

inherit F_CLEAN_UP;

int do_put(object me, object obj, object dest);

void create() {seteuid(getuid());}

int main(object me, string arg)
{
	string target, item;
	object obj, dest, *inv, obj2;
	int i, amount;

	if(!arg || sscanf(arg, "%s in %s", item, target)!=2 )
		return notify_fail("你要將什麼東西放進哪裡﹖\n");

	dest = present(target, me);
	if( !dest || living(dest) ) dest = present(target, environment(me));
	if( !dest || living(dest) )
		return notify_fail("這裡沒有這樣東西。\n");

	if(sscanf(item, "%d %s", amount, item)==2) {
		if( !objectp(obj = present(item, me)) )
			return notify_fail("你身上沒有這樣東西。\n");
		if( !obj->query_amount() )
			return notify_fail( obj->name() + "不能被分開。\n");
		if( amount < 1 )
			return notify_fail("東西的數量至少是一個。\n");
		if( amount > obj->query_amount() )
			return notify_fail("你沒有那麼多的" + obj->name() + "。\n");
		else if( amount == (int)obj->query_amount() ) {
			// add buy dragoon to fix the bug of amount
			if( obj->weight() > dest->query_max_encumbrance()
			- dest->query_encumbrance() )
				return notify_fail(dest->name() + "似乎裝不下了。\n");
			return do_put(me, obj, dest); }
		else {
			// add buy dragoon to fix the bug of amount
			if( amount*obj->weight()/obj->query_amount() >
			dest->query_max_encumbrance()-dest->query_encumbrance() ) 
				return notify_fail(dest->name() + "似乎裝不下了。\n");
			obj->set_amount( (int)obj->query_amount() - amount );
			seteuid(MUDLIB_UID);
			obj2 = new(base_name(obj));
			seteuid(getuid());
			obj2->set_amount(amount);
			return do_put(me, obj2, dest);
		}
	}

	if(item=="all") {
		inv = all_inventory(me);
		for(i=0; i<sizeof(inv); i++)
			if( inv[i] != dest ) do_put(me, inv[i], dest);
		write("Ok.\n");
		return 1;
	}

	if(!objectp(obj = present(item, me)))
		return notify_fail("你身上沒有這樣東西。\n");
	// add buy dragoon to fix the bug of amount
	if( obj->weight() > dest->query_max_encumbrance()
	- dest->query_encumbrance() )
		return notify_fail(dest->name() + "似乎裝不下了。\n");
	return do_put(me, obj, dest);
}

int do_put(object me, object obj, object dest)
{
	object inv;	

	if( obj->query("no_drop") || obj->query("no_get"))
		return notify_fail("這樣東西無法被移動。\n");

	if( obj==dest ) 
		return notify_fail("把" + obj->name() + "放進" + dest->name() + "﹖﹖﹖\n");

	if( !dest->accept_object(me, obj) ) return 0;

	if( inv=present(obj->query_id(), dest) ) {
		if( obj->query_amount() > 0 ) {
			inv->set_amount((int)inv->query_amount() 
			+ (int)obj->query_amount());
			message_vision( sprintf("$N將一%s%s放進%s。\n",
			obj->query("unit"), obj->name(), dest->name()), me);
			destruct(obj); }
		else {
			obj->move(dest);
			message_vision( sprintf("$N將一%s%s放進%s。\n",
			obj->query("unit"), obj->name(), dest->name()), me); }
	} else { 
		obj->move(dest);
		message_vision( sprintf("$N將一%s%s放進%s。\n",
		obj->query("unit"), obj->name(), dest->name()),	me );
	}
#ifdef SAVE_USER
        me->save();
#endif
	return 1;
}

int help(object me)
{
write(@HELP
指令格式 : put <物品名稱> in <某容器>
 
這個指令可以讓你將某樣物品放進一個容器﹐當然﹐首先你要擁有這樣物品。
 
HELP
    );
    return 1;
}
