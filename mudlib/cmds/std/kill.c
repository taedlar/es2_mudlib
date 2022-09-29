/*  kill.c

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

inherit F_CLEAN_UP;

void do_kill(object, object);

int main(object me, string arg)
{
    object obj;
    string *killer, callname;

    if( me->query("life_form") == "ghost" )
	return notify_fail("人死了就一了百了, 快去找城隍復活吧!!\n");

    if( !arg ) return notify_fail("你想殺誰﹖\n");

    if( !(obj = present(arg, environment(me)))
    ||	!obj->is_character()
    ||	obj->is_corpse() )
	return notify_fail("這裡沒有這個人。\n");

    // add by dragoon
    if( wizardp(obj) ) return notify_fail("不行!!\n");

    if( environment(me)->query("no_fight") ) {
	if( !obj->query("unprotect_mark") ) {
	    tell_object(me, "這裡不准戰鬥。\n");
	    return 1;
	}
    }

    // add by dragoon
    if( userp(obj) && !interactive(obj) ) 
        return notify_fail("這人現在不在線上。\n");

    if( obj->query("no_fight") ) return notify_fail("不行。\n");

    if( me->is_busy() ) return notify_fail("你被敵人攻得手忙腳亂 ...\n");

    if(obj==me) return notify_fail("你那麼想死嗎﹖\n");

    me->set_temp("pending/hidden", 0);      // be visible
        
    if( ! me->is_fighting(obj)
    ||	! me->is_killing(obj) ) {
	switch( random(3) )
	{
	case 0:
	    message_vision("\n$N對著$n喝道﹕「" + obj->rank("rude")
		+ "﹗今日不是你死就是我活﹗」\n\n", me, obj);
	    break;
	case 1:
	    message_vision("\n$N對著$n喝道﹕「" + obj->rank("rude")
		+ "，納命來！」\n\n", me, obj);
	    break;
	case 2:
	    message_vision("\n$N對著$n喝道﹕「" + obj->rank("rude")
		+ "，明年的今天就是你的忌日！」\n\n", me, obj);
	    break;
	}
    }

    do_kill(me, obj);

    return 1;
}

void do_kill(object me, object obj)
{
    me->kill_ob(obj);

    if( obj->is_fighting(me) && obj->is_killing(me) ) return;

    if( userp(obj) )
    {
	/* 對玩家的 kill 指令 */
	obj->fight_ob(me);
	tell_object(obj, HIR "如果你要和" + me->name() 
	    + "性命相搏，請你也對這個人下一次 kill 指令。\n" NOR);
    }
    else {
	/* 對 NPC 的 kill 指令 */
	if( function_exists("accept_kill", obj)
	&&  ! obj->is_killing(me)
	&&  obj->accept_kill(me) ) {
	    return; }

	if( ! obj->is_killing(me) ) obj->kill_ob(me);
	else if( ! obj->is_fighting(me) ) obj->fight_ob(me);
    }
}

int help(object me)
{
  write(@HELP
指令格式 : kill <人物>
 
這個指令讓你主動開始攻擊一個人物﹐並且嚐試殺死對方﹐kill 和 fight 最大的
不同在於雙方將會真刀實槍地打鬥﹐也就是說﹐會真的受傷。由於 kill 只需單方
面一廂情願就可以成立﹐因此你對任何人使用 kill 指令都會開始戰鬥﹐通常如果
對方是 NPC 的話﹐他們也會同樣對你使用 kill。

當有人對你使用 kill 指令時會出現紅色的字樣警告你﹐對於一個玩家而言﹐如果
你沒有對一名敵人使用過 kill 指令﹐就不會將對方真的打傷或殺死( 使用法術或
偷襲除外)。

其他相關指令: fight

有關 fight 跟 kill 的區分請看 'help combat'.
HELP
    );
    return 1;
}
 
