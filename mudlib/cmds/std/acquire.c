/*  acquire.c

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

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string skill, who;
    object ob;

    if( !arg || sscanf(arg, "%s from %s", skill, who)!= 2)
        return notify_fail("指令格式﹕acquire <技能名稱> from <人物>\n");

    if( !objectp(ob = present(who, environment(me))) )
        return notify_fail("這裡沒有這個人。\n");

    // add by dragoon
    if( ob->query("no_fight") )
	return notify_fail("什麼??\n");

    if( !ob->query_skill(skill, 1) )
        return notify_fail(ob->name() + "並不會這種技能。\n");

    if( me->query_skill(skill, 1) > 0 )
        return notify_fail("你已經會這種技能了。\n");

    // Produce proper message according to relation between player and teacher.
    if( me->is_apprentice_of(ob) ) {
        message_vision("$N向$n磕頭行禮﹐恭恭敬敬地說道﹕師父在上﹐求您傳授徒兒"
            + "「" + to_chinese(skill) + "」﹗\n",
            me, ob);
    } else if( ob->query("age") > me->query("age") ) {
        message_vision("$N向$n屈膝下拜﹐說道﹕晚輩" + me->name() + "﹐望前輩指點"
            + me->rank("self") + "「" + to_chinese(skill) + "」﹗\n",
            me, ob);
    } else {
        message_vision("$N向$n拱手行禮﹐說道﹕請這位" + ob->rank("respectful") 
            + "指點" + me->rank("self") + "「" + to_chinese(skill) + "」﹗\n",
            me, ob);
    }

    notify_fail("看起來" + ob->name() + "不想教你這項技能。\n");
    return ob->acquire_skill(me, skill);
}

int help()
{
    write(@TEXT
指令格式﹕acquire <技能名稱> from <人物>

這個指令可以讓你向某人「求教」某種技能﹐不過對方願不願意教你﹐並不是你自己
可以決定的﹐通常武林人士不會輕易地教你武功﹐而且有時候即使教你﹐也不見得會
傾囊相授。
TEXT
    );
    return 1;
}
