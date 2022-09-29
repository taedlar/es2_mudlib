/*  ccmd.c - the cmd to use clan functions
        
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
#include <ansi.h>
inherit F_CLEAN_UP;

varargs int clan_line(string msg,int emote);

int main(object me, string arg)
{
    object who, obj;
    string target;

    // function 1: check all clans
    if( !arg ) {
	seteuid( getuid() );
	write("目前已有的幫派如下: \n\n");
        write( read_file("/obj/CLAN") );
	write("\n欲加入一個幫派請找長老級以上玩家申請加入幫派。\n");
	return 1;
    }

    seteuid( getuid() );

    // function 2: verify - get clan symbol for a clan member
    if( arg == "verify" ) {	
	if( !me->query("clan/clan_name") ) 
	    return notify_fail("你並非幫派成員。\n");
	if( !present("clan symbol", me) ) {
	    obj = new("/obj/clan_symbol");
	    tell_object(me, "你得到一個新的幫派信物。\n");
	    obj->move(me);
	    return 1; }
	else return notify_fail("你已經有一個信物了。\n");
    }

    // function 3: leave - leave this clan
    if( arg == "leave" ) {
	if( me->query("clan/clan_level") > 1 )
	    return notify_fail("請先把職位降至一般幫眾。\n");
	log_file("CLAN",sprintf("[%s] %s: %s(%s) leaves\n",
	    ctime(time()), me->query("clan/clan_name"), me->name(),
	    me->query("id")) );
	clan_line("本人宣佈, 即刻起退出" + me->query("clan/clan_name")
	    + "。\n");
	me->delete("clan/clan_name");
	me->delete("clan/clan_level");
	me->delete("clan/nick");
	me->delete("clan/rank");
	if( obj = present("clan symbol", me) )
	    destruct(obj);
#ifdef SAVE_USER
	me->save();
#endif
	return 1;
    }
 
    // following function should classify function name
    if( sscanf(arg, "%s %s", arg, target)!=2 ) 
	return notify_fail("請用help ccmd查詢正確的指令格式。\n");

    // function 4: nick - change clan nickname
    if( arg == "nick" ) {
	me->set("clan/nick", target);
	tell_object(me, "Ok.\n");
	return 1 ;
    }

    // searching target
    //who = present(target, environment(me));
    who = find_player(target);
    if( !who || !userp(who) ) return notify_fail("沒有這個人。\n");

    // function 5: join - apply to join a clan
    if( arg == "join" ) {
        if( me->query("clan/clan_name") ) 
	    return notify_fail("你已加入過幫派，請先脫離原來幫派。\n");
	if( who->query("clan/clan_level") < 2 )
	    return notify_fail("此人並非幫派長老。\n");

	if( !present(target, environment(me)) )
	    return notify_fail("這裡沒有這個人。\n");

	message_vision(HIW"$N向$n申請加入"+who->query("clan/clan_name")
	+"。\n"NOR, me, who);
	me->set_temp("clan/apply", 1);
	return 1;
    }

    // following cmds only clan member can use them
    if( !me->query("clan/clan_name") ) return notify_fail("你並非幫派成員。\n");
    // the cmds need clan_level > 1 
    if( me->query("clan/clan_level") < 2 )
	return notify_fail("錯誤指令用法，或幫派職等不足。\n");    

    // function 6: accept - accept somebody join my clan
    if( arg == "accept" ) {
	if( !arg ) return notify_fail("你想收誰入幫??\n");

        if( !present(target, environment(me)) )
            return notify_fail("這裡沒有這個人。\n");

	if( who->query_temp("clan/apply") ) {
	    obj = new("/obj/clan_symbol");
	    obj->move(who);
	    seteuid( geteuid() );
	    who->delete_temp("clan/apply");
	    who->set("clan/clan_name", me->query("clan/clan_name"));
	    who->set("clan/rank", "幫眾");
	    who->set("clan/nick", "無");
	    who->set("clan/clan_level", 1);

	    log_file("CLAN", sprintf("[%s] %s: %s(%s) accepts %s(%s)\n",
		ctime(time()), me->query("clan/clan_name"), me->name(),
		me->query("id"), who->name(), who->query("id")));

	    tell_object(who, HIY+me->name() + "交給你一個幫派信物。\n"NOR);
	    clan_line("歡迎"+who->name()+"加入" +
		me->query("clan/clan_name") + "!!\n"); }
        else {
            tell_object(me, "此人並未向你申請加入幫派。\n");
	}
	return 1;
    }

    // check if same clan
    if( who->query("clan/clan_name") != me->query("clan/clan_name") )
	return notify_fail("他不是本幫成員。\n");

    // function 7: kickout - kickout someone from clan members  
    if( arg == "kickout" ) {
	if( target == me->query("id") ) 
	    return notify_fail("要叛幫請用: ccmd leave.\n");
	if( who->query("clan/clan_level") == 2 
	&& me->query("clan/clan_level") != 3 ) 
	    return notify_fail("長老必需由幫主親自處理。\n");
	if( who->query("clan/clan_level") == 3 )
	    return notify_fail("不行。\n");

        seteuid( getuid() );
	if( who->query("clan/clan_level") == 2 ) {
	    // 重設長老數目
	    me->add("clan/elder", -1); }
	clan_line("即刻起, " + who->name() + "不再是本幫幫眾。\n");
	log_file("CLAN",sprintf("[%s] %s: %s(%s) kickouts %s(%s)\n",
	    ctime(time()), me->query("clan/clan_name"), me->name(),
	    me->query("id"), who->name(), who->query("id")) );
	tell_object(who, HIY"你已經不再是" +
	    who->query("clan/clan_name") + "的幫眾了。\n"NOR);
	who->delete("clan/clan_name");
	who->delete("clan/clan_nick");
	who->delete("clan/clan_level");
	who->delete("clan/rank");
	if( obj = present("clan symbol", who) )
	    destruct(obj);
#ifdef SAVE_USER
	me->save();
	who->save();
#endif
	return 1;
    }

    // the cmds for clan leader
    if( me->query("clan/clan_level") < 3 )
	return notify_fail("錯誤指令用法，或幫派職等不足。\n");

    // function : promote - promote the level of clan member
    if( arg == "promote" ) {
	if( target == me->query("id") ) return notify_fail("不行!\n");
	if( who->query("class") == "commoner" )
	    return notify_fail("平民能力不足以擔任要職。\n");
	if( me->query_temp("clan/change_leader") ) {
	    me->delete_temp("clan/change_leader");
	    clan_line("本人已完成讓位儀式, 即刻起, 由"
		+ who->name() + "繼任" + me->query("clan/clan_name")
		+ me->query("clan/rank") + "之職。\n");

	    seteuid( getuid() );
	    who->set("clan/clan_level", 3);
	    who->set("clan/elder", me->query("clan/elder"));
	    who->set("clan/rank", me->query("clan/rank"));
	    tell_object(who, HIY"你的職位由長老升為" +
	    	me->query("clan/rank") + "。\n"NOR);
	    me->delete("clan/elder");
	    me->set("clan/clan_level", 2);
	    me->set("clan/rank", "長老");
	    tell_object(me, HIY"你的職位由" + who->query("clan/rank") +
		"降為長老。\n"NOR);
#ifdef SAVE_USER
	    me->save();
	    who->save();
#endif
	    log_file("CLAN", sprintf("[%s] %s changes %s: %s(%s) to %s(%s)\n",
		ctime(time()), me->query("clan/clan_name"), 
		who->query("clan/rank"), me->name(), me->query("id"),
		who->name(), who->query("id")) );
	    return 1;
	}

	if( who->query("clan/clan_level") == 2 ) {
	    if( who->query("level") < 40 ) {
		tell_object(me, "此人能力不足以擔當"
		    + me->query("clan/rank")+"之職。\n");
		return 1;
	    }
	    tell_object(me, HIY"注意!! 此舉將使" + who->name() +
		"取代你目前的" + me->query("clan/rank") + "地位," +
		"若是正確請再輸入一次。\n"NOR);
	    me->set_temp("clan/change_leader", 1);
	    return 1;
	}

	if( me->query("clan/elder") > 2 )
	    return notify_fail("本幫長老人數已滿, 無法再收新長老。\n");

	if( who->query("level") < 20 ) 
	    return notify_fail("此人能力不足以擔當長老之職。\n");

	clan_line("即刻起, " + who->name() + "升為" + 
	    me->query("clan/clan_name") + "長老。\n");
	seteuid( getuid() );
	me->add("clan/elder", 1);
	if( who->query("clan/elder") )
	    who->delete("clan/elder");
	who->set("clan/clan_level", 2);
	who->set("clan/rank", "長老");
	log_file("CLAN", sprintf("[%s] %s: %s(%s) promotes %s(%s)\n",
	    ctime(time()), me->query("clan/clan_name"), me->name(),
	    me->query("id"), who->name(), who->query("id")) );
#ifdef SAVE_USER
	me->save();
	who->save();
#endif
	return 1;
    }

    // function 9 : demote - demote the level of clan member
    if( arg == "demote" ) {
	if( target == me->query("id") ) 
	    return notify_fail("你必需先傳位給長老\n");

	if( who->query("clan/clan_level") == 1 )
	    return notify_fail("他的職等已是最低了, 無法再降。\n");

	clan_line("即刻起, 取消 " + who->name() + "長老之職, 降為一般幫眾。\n");
	seteuid( getuid() );
	me->add("clan/elder", -1);
	who->set("clan/clan_level", 1);
	who->set("clan/rank","幫眾");
	log_file("CLAN", sprintf("[%s] %s: %s(%s) demotes %s(%s)\n",
	    ctime(time()), me->query("clan/clan_name"), me->name(),
	    me->query("id"), who->name(), who->query("id")) );
#ifdef SAVE_USER
	me->save();
	who->save();
#endif
	return 1;
    }
}

varargs int clan_line(string msg, int emote) 
{
    object *usr, me, obj;
    int i, tmp;

    me = this_player();

    if(!msg) return notify_fail("你要對本幫派的同伴說什麼﹖\n");

    if( me->query_temp("pending/hidden") ) {
        tmp = me->query_temp("pending/hidden");
        me->delete_temp("pending/hidden");
    }

    if( !obj = present("clan symbol", me) )
	return notify_fail("請先取得幫派信物。\n");

    usr = filter_array(users(), "filter_clan", obj);
    if(sizeof(usr))
        for(i=0; i<sizeof(usr); i++)
            if (emote)
                message("system", HIY"【"+me->query("clan/clan_name")+"】"
                HIW+msg+"\n"NOR, usr[i]);
            else
                message("system", HIY"【"+me->query("clan/clan_name")+"】"
                +me->name()+"("+me->query("id")+")："HIW
                +msg+"\n"NOR, usr[i]);

    if( tmp > 0 ) {
        me->set_temp("pending/hidden", tmp); }
    return 1;
}

int help(object me)
{
write(@HELP
本指令為幫派專用指令, 指令格式:
   1. ccmd: 可查詢目前已成立的幫派及負責人
   2. ccmd verify: 補發信物
   3. ccmd leave: 叛幫
   4. ccmd nick <敘述>: 更改你在幫派的匿稱
   5. ccmd join <玩家id>: 申請加入幫派
HELP
	);

    if( me->query("clan/clan_level") > 1 ) {
write(@HELP
   6. ccmd accept <玩家id>: 接受加入幫派申請
   7. ccmd kickout <玩家id>: 取消該玩家之幫派資格
HELP
        );
    }

    if( me->query("clan/clan_level") > 2 ) {
write(@HELP
   8. ccmd promote <玩家id>: 提昇幫眾職等
   9. ccmd demote <玩家id>: 降低幫眾職等
HELP
	);
    }
	return 1;
}
