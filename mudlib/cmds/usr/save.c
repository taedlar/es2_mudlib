/*  save.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <login.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object link_ob;
    int last_save, delta_t;
    
    seteuid(getuid());

    if( !objectp(link_ob = me->link()) )
        return notify_fail("你不是經由正常連線進入﹐不能儲存。\n");

    if( environment(me)->query("valid_startroom") ) {
        me->set("startroom", base_name(environment(me)));
        write("當你下次連線進來時﹐會從這裡開始。\n");
    }

    if( !wizardp(me)
    &&  (last_save = me->query_temp("last_save"))
    &&  (delta_t = (time()/60 - last_save)) < 5 ) {
        return notify_fail("不用急, " +delta_t+ " 分鐘前你才剛儲存過, " +(5 - delta_t)+ " 分鐘後再試吧。\n");
    }

#ifdef SAVE_USER
    if( (int)me->save() ) {
        write("檔案儲存完畢。\n");
        me->set_temp("last_save", (int)(time()/60) );
        return 1;
    } else {
        write("儲存失敗。\n");
        return 0;
    }
#else
    return notify_fail("系統目前不儲存使用者資料。\n");
#endif
}

int help(object me)
{
    write(@HELP
指令格式﹕save

把你辛苦奮鬥的結果存起來。為了提升系統效率, 目前兩次 save 之時間間距最短為
五分鐘。 quit 時系統會試著幫你 save。
see also: quit, backup
HELP
    );
    return 1;
}
