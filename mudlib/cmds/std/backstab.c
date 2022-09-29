/*  backstab.c - the backstab command

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

#define DELAY_COUNTDOWN         3

#define INITIAL_COUNTDOWN       DELAY_COUNTDOWN + 1
#define ENGAGE_COUNTDOWN        DELAY_COUNTDOWN + 2
#define DOIT_COUNTDOWN          DELAY_COUNTDOWN + 3

int do_backstab(object, object);
int halt_backstab(object me);

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{       
    object victim;

    if( me->query_stat("fatigue")>=me->query_stat_maximum("fatigue") )
        return notify_fail(me->name() + "厭倦了這種偷雞摸狗的行為，不願意執行這個命令。\n");

    if( me->query("life_form") == "ghost" )
        return notify_fail("人死了就一了百了, 快去找城隍復活吧!!\n");

    if( me->is_busy() )
        return notify_fail("請先用 halt 停止你正在做的事。\n");

    if( me->is_fighting() )
        return notify_fail("你已經在戰鬥中了。\n");

    if( !arg
    ||  arg == "corpse"
    ||  !(victim = present(arg, environment(me)))
    ||  victim==me
    ||  !victim->is_character() )
        return notify_fail("你要偷襲誰？\n");

    if( environment(me)->query("no_fight") ) {
        if( !victim->query("unprotect_mark") ) {
            tell_object(me, "這裡不准戰鬥。\n");
            return 1;
        }
    }
    
    // add by dragoon
    if( userp(victim) && !interactive(victim) ) 
        return notify_fail("這人現在不在線上。\n");

    if( victim->query("no_fight") ) return notify_fail("不行。\n");

//    if( !arg ) return notify_fail("你要偷襲誰？\n");

    if( me->skill_mapped("backstab") )
    {
        mapping weapon = me->query_temp("weapon");
        string skill;

        if( !mapp(weapon) || !sizeof(weapon) ) skill = "unarmed";
        else {
            skill = "dagger";
            if( !objectp(weapon["dagger"]) )
                return notify_fail("你只能以空手或匕首類武器進行偷襲。\n");
        }
        if( ! me->skill_mapped(skill) )
            return notify_fail("你必須 enable 你的攻擊技能。\n");
    }
    else {
        write("你必須 enable 「" + to_chinese("backstab") + "」才能進行偷襲。\n");
        return 1;
    }

    // 讓玩家可以利用stake來練backstab -Dragoon
    if( !living(victim) && !victim->query("TARGET_STAKE") ) {
        write("你不需要偷襲這個傢伙，直接上啊！\n");
        return 1;
    }

    if( victim->query("TARGET_STAKE")
    && me->query_skill("backstab") > 40 ) {
        write("假人類的對象只能讓你習得粗淺的偷襲暗算技巧！\n");
        return 1;
    }

    me->set_temp("pending/hidden", 0);
    write("你開始不動聲色地從" + victim->name() + "背後慢慢靠近 ... 。\n\n");

    me->set_temp("backstab_countdown", INITIAL_COUNTDOWN);
    me->start_busy( (: do_backstab, victim :), (: halt_backstab :) );

    return 1;
}

int do_backstab(object victim, object me)
{
    int countdown, ability, defend, bonus;

    if( !me ) return 0;

    /* 檢查要偷襲的對象是否還在 */
    if( !victim || environment(me) != environment(victim) ) {
        tell_object(me, "\n...  真可惜，那個傢伙已經不在這裡了。\n");
        return 0;
    }

    /* 取得偷襲者的偷襲程序倒數計時值 */
    countdown = me->query_temp("backstab_countdown");
    if( ! countdown ) countdown = 4;

    /* 在戰鬥狀態？ */
    if( me->is_fighting() ) {
        tell_object(me, "\n可惡，還是先應付眼前的敵人吧。\n");
        return 0;
    }

    switch(countdown)
    {
    case INITIAL_COUNTDOWN:
        /* 第一階段的測試 */
        ability = 3 * (me->query_attr("dex") + me->query_skill("backstab"));
        defend = victim->query_ability("awarness")
                 + random(victim->query_stat("gin"));

        if( living(victim) && defend > ability ) {
            write("你被發現了！\n");
            victim->catch_backstab(me);
            return 0;
        }

        /* 對方還沒有發現，進入下一階段 */
        tell_object(me, CYN "\n看起來" + victim->name()
                + "還沒有察覺你的舉動 ... 。\n\n" NOR);
        me->set_temp("backstab_countdown", ENGAGE_COUNTDOWN);
        me->improve_skill("backstab", 1 + random(me->query_attr("int")));
        return 1;

    case ENGAGE_COUNTDOWN:
        /* 第二階段的測試 */
        ability = 2 * (me->query_attr("dex") + me->query_skill("backstab"));
        defend = victim->query_ability("awarness")
                 + random(victim->query_stat("gin"));

        if( living(victim) && defend > ability ) {
            message_vision(HIY "\n$N發現$n在$P背後想偷施暗算！\n" NOR,
                        victim, me);
            me->kill_ob(victim);
            me->start_busy(3);
            write(HIR "\n糟糕！被發現了！\n\n" NOR "你被"
                        + victim->name() + "攻了個措手不及！\n\n");

            /* 被偷襲者的反擊 */
	    // 降低失敗的傷害和命中率, for thief's low hp -dragoon
            victim->kill_ob(me);
            victim->add_temp("apply/attack", 2 * victim->query_level() );
            victim->add_temp("apply/damage", victim->query_level() );
            victim->attack(me);
            victim->add_temp("apply/attack", -2 * victim->query_level() );
            victim->add_temp("apply/damage", -victim->query_level() );

            /* 偷襲者的代價 */
            me->gain_score("mortal sin", 1);
            me->set_temp("backstab_countdown", DELAY_COUNTDOWN);
            return 1;
        }
        write(CYN "\n你成功\地來到" + victim->name() + "的背後，準備動手了 ...\n\n" NOR);
        me->improve_skill("backstab", 1 + random(me->query_attr("int")));
        me->gain_score("mortal sin", random(10)+1);
        me->set_temp("backstab_countdown", DOIT_COUNTDOWN);
        return 50;

    case DOIT_COUNTDOWN:
        message_vision(HIY "\n$N從$n的背後一躍而出，喝道：「去死吧！"
                + victim->rank("rude") + "！」\n\n" NOR, me, victim);
        tell_object(victim, "不妙！你被" + me->name() + "從後面偷襲了！\n");
        me->kill_ob(victim);

        /* 偷襲成功的威力 */
        bonus = 10 + me->query_skill("backstab")
                + 2 * me->query_skill("killerhood");

        me->add_temp("apply/damage", bonus/2);
        me->add_temp("apply/attack", bonus);
        me->attack(victim);
        if( random(victim->query_ability("defense"))
                < me->query_skill("killerhood") ) {
            message_vision(HIY "$N偷襲$n得手，見$p一時反應不過來，臉露獰笑"
                        "，老實不客氣地往前進逼一步。\n" NOR,
                        me, victim);
            /* 殺手本能，追加兩下 */
            me->attack(victim);
            me->attack(victim);
        }
        me->add_temp("apply/damage", - bonus/2);
        me->add_temp("apply/attack", - bonus);

        victim->kill_ob(me);

        /* 給予經驗值 */
        me->improve_skill("backstab", 1 + random(me->query_attr("int")*2) );
        if( me->query_learn("killerhood") )
            me->improve_skill("killerhood", 1 + random(me->query_attr("dex")) );
        me->gain_score("mortal sin", bonus);

        return 0;

    default:
        /* 若在延遲中，執行倒數計數 */
        if( countdown > 0 && countdown <= DELAY_COUNTDOWN ) {
            me->set_temp("backstab_countdown", --countdown);
            return countdown;
        }
        return 0;
    }
}

int halt_backstab(object me)
{
    int countdown = me->query_temp("backstab_countdown");

    if( countdown > DELAY_COUNTDOWN ) {
        write("你打消了偷襲的念頭。\n");
        me->supplement_stat("fatigue", 5);
        return 1;
    }
    write("太遲了，你手忙腳亂地想脫身，但是沒空！\n");
    return 0;
}

int help()
{
    write(@TEXT
指令格式：backstab <目標>

這個指令讓你對敵人進行偷襲暗算，偷襲成功可以對敵人造成很大的傷害，但是
失敗也會讓你付出不少代價。

偷襲的成功率主要跟你的「偷襲暗算技倆」這個技能有關，而傷害力則受此一技
能與「殺手本能」的影響，偷襲時只能空手，或使用匕首等短兵刃。
TEXT
    );
        return 1;
}
