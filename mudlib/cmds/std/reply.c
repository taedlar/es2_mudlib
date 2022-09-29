/*  reply.c

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
#include <net/dns.h>

#define MIN_REPLY_INTERVAL       3
#define MAX_REPLY_MESSAGE        1024

inherit F_CLEAN_UP;

int help(object me);

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string target, mud, uid, *ignore;
    object obj;
    int idle_time, tmp;

    if( !arg || arg=="" )
	return notify_fail("你要回答什麼﹖\n");

    if( !stringp(target = me->query_temp("reply")) )
	return notify_fail("剛才沒有人和你說過話。\n");

    if( time() - (int)me->query_temp("last_reply_time") < MIN_REPLY_INTERVAL )
        return notify_fail("你必須休息一下才能再度使用這個指令。\n");

    me->set_temp("last_reply_time", time());

    if( strlen(arg) > MAX_REPLY_MESSAGE )
        return notify_fail("你的訊息太長了，請長話短說。\n");

    if( sscanf(target, "%s@%s", target, mud)==2 ) {
	GTELL->send_gtell(lower_case(mud), lower_case(target), me, arg);
	write("網路訊息已送出﹐可能要稍候才能得到回應。\n");
	return 1;
    }

    if( (!obj = find_player(target)) || (!interactive(obj)) )
	return notify_fail("剛才和你說話的人現在無法聽見你﹐或者已經離開遊戲了。\n");

    uid = getuid(me);
    ignore = obj->query("ignore");

    // add by dragoon to let tell work in sneak
    if( me->query_temp("pending/hidden") ) {
        tmp = me->query_temp("pending/hidden");
        me->delete_temp("pending/hidden");
    }

        if (pointerp(ignore) && member_array(uid, ignore) != -1  )
         return notify_fail(sprintf("%s不想聽你的回答。\n",obj->name(1)));
        if( obj->query_temp("netdead") )
          return notify_fail(sprintf(HIG"%s斷線中...\n"NOR,obj->name(1)));
        write("你回答" + obj->name(1) + "(" + obj->query("id") + ")﹕"
                + HIG + arg + "\n" NOR);

    if ((idle_time = query_idle(obj)/60) > 2
    || in_input(obj)
    || in_edit(obj) ) {
                write(GRN+obj->name(1)+
                (idle_time>2 ? "已經發呆 "+idle_time+" 分鐘了" : "")+
                (in_input(obj) ? " <輸入文字中> " : "")+
                (in_edit(obj) ? " <編輯檔案中> " : "")+
                ", 可能不會馬上回答你喔...\n"+NOR);
    }

        tell_object(obj, sprintf("%s(%s)回答你﹕" + HIG + "%s\n"NOR,
                me->name(1), me->query("id"), arg ));

    // reset hidden if it exist before use tell, -Dragoon
    if( tmp > 0 ) {
        me->set_temp("pending/hidden", tmp); }

        obj->set_temp("reply", me->query("id"));
        return 1;
}

int help(object me)
{
        write(@HELP
指令格式﹕reply <訊息>

你可以用這個指令和剛才用 tell 或 reply 與你說話的使用者說話。對方若發呆三
分鐘以上, 或處於輸入文字狀態, 或編輯檔案狀態, 則系統會顯示於你。

see also : tell
HELP
        );
        return 1;
}
