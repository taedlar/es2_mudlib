/*  identify.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <combat.h>

inherit F_CLEAN_UP;

mapping named_apply = ([
    "attack" : "攻擊能力值",
    "defense" : "防禦能力值",
    "str" : "膂力",
    "cor" : "膽識",
    "cps" : "定力",
    "int" : "悟性",
    "wis" : "慧根",
    "spi" : "靈性",
    "con" : "根骨",
    "dex" : "機敏",
    "damage" : "傷害力",
    "armor" : "防禦力",
]);

int main(object me, string arg)
{
    object ob;
    string type, sub_type;
    class damage_parameter dp;
    mapping apply;

    if( me->is_busy() ) return 0;

    if( !arg ) return notify_fail("你要鑑定什麼東西?\n");
    ob = present(arg, me);
    if( !ob ) return notify_fail("你要鑑定什麼東西?\n");
    if( !(type = ob->query("equipped")) )
        return notify_fail("你必須先裝備" + ob->name() + "才能鑑定.\n");
    if( sscanf(type, "%s/%s", type, sub_type) != 2 )
        return notify_fail("這類物品無法鑑定.\n");

    if( !wizardp(me) ) {
        if( me->query_stat("sen") < 10 )
            return notify_fail("你太累了.\n");
        me->consume_stat("sen", 10);
    }

    write(ob->long() + "\n");
    if( type=="weapon" ) {
        string s;
        mixed sk = ob->query("wield_as");
        
        write("可用技能：\n");
        if( stringp(sk) ) sk = ({ sk });
        foreach(s in sk)
            printf("  %s (%s)\n", to_chinese(s), s);
        dp = ob->query("damage/" + sub_type);
        printf("\n傷害力： %d - %d (力量修正 %d%%，乘數等級 %d)\n", 
            dp->roll + dp->multipler,
            dp->roll + dp->multipler * dp->range,
            dp->bonus, dp->multipler );
        apply = ob->query("apply_" + type + "/" + sub_type);
        if( mapp(apply) ) {
            string prop;
            mixed value;
            
            write("\n武器特性﹕\n");
            foreach(prop, value in apply) {
                if( !undefinedp(named_apply[prop]) )
                    prop = named_apply[prop];
                else
                    prop = to_chinese(prop);
                printf("  %-30s %O\n", prop, value);
            }
        }
    }
    if( type=="armor" ) {
        apply = ob->query("apply_" + type + "/" + sub_type);
        if( mapp(apply) ) {
            string prop;
            mixed value;
            
            write("護具特性﹕\n");
            foreach(prop, value in apply) {
                if( !undefinedp(named_apply[prop]) )
                    prop = named_apply[prop];
                printf("  %-30s %O\n", to_chinese(prop), value);
            }
        }
    }

    return 1;        
}

int help(object ob)
{
    write(@TEXT
指令格式: identify <物件>

這個指令可以讓你鑑定一些武器裝備等物品, 需要花費一些精神.
TEXT
    );
    return 1;
}
