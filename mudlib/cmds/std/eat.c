/*  eat.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object ob;

    if( me->is_busy() ) return 0;

    if( !arg || arg=="" ) return notify_fail("你要吃什麼﹖\n");

    if( !me->query_stat_maximum("kee") )
        return notify_fail("你沒有形體﹐無法進食。\n");

    ob = present(arg, me);
    if( !ob ) ob = present(arg, environment(me));
    if( !ob ) return notify_fail("這裡沒有這樣東西 ...。\n");
    if( inherits(COMBINED_ITEM, ob) && !ob->query_amount() )
	return notify_fail("你得先把口中的食物吞下去！\n");

    notify_fail("這樣東西不能吃。\n");
    if( ob->stuff_ob(me) ) {
        me->start_busy(1);
        return 1;
    }
    return 0;
}

int help()
{
    write(@TEXT
指令格式：eat <物品>

這個指令用來讓你的人物進食或服用藥物，es2 雖然設定了食物，但是通常你不必像
真實生活一樣一天吃三餐，只要肚子餓時再吃就可以了。

戰鬥中吃食物會失去一次攻擊機會。
TEXT
    );
    return 1;
}
