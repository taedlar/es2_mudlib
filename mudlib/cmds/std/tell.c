/*  tell.c - the tell command

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

#define MIN_TELL_INTERVAL	3
#define MAX_TELL_MESSAGE	1024

inherit F_CLEAN_UP;

int help(object me);

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string target, msg, mud, uid, *ignore;
    object obj;
    int idle_time, tmp;

    if( time() - (int)me->query_temp("last_tell_time") < MIN_TELL_INTERVAL )
	return notify_fail("你必須休息一下才能再度使用這個指令。\n");
    me->set_temp("last_tell_time", time());

    if( !arg || sscanf(arg, "%s %s", target, msg)!=2 ) return help(me);
    if( strlen(msg) > MAX_TELL_MESSAGE )
	return notify_fail("你的訊息太長了，請長話短說。\n");

    if( sscanf(target, "%s@%s", target, mud)==2 ) {
	GTELL->send_gtell(mud, target, me, msg);
	write("網路訊息已送出，可能要稍候才能得到回應。\n");
	return 1;
    }

    obj = find_player(target);
    if( !obj
    || (!obj->visible(me) && wizardp(obj)) )
	return notify_fail("沒有這個人....。\n");

    // add by dragoon
    if( obj->query_temp("ignore/all") && !wizardp(me) ) {
        tell_object(me, "這個人現在不接收任何人的談話。\n");
        return 1;
    }
    
    if(!interactive(obj))
	return notify_fail("這個人現在沒辦法聽到你的訊息。\n");

    // check for ignore
    uid = getuid(me);
    ignore = obj->query("ignore");
    if( arrayp(ignore) && member_array(uid, ignore) != -1 )
         return notify_fail("對方不想聽你說話。\n");

    // add by dragoon to let tell work in sneak
    if( me->query_temp("pending/hidden") ) {
          tmp = me->query_temp("pending/hidden");
          me->delete_temp("pending/hidden");
    }

    write("你告訴" + obj->name(1) + "：" + HIG + msg + "\n" NOR);
        
    if( (idle_time = query_idle(obj)/60) > 2
    || in_input(obj)
    || in_edit(obj) ) {
	write(GRN+obj->name(1)+
	(idle_time>2 ? "已經發呆 "+idle_time+" 分鐘了" : "")+
	(in_input(obj) ? " <輸入文字中> " : "")+
	(in_edit(obj) ? " <編輯檔案中> " : "")+
	"，可能不會馬上回答你喔...\n"+NOR);
    }
    tell_object(obj, sprintf("%s告訴你﹕" + HIG + "%s\n" NOR,
	me->name(1)+"("+me->query("id")+")", msg));

    // reset hidden if it exist before use tell, -Dragoon
    if( tmp > 0 ) {
	me->set_temp("pending/hidden", tmp); }

    obj->set_temp("reply", me->query("id"));
    return 1;
}

int remote_tell(string cname, string from, string mud, string to, string msg)
{
    object ob;

    if( ob = find_player(to) ) {
	if( cname )
	    tell_object(ob, sprintf(HIG "%s(%s@%s)告訴你﹕%s\n" NOR,
		cname, capitalize(from), mud, msg ));
	else
	    tell_object(ob, sprintf(HIG "%s@%s 告訴你﹕%s\n" NOR,
		capitalize(from), mud, msg ));
	ob->set_temp("reply", from + "@" + mud);
	return 1;
    } else
	return 0;
}

int help(object me)
{
    write(@HELP
指令格式：tell <某人> <訊息>

你可以用這個指令和線上其他的使用者說話。如果對方正在發呆，或處於輸入文
字、編輯檔案等狀態，可能不會立刻看到你的訊息，系統會提示你這種狀況。

為了防止某些人利用這個指令惡意騷擾他人，所以系統在某些情況下會對這個指
令做一些限制。如果有人用 tell 指令不斷地送訊息給你，對你造成困擾(俗稱
洗銀幕)，你可以用 ignore 指令來拒絕所有來自這位使用者的 tell 訊息。

其他相關指令：reply, ignore
HELP
    );
    return 1;
}
