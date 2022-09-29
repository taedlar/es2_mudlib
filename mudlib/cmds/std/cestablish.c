/*  cestablish.c - to establish a clan

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

int main(object me, string arg)
{
	object who;

	if( !arg ) {
		if( me->query("level") < 50 
		|| me->query_score("reputation") < 60000 ) return 
			notify_fail("只有聲望夠高的頂級玩家才有建幫的資格。\n");

		if( me->query("clan/create") ) return 
			notify_fail("每個人一生只允許\創造一次新幫派。\n");

		if( me->query("clan/clan_level") ) return 
			notify_fail("你已經是幫派成員了, 請先脫離原來幫派。\n");

		if( me->query_temp("clan/sign_number") < 2 ) return
			notify_fail("你至少得找兩個副手來幫忙才能建幫。\n");
		write("目前已有的幫派如下: \n\n");
		seteuid(ROOT_UID);
		write( read_file("/obj/CLAN") );
		write("\n在幫名決定之前, 可隨時以指令:q脫離建幫手續。請\n");
		write("創一具風格的全新幫派。請輸入幫派名: ");
		input_to("get_clan_name", me);
		return 1;
	}

	if( sscanf(arg, "with %s", arg) ) {
		if( me->query("level") < 44 ) return 
			notify_fail("你的等級不足以參與建幫。\n");

		if( me->query("clan/clan_level") ) return 
			notify_fail("你已經是幫派成員了, 無法幫助外人建幫。\n");

		if( me->query("clan/signed") ) return 
			notify_fail("你已經幫忙成立過幫會了。\n");

		who = present(arg, environment(me));

		if( who == me ) return notify_fail("請找自己以外的人幫忙。\n");

		if( !objectp(who) || !living(who) ) 
			return notify_fail("這裡沒有這個人。\n");

		message_vision(HIW"$N決定協助$n創立一個新幫派。\n"NOR
		, me, who);

		who->add_temp("clan/sign_number", 1);
		me->set("clan/signed", 1);
#ifdef SAVE_USER
        me->save();
#endif
		return 1; }
	else {
		tell_object(me, 
		"指令格式: cestablish 或是 cestablish with <某人>\n");
		return 1;
	}
}

private void get_clan_name(string arg, object me)
{
        seteuid(getuid(me));
	me->set("clan/clan_name", arg);
	write("您確定要用這當您的幫派名嗎(y/n/q)﹖");
	input_to("confirm_name", me);
}

private void confirm_name(string yn, object me)
{        
	if( yn=="" ) {
		write("您確定要用這當您的幫派名嗎(y/n)﹖");
                input_to("confirm_name", me);
                return;
	}
	if( yn == "q" ) {
		write("那麼....下次吧。\n");
		return;
	}

        if( yn[0]!='y' && yn[0]!='Y' ) {
                write("那麼請重新輸入您想要的幫派名字: ");
                input_to("get_clan_name", me);
                return;
        }
	seteuid(ROOT_UID);
	// keep in /log/CLAN
	log_file("CLAN", sprintf("[%s] %s 由 %s(%s) 創立\n",
        ctime(time()), me->query("clan/clan_name"), me->name(),
        me->query("id")) );
	// write in /obj/CLAN for player to query clan data
	write_file("/obj/CLAN", sprintf("[%s] %s 由 %s(%s) 創立\n",
	ctime(time()), me->query("clan/clan_name"), me->name(),
	me->query("id")) );
	write("請輸入這個幫派領導人的稱號(如: 幫主, 門主, 掌門): ");
	input_to("get_clan_rank", me);
}

private void get_clan_rank(string arg, object me)
{
	if( strlen(arg) > 4 ) {
		write("稱號過長, 請重新輸入: ");
		input_to("get_clan_rank", me);
	}
	seteuid(getuid(me));
	me->set("clan/rank", arg);
	me->set("clan/nick", "第一代大頭目");
	me->set("clan/clan_level", 3);
	me->set("clan/elder", 0);
	write("您確定要用這當您的幫派稱號嗎(y/n)﹖");
	input_to("confirm_rank", me);
}

private void confirm_rank(string yn, object me)
{
	object mark;

	if( yn=="" ) {
		write("您確定要用這當您的幫派稱號嗎(y/n)﹖");
                input_to("confirm_rank", me);
                return;
        }

        if( yn[0]!='y' && yn[0]!='Y' ) {
                write("那麼請重新輸入您想要的幫派稱號: ");
                input_to("get_clan_rank", me);
                return;
        }
#ifdef SAVE_USER
        me->save();
#endif
	message("system", HIW"天朝公告: "+me->query("clan/clan_name")
	+"已由"+me->name()+"創立完成, 開始招收幫眾!!\n"NOR, users());
	mark = new("/obj/clan_symbol");
	mark->move(me);
	return;		
}


int help(object me)
{
write(@HELP
本指令用途為成立幫派, 想成立幫派的人(以下以幫主稱之)首先需
找人(以下以幫眾稱之)協助一起進行, 方能成立新幫派。在成立幫
派之初, 幫眾人數必需在兩人以上, 而各項成立幫派之限制請參閱
: help clan, 指令格式如下:
1. 幫眾
   cestablish with <某人>
2. 幫主
   cestablish
   並隨指示填入幫派各項基本資料。>
HELP
    );
    return 1;
}
