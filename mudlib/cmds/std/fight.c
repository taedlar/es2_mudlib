/*  fight.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <ansi.h>
#include <char.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object obj, old_target;
    int rg, rk, rgm, rkm;

    if( me->query("life_form")=="ghost" )
        return notify_fail("人死了就一了百了, 快去找城隍復活吧!!\n");

    if( environment(me)->query("no_fight") )
	return notify_fail("這裡禁止戰鬥。\n");
 
    if( !arg
    ||	!objectp(obj = present(arg, environment(me)))
    ||	!obj->is_character()
    ||	obj->is_corpse() )
	return notify_fail("你想攻擊誰﹖\n");

    if( obj->query("no_fight") ) return notify_fail("不行。\n");

    // 不可攻擊已斷線玩家 -Dragoon
    if( userp(obj) && !interactive(obj) ) 
        return notify_fail("這人現在不在線上。\n");

    if( !me->visible(obj) )
	return notify_fail( obj->name() + "看不見你，不知道你在向他挑戰。\n");

    if( me->is_apprentice_of(obj) )
	return notify_fail( obj->name() + "是你的師父﹗\n");

    if( obj->is_fighting(me) )
	return notify_fail("加油﹗加油﹗加油﹗\n");

    if( !living(obj) && inherits(F_ATTACK, obj) )
	return notify_fail( "和" + obj->name() + "戰鬥？\n"); 

    if( obj==me ) return notify_fail("你不能攻擊自己。\n");

    if( userp(obj) && (object)obj->query_temp("pending/fight")!=me ) {
	message_vision("\n$N對著$n說道﹕" 
	    + me->rank("self") + me->name() + "，領教這位"
	    + obj->rank("respectful") + "的高招﹗\n\n", me, obj);

	if( objectp(old_target = me->query_temp("pending/fight"))
	&&  obj != old_target
	&&  environment(old_target)==environment(me) )
	    message_vision("\n$N向$n抱拳做揖，說道："
		+ me->rank("self") + "改日再行請教。\n", me, old_target);
	me->set_temp("pending/fight", obj);

	tell_object(obj, YEL "如果你願意和對方進行比試，請你也對" + me->name() + "("+(string)me->query("id")+")"+ "下一次 fight 指令。\n" NOR);
	write(YEL "由於對方是由玩家控制的人物，你必須等對方同意才能進行比試。\n" NOR);
	return 1;
    }

    if( obj->query("civilized") ) {
	if( me->is_fighting() )
	    message_vision( "\n$N對著$n說道：一併領教這位"
		+ obj->rank("respectful") + "的高招﹗\n\n", me, obj);
	else
	    message_vision( "\n$N對著$n說道﹕" 
		+ me->rank("self") + me->name() + "，領教這位"
		+ obj->rank("respectful") + "的高招﹗\n\n", me, obj);

	if( obj->query_temp("pending/fight")==me ) {
	    obj->start_busy(1);
//	delete else, 讓雙方都busy而非先下fight指令的人busy only -dragoon
//	else 
	    me->start_busy(1);
	}

	if( !userp(obj) && !obj->accept_fight(me) ) {
	    write("看起來" + obj->name() + "並不想跟你較量。\n");
	    return 1;
	}

	me->fight_ob(obj);
	obj->fight_ob(me);
    } else {
	message_vision("\n$N大喝一聲，開始對$n發動攻擊﹗\n\n", me, obj);
	me->fight_ob(obj);
	obj->kill_ob(me);
    }

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : fight <人物>
 
這個指令讓你向一個人物「討教」或者是「切磋武藝」﹐這種形式的戰鬥純粹是
點到為止﹐因此只會消耗體力﹐不會真的受傷﹐但是並不是所有的  NPC 都喜歡
打架﹐因此有許\多狀況你的比武要求會被拒絕。
 
其他相關指令: kill

PS. 如果對方不願意接受你的挑戰﹐你仍然可以逕行用 kill 指令開始戰鬥﹐有
    關 fight 跟 kill 的區分請看 'help combat'.
HELP
    );
    return 1;
}
 
