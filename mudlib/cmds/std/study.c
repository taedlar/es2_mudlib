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

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object ob;

    if( me->is_busy() ) return notify_fail("你必須用 halt 停止正在做的事！\n");

    if( !arg || arg=="" ) return notify_fail("你要研讀什麼？\n");

    ob = present(arg, me);
    if( !ob ) ob = present(arg, environment(me));
    if( !ob ) return notify_fail("這裡沒有這樣東西 ...。\n");

    return ob->study_ob(me);
}

int help()
{
    write(@TEXT
指令格式：study <物品>

這個指令讓你的人物研讀一些書籍或文件之類的物品。

研讀的過程是相當耗時間及精神的，而且隨著持續的研讀，會增加人物對這些
活動的「疲勞」(fatigue)，而且這些疲勞是心理上的，只能隨著時間而降低。
TEXT
    );
    return 1;
}
