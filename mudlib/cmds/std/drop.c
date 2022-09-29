// drop.c

#include <login.h>
inherit F_CLEAN_UP;

int do_drop(object me, object obj);

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    object obj, *inv, obj2;
    int i, amount;
    string item;

    if( me->is_busy() )
        return notify_fail("你正忙著其他事﹗\n");

    if(!arg) return notify_fail("你要丟棄什麼東西﹖\n");
    if( !environment(me) ) return 0;

    if(sscanf(arg, "%d %s", amount, item)==2) {
        if( !objectp(obj = present(item, me)) )
            return notify_fail("你身上沒有這樣東西。\n");
        if( stringp(obj->query("no_drop")) )
            return notify_fail( (string)obj->query("no_drop") );
        if( !obj->query_amount() )
            return notify_fail( obj->name() + "不能被分開丟棄。\n");
        if( amount < 1 )
            return notify_fail("東西的數量至少是一個。\n");
        if( amount > obj->query_amount() )
            return notify_fail("你沒有那麼多的" + obj->name() + "。\n");
        else if( amount == (int)obj->query_amount() )
            return do_drop(me, obj);
        else {
            obj->set_amount( (int)obj->query_amount() - amount );
            seteuid(MUDLIB_UID);
            obj2 = new(base_name(obj));
            seteuid(getuid());
            obj2->set_amount(amount);
            return do_drop(me, obj2);
        }
    }

    if(arg=="all") {
        inv = all_inventory(me);
        for(i=0; i<sizeof(inv); i++) {
            if( inv[i]->query("keep_this") ) continue;
            do_drop(me, inv[i]);
        }
        write("Ok.\n");
        return 1;
    }

    if(!objectp(obj = present(arg, me)))
        return notify_fail("你身上沒有這樣東西。\n");
    return do_drop(me, obj);
}

int do_drop(object me, object obj)
{
    mixed no_drop;

    if( no_drop = obj->query("no_drop") )
        return notify_fail( stringp(no_drop) ? no_drop : "這樣東西不能隨意丟棄。\n");

    if (obj->move(environment(me))) {
        if( obj->is_character() )
            message_vision("$N將$n從背上放了下來﹐躺在地上。\n", me, obj);
        else {
            if ( wizardp(me) ) {
		write("一隻綠色的小鬼很快的跑出來，接住你丟下的東西，然後消失不見了。\n");
		destruct(obj); 
		return 1;
	    } // 巫師不能丟東西 add by tears
	    message_vision( sprintf("$N丟下一%s$n。\n", obj->query("unit")),
		me, obj );
	    if( !obj->query("value") && !obj->value() ) {
		write("因為這樣東西並不值錢﹐所以人們並不會注意到它的存在。\n");
		destruct(obj);
		return 1;
	    }
	}
	// 不值錢的東西早就被 dest 掉了... 這裡沒有 obj.... -elon 12/2/96
	if(obj) obj->delete("keep_this");
#ifdef SAVE_USER
//    me->save();
#endif 
	return 1;
    }
    return 0;
}

int help(object me)
{
    write(@HELP
指令格式 : drop <物品名稱>
 
這個指令可以讓你丟下你所攜帶的物品.
例如:
drop all	丟下所有未 keep 的物品.
drop sword 2	丟下你所攜帶的第二把 sword.
drop 200 gold	丟下二百兩黃金. 
HELP
    );
    return 1;
}
