/*  get.c - get command

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#pragma save_binary

#include <ansi.h>

inherit F_CLEAN_UP;

int do_get(object me, object ob);

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string from, item;
    object obj, *inv, env, obj2, obj3;
    int i, amount;

    if( me->query("life_form") == "ghost" )
        return notify_fail("人死了就一了百了, 快去找城隍復活吧!!\n");

    if( !arg ) return notify_fail("你要撿起什麼東西﹖\n");
    if( me->is_busy() )
        return notify_fail("你上一個動作還沒有完成﹗\n");

    if( !me->query_stat_maximum("HP") )
        return notify_fail("你沒有形體﹐無法拾取物品。\n");

    // Check if a container is specified.
    if( sscanf(arg, "%s from %s", arg, from)==2 ) {
	env = present(from, me);
	if(!env) env = present(from, environment(me));
	if(!env) return notify_fail("你找不到 " + from + " 這樣東西。\n");
	if( (living(env) || userp(env))
	&& (wiz_level(me) <= wiz_level(env)))
	    return notify_fail("你的巫師等級必須比對方高﹐才能搜身。\n");
    } else env = environment(me);

    // Check if a certain amount is specified.
    if(sscanf(arg, "%d %s", amount, item)==2) {
        if( !objectp(obj = present(item, env)) )
            return notify_fail("這裡沒有這樣東西。\n");
        if( !obj->query_amount() )
            return notify_fail( obj->name() + "不能被分開拿走。\n");
        if( amount < 1 )
            return notify_fail("東西的個數至少是一個。\n");

        if( amount > obj->query_amount() )
            return notify_fail("這裡沒有那麼多的" + obj->name() + "。\n");
        else if( amount == (int)obj->query_amount() ) {
            return do_get(me, obj);

        } else {
            int old_amount;
            old_amount = obj->query_amount();
            
            obj2 = new(base_name(obj));
            obj2->move(env);    // for container
            obj2->set_amount( amount );            
            obj->set_amount( old_amount - amount );
            // Counting precise amount costs more time.
            if( me->is_fighting() ) me->start_busy(3);
            return do_get(me, obj2);
        }
    }

    // Check if we are makeing a quick get.
    if(arg=="all") {
	object ob;
	if( me->is_fighting() )
	    return notify_fail("你還在戰鬥中﹗只能一次拿一樣。\n");

	if( !function_exists("receive_object", env) )
	    return notify_fail("那不是容器。\n");

	inv = all_inventory(env);
	if( !sizeof(inv) )
	    return notify_fail("那裡面沒有任何東西。\n");

	foreach(ob in inv) do_get(me, ob);

	return 1;
    }

    if( !objectp(obj = present(arg, env)) || living(obj) )
        return notify_fail("你附近沒有這樣東西。\n");

    return do_get(me, obj);
}
    
int do_get(object me, object obj)
{
    object from_ob, *enemy, *guard;
    int equipped;
    mixed msg;

    if( !obj ) return 0;
    from_ob = environment(obj);

    if( obj->is_character() ) 
	return notify_fail("不行!\n");
/*
    {
        if( living(obj) ) return 0;
        // Don't allow player to move unconcious NPC
        if( !userp(obj) && !obj->is_corpse() )
            return notify_fail("你只能背負其他玩家的身體。\n");
    }
*/
    notify_fail("這樣東西拿不出來。\n");
    if( from_ob->hold_object(obj) ) return 0;

    if( guard = obj->query_temp("guarded") ) {
	guard = filter_array(guard, (:
	    objectp($1)
	    && present($1, environment($2))
	    && living($1)
	    && ($1!=$2)
	    :), me);
	if( sizeof(guard) )
	    return notify_fail( guard[0]->name() 
		+ "正守在" + obj->name() + "一旁﹐防止任何人拿走。\n");
    }

    if( (msg = obj->query("no_get")) )
	return notify_fail(stringp(msg) ? msg : "這個東西拿不起來。\n");

    if( me->over_encumbranced() ) return notify_fail("你已經負荷過重了﹗\n");

    if( obj->query("equipped") ) equipped = 1;

    if( ! obj->move(me) ) return 0;
    if( me->is_fighting() ) me->start_busy(1);
    if( obj->is_character() )
	message_vision( "$N將$n扶了起來揹在背上。\n", me, obj );
    else
	message_vision( sprintf("$N%s一%s$n。\n", 
	    from_ob==environment(me)? "撿起":
		(from_ob->is_character() ?
		"從" + from_ob->name() + "身上" + (equipped? "除下" : "搜出"):
		"從" + from_ob->name() + "中拿出"),
		obj->query("unit")), me, obj );
    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : get <物品名稱> [from <容器名>]
 
這個指令可以讓你撿起地上或容器內的某樣物品。
 
HELP
    );
    return 1;
}
 
