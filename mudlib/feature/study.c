/*  study.c

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

int study_content(object me);
int halt_study(object me, object from, string how);

int study_ob(object me)
{
    mapping req, content;
    string t, msg;
    int min, max, to_learn = 0;

    if( this_player()->is_busy() )
	return notify_fail("你正忙著其他事呢！\n");
    if( this_player()->is_fighting() )
	return notify_fail("你還是先應付眼前的敵人吧！\n");
    req = query("required/attribute");
    if( mapp(req) ) {
	foreach(t, min in req)
	    if( this_player()->query_attr(t, 1) < min ) {
		write("你的" + to_chinese(t) + "不夠，所以對這上面記載的"
			"內容完全無法體會。\n");
		return 1;
	    }
    }
    req = query("required/skill");
    if( mapp(req) ) {
	foreach(t, min in req)
	    if( this_player()->query_skill(t, 1) < min ) {
		write("你在" + to_chinese(t) + "上的造詣還不夠理解這上面所記載的內容。\n");
		return 1;
	    }
    }

    /* 只有符合條件的人才知道記載的內容有沒有用 .... */
    content = query("content");
    if( !content )
	return notify_fail("這上面沒有記載什麼有用的內容。\n");

    foreach(t, max in content)
	if( this_player()->query_skill(t, 1) < max ) to_learn++;
    if( ! to_learn ) {
	write("這上面記載的內容對你而言都了無新意 ...。\n");
	return 1;
    }

    this_player()->start_busy((: study_content :), (: halt_study :));
    if( stringp(msg = query("study_msg")) )
	message_vision(msg, this_player());
    else
	message_vision("$N開始聚精會神地研讀" + name() + "上面的內容。\n",
		this_player());

    return 1;    
}

int study_content(object me)
{
    mapping content;
    int skill, archaism, cost, max, point, gain = 0;
    string t;

    if( ! mapp(content = query("content"))
    ||	! sizeof(content) ) return 0;

    /* 使用讀書識字的技能 */
    skill = me->query_skill("literate");

    /* 耗精神 1 - 10 點 */
    cost = 1 + (9 - me->query_attr("wis")/3);
    if( cost < 1 ) cost = 1;

    /* 若需要古文知識，重新計算技能及損耗 */
    if( (archaism = query("required/skill/archaic attainment")) ) {
	skill = skill/10 + me->query_skill("archaic attainment");
	cost *= 2;
    }

    if( me->query_stat("sen") < cost
    ||	me->query_stat("gin") < cost
    ||	me->query_stat("fatigue") >= me->query_stat_maximum("fatigue") ) {
	tell_object(me, "你覺得精神不繼，無法再繼續研讀了。\n");
	return 0;
    }

    me->consume_stat("gin", cost);
    me->consume_stat("sen", cost);

    point = me->query_attr("int") / sizeof(content);
    foreach(t, max in content) {
	if( max > skill ) max = skill;	
	if( me->query_skill(t, 1) < content[t]
	&&  random( point + skill ) >= max/2 ) {
	    me->improve_skill(t, 1 + random(point));
	    gain++;
	}
    }

    if( gain ) {
	me->improve_skill("literate", 1 + random(me->query_attr("int")));
	if( archaism )
	    me->improve_skill("archaic attainment",
		1 + random(me->query_attr("int")));
	me->supplement_stat("fatigue", gain);
	me->damage_stat("sen", gain);
    }

    return 1;
}

int halt_study(object me, object from, string how)
{
	string msg;
	if( stringp( msg = query("halt_msg") ) ) 
		write( msg );
	else
		write("你停止研讀" + name() + "上面記載的內容。\n");
		return 1;
}
