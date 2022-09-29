/*  goto.c

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

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    int goto_inventory = 0, goto_object = 0;
    object obj, oldenv;
    string msg;

    SECURED_COMMAND;

    if( !arg ) return notify_fail("你要去哪裡﹖\n");

    if( sscanf(arg, "-i %s", arg) ) goto_inventory = 1;
    if( sscanf(arg, "-o %s", arg) ) goto_object = 1;

    if( !arg ) return notify_fail("你要去哪裡﹖\n");

    obj = find_player(arg);
    if(!obj) obj = find_living(arg);
    if(!obj && goto_object ) obj = present(arg, environment(me));
    if(!obj || !obj->visible(me)) {
        arg = resolve_path(me->query("cwd"), arg);
        if( !sscanf(arg, "%*s.c") ) arg += ".c";
        if( !(obj = find_object(arg)) ) {
            if( file_size(arg)>=0 ) {
                message("vision", "一陣煙霧湧起﹐" + me->name() + "的身影逐漸糢糊。\n", environment(me), me);
                return me->move(arg);
            }
            return notify_fail("沒有這個玩家、生物、或地方。\n");
        }
    }

    if(!goto_inventory && !goto_object && environment(obj))
        obj = environment(obj);

    if( !obj ) return notify_fail("這個物件沒有環境可以 goto。\n");

    oldenv = environment(me);
    if( !me->move(obj) )
        return notify_fail("因為某種理由，你的 goto 失敗了。\n");

    message("vision", "只見一陣煙霧過後﹐" + me->name() + "的身影已經不見了。\n", oldenv);
    message("vision", me->name() + "的身影突然出現在一陣煙霧之中。\n", environment(me), me);

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : goto [-i] <目標>
 
這個指令會將你傳送到指定的目標. 目標可以是一個living 或房間
的檔名. 如果目標是living , 你會被移到跟那個人同樣的環境.
如果有加上 -i 參數且目標是 living, 則你會被移到該 living 的
的 inventory 中.
 
HELP
    );
    return 1;
}
