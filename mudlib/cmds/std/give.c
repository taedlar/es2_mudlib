/*  give.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <command.h>
#include <login.h>

inherit F_CLEAN_UP;

int do_give(object me, object item, object who);

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string target, item;
    object obj, who, *inv, obj2, fwho;
    int i, amount;

    if(!arg) return notify_fail("你要給誰什麼東西﹖\n");

    if( !(sscanf(arg, "%s to %s", item, target)==2
        || sscanf(arg, "%s %s", target, item)==2) )
        return notify_fail("你要給誰什麼東西﹖\n");

    if( !objectp(who = present(target, environment(me)))
    ||  !living(who))
        return notify_fail("這裡沒有 \"" + target + "\" 這個人。\n");

    if(who==me) return notify_fail("你給自己東西幹麻﹖\n");

    // add by dragoon
    if( objectp(who) && who->query("no_fight") )
        return notify_fail("什麼??\n");

    if(sscanf(item, "%d %s", amount, item)==2)
    {
        if( !objectp(obj = present(item, me)) )    
            return notify_fail("你身上沒有這樣東西。\n");
        // 增加不能把玩家給人的檢查 -Dragoon
        if( obj->query("no_drop") || userp(obj) )
            return notify_fail("這樣東西不能隨便給人。\n");
        if( !obj->query_amount() )    
            return notify_fail( obj->name() + "不能被分開給人。\n");
        if( amount < 1 )
            return notify_fail("東西的數量至少是一個。\n");
        if( amount > obj->query_amount() ) 
            return notify_fail("你沒有那麼多的" + obj->name() + "。\n");
        else if( amount == (int)obj->query_amount() )
            return do_give(me, obj, who);
        else {
            seteuid(MUDLIB_UID);
            obj2 = new(base_name(obj));
            seteuid(getuid());
            obj2->set_amount(amount);
            if( do_give(me, obj2, who) ) {
                obj->add_amount(-amount);
                return 1;
            } else {
                destruct(obj2);
                return 0;
            }
        }
    }

    if( arg=="all" ) {
        foreach(object ob in all_inventory(me))
            do_give(me, ob, who);
        write("Ok.\n");
        return 1;
    }

    if(!objectp(obj = present(item, me)))
        return notify_fail("你身上沒有這樣東西。\n");

    return do_give(me, obj, who);
}

private int
do_give(object me, object item, object who)
{
    if( item->query("no_drop") )
        return notify_fail("這樣東西不能隨便給人。\n");

    if( who->over_encumbranced() )
        return notify_fail( who->name() + "已經負荷過重﹐再也搬不動任何東西了。\n");

    notify_fail("看起來" + who->name() + "並不願意收下這" + item->query("unit")
        + item->name() + "。\n");

    if( !userp(who) ) {
        if( !who->accept_object(me, item) ) return 0;
        if( item->value() ) {
            /* 給 NPC 錢的時候，直接將錢銷毀 */
            message_vision("$N拿出" + item->short() + "交給$n。\n", me, who);
            destruct(item);
            return 1;
        }
    }

    if( !item->move(who) ) return 0;

    if( wizardp(me) && (userp(who) && !wizardp(who)) )
        log_file( "static/GIVE", sprintf("[%s] %s gived %s to %s\n",
                ctime(time()), geteuid(me), item->name(), geteuid(who)) );
    message_vision("$N交給$n一" + item->query("unit") + item->name()
        + "。\n", me, who );

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : give <物品名稱> to <某人>
           give <某人> <物品名稱>

這個指令可以讓你將某樣物品給別人。
 
HELP
    );
    return 1;
}
