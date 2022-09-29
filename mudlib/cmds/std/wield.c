/*  wield.c

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

varargs int do_wield(object me, object ob);

int main(object me, string arg)
{
    string skill;
    object ob, *inv;
    int i, count;

    if( me->is_busy() ) return notify_fail("你現在沒有空﹗\n");

    if( !arg ) return notify_fail("你要裝備什麼武器﹖\n");

    if(arg=="all") {
        inv = all_inventory(me);
        for(count=0, i=0; i<sizeof(inv); i++) {
            if( inv[i]->query("equipped") ) continue;
            if(do_wield(me, inv[i]) ) count ++;
        }
        write("Ok.\n");
        return 1;
    }

    // Get skill type if specified.
    sscanf(arg, "%s as %s", arg, skill);

    if( !objectp(ob = present(arg, me)) )
        return notify_fail("你身上沒有這樣東西。\n");

    return do_wield(me, ob, skill);
}

varargs int do_wield(object me, object ob, string skill)
{
    string str;

    if( ob->wield(skill) ) {
        if( !stringp(str = ob->query("wield_msg")) )
            str = "$N裝備$n作武器。\n";
        message_vision(str, me, ob);
        return 1;
    } else
        return 0;
}

int help(object me)
{
    write(@HELP
指令格式﹕wield <武器名稱> [as <武器技能>]
 
這個指令讓你裝備某件物品作武器﹐你必需要擁有這樣物品。有些武器可以當成許多
種兵器使用﹐例如一把闊劍可以當劍使﹐也可以當刀耍﹐你可以用

wield broadsword as sword 或
wield broadsword as blade 來指定。其他握法如:

wield blade as twohanded blade
wield dagger as secondhand dagger
wield kris 2 as secondhand blade

某件武器的握法可能有一種以上，握法不同時引發的武器威力可能也不同，你可以在
握起後 identify 來獲得其他資訊。

HELP
    );
    return 1;
}
