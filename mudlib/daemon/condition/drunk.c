/*  drunk.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

inherit CONDITION;

private void create()
{
    seteuid(getuid());
    DAEMON_D->register_condition_daemon("drunk");
}

void condition_update(object me, string cnd, mixed cnd_data)
{
    object *inv, ob, target;
    int drunk_cap;

    if( !intp(cnd_data) || cnd_data < 1 || !living(me) ) {
        me->delete_condition(cnd);
        return;
    }

    drunk_cap = me->query_attr("spi", 1) * me->query_stat_maximum("water") / 100;
    drunk_cap += me->query_skill("force");

    if( cnd_data >= drunk_cap ) {
        me->unconcious(" <爛醉如泥>");
        cnd_data = 0;
    } else if( cnd_data >= drunk_cap / 2 ) {
        switch(random(5)) {
        case 0:
            message_vision("$N醉得一蹋糊塗，「哇」地一聲吐了一地都是，噁 ...\n", me);
            me->consume_stat("food", cnd_data + random(cnd_data));
            cnd_data -= random(cnd_data);
            break;
        case 1:
            message_vision("$N醉得糊裡糊塗，將身上東西一件件脫了下來！\n", me);
            inv = all_inventory(me);
            inv->unequip();
            break;
        case 2:
            if( (ob = present("coin_money", me))
            ||	(ob = present("silver_money", me))
            ||	(ob = present("gold_money", me)) ) {
                message_vision("$N醉得亂七八糟，將身上銀兩銅錢「嘩啦」一聲撒了滿地！\n", me);
                ob->move(environment(me));
                break;
            }
        case 3:
            me->unconcious(" <爛醉如泥>");
            cnd_data = 0;
            break;
        default:
            cnd_data -= 1;
            me->supplement_stat("kee", me->query_attr("con"));
            break;
        }
    } else if( cnd_data > drunk_cap / 4 ) {
        switch(random(5)) {
        case 0:
            message_vision("$N腳下一個不穩，咕咚一聲摔了一交！\n", me);
            me->damage_stat("kee", random(5)+1);
            cnd_data -= 10;
            break;
        case 1:    // Randomly drop something ... :P
            inv = all_inventory(me);
	    // 防止把蛇, 幫派信物等設有no_drop的物件丟掉 -Dragoon
	    // inv[random(sizeof(inv))]->move( environment(me) );
	    target = inv[random(sizeof(inv))];
	    if( !target->query("no_drop") )
		target->move( environment(me) );
            break;
        case 2:
            message_vision("$N醉態可掬，依依呀呀地唱起歌來。\n", me);
            break;
        default:
            cnd_data -= 1;
            me->supplement_stat("kee", me->query_attr("con")/2 );
            break;
        }
    } else if( cnd_data > drunk_cap / 8 ) {
        switch(random(10)) {
        case 0:
            message_vision("$N滿臉通紅，「呃」地一聲打了個嗝。\n", me);
            cnd_data -= 3;
            break;
        case 1:
            message_vision("$N酒意上湧，揉了揉眼睛。\n", me);
            break;
        default:
            cnd_data -= 1;
	    // 微醉狀態精氣神狀態值的恢復是平常的三倍。
            me->heal_stat("gin", me->query_attr("dex") / 3 );
            me->heal_stat("kee", me->query_attr("con") / 3 );
            me->heal_stat("sen", me->query_attr("spi") / 3 );
            break;
        }
    }
    else cnd_data -=1;

    me->set_condition(cnd, cnd_data);
}

void apply_drunk(object me, int drunkness)
{
    int drunk, drunk_cap;

    drunk = me->query_condition("drunk") + drunkness;

    drunk_cap = me->query_attr("spi", 1) * me->query_stat_maximum("water") / 100;
    drunk_cap += me->query_skill("force");

    me->set_condition("drunk", drunk);
    if( drunk >= drunk_cap / 2 )
        tell_object(me, "你已經喝得天旋地轉，搖搖欲墜了，再喝下去定會醉倒。\n");
    else if( drunk >= drunk_cap / 4 )
        tell_object(me, "你已經有七分醉意了，眼睛看出去都是朦朧朧的一片。\n");
    else if( drunk >= drunk_cap / 8 )
        tell_object(me, "你覺得有些醉意了。\n");
}

