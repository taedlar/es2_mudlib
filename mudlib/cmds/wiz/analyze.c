/*  analyze.c

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

int main(object me, string arg)
{
    object ob;
    string str;
    mapping apply, weapon;

    if( !arg ) ob = me;
    else {
        if( !(ob = find_player(arg))
        &&    !(ob = present(arg, environment(me)))
        &&    !(ob = find_living(arg)) )
            return notify_fail("這裡沒有 " + arg + " 這種生物。\n");
    }

    str = sprintf("%s的各項能力分析﹕\n", ob->name());
    str += HIY "\n<基本值 - 來自屬性>\n" NOR;
    str += sprintf("攻擊力\t\t攻勢 %d，技巧 %d，力道 %.2f 公斤\n",
	ob->query_ability("intimidate"),
	ob->query_ability("attack"),
	ob->query_strength("attack") / 1000.0);
    str += sprintf("防禦力\t\t守勢 %d，技巧 %d，強度 %.2f 公斤\n",
	ob->query_ability("wittiness"),
	ob->query_ability("defense"),
        ob->query_strength("defense") / 1000.0);
    str += sprintf("魔力\t\t能力值 %d﹐強度 %.2f Kw\n", ob->query_ability("magic"),
        ob->query_strength("magic") / 1000.0);
    str += sprintf("法力\t\t能力值 %d﹐強度 %.2f Kw\n", ob->query_ability("spell"),
        ob->query_strength("spell") / 1000.0);

    str += HIY "\n<修正值 - 來自技能>\n" NOR;
    if( ob->query_skill("force") && ob->skill_mapped("force")!="force" ) {
        int skill, modify;
        skill = ob->query_skill("force");
        modify = skill * ob->query_stat("kee") * 4;
        str += sprintf("使用內力 \t%s(攻擊強度 +%.2f Kg)\n",
            to_chinese(ob->skill_mapped("force")), (float)modify/1000.0);
    }
    if( mapp(weapon  = ob->query_temp("weapon")) && sizeof(weapon) ) {
        string term;
        str += "使用武器\t";
        foreach(term in keys(weapon)) {
        str += sprintf("%s﹐(攻擊能力 %+d)\n\t\t", weapon[term]->name() + "(" + term + ")",
                    ob->query_skill(term));
        }
    }
    else str += sprintf("徒手攻擊 \t(攻擊能力 %+d)\n", ob->query_skill("unarmed"));

    if( mapp(apply = ob->query_temp("apply")) ) {
        string term;
        int prop, k;

        str += HIY "\n<附加值 - 來自裝備，法術等影響>\n" NOR;
        k = 0;
        foreach(term, prop in apply) {
            if( !intp(prop) ) continue;
            if( prop ) {
                str += sprintf("  %-16s %+d%s", to_chinese(term), prop,
                    k%2==0 ? "\t" : "\n");
                k++;
            }
        }
    }
    str += "\n";

    write(str);
    return 1;
}

int help()
{
    write(@TEXT
指令格式﹕analyze [<對象>]

這個指令會列出一些有關指定對象的能力值﹐不指定對象時則列出你自己的
能力值。
TEXT
    );
    return 1;
}
