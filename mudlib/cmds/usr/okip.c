/*  okip.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#pragma optimize
#pragma save_binary

#include <command.h>
#include <login.h>

//inherit F_CLEAN_UP;

void create()
{
    seteuid(getuid());
    set_reset(this_object(), 86400);
}


int main(object me, string str)
{
    object link_ob;
    string *args, *okip, ip;

    SECURED_COMMAND;

    if( stringp(str) ) args = explode(str, " ") - ({ "" });
    else args = ({});

    if( wizhood(me)=="(admin)" && sizeof(args) > 0 && args[0][0] != '-' ) {
	link_ob = FINGER_D->acquire_login_ob(args[0]);
	args = args[1..];
    }
    else link_ob = me->link();

    if( ! link_ob ) return notify_fail("找不到連線物件。\n");

    if( stringp(link_ob->query("okip")) )
	okip = explode(link_ob->query("okip"), ":");
    else
	okip = ({});

    if( ! sizeof(args) ) {
	write( sprintf( "%s 的 okip 目前設定為：\n\t%s\n",
		link_ob->query("id"),
		implode(okip, "\t\n")) );
	return 1;
    }

    switch(args[0])
    {
    case "-a":
	foreach(ip in args[1..])
	    if( member_array(ip, okip)== -1 ) okip += ({ ip });
	break;
    case "-d":
	okip -= args[1..];
	break;
    default:
	return notify_fail("指令格式：okip [-a|-d] 連線位置 ...\n");
    }

#ifdef	SAVE_USER
    link_ob->set("okip", implode(okip, ":"));
    link_ob->save();
    if( ! link_ob->body() ) destruct(link_ob);
#endif

    write("Ok。\n");
    return 1;
}

int help()
{
        write(@TEXT
指令格式：okip [-a|-d ] [IP:IP:...]

這個指令可以設定你的來源位址, 最多可設定十組常用的位址, 當來源位
址不符時, 系統將拒絕該連線要求, 以防止被來自這些位址外的不明人士
所侵入。設定位址時請小心設定, 若是設錯而造成不能連線進入時, 請備
妥下列資料mail給arch級以上巫師處理:
1. 該char平日的上線IP (記得的話, 最好把okip設成什麼也寫上去)
2. 該char的特徵 (包括屬性, 精氣神, 技能等)
3. 該char的密碼

範例
okip -a 111.222.333.444 就是增加了 111.222.333.444 為你來源路徑
okip -d 111.222.333.444 就是刪除了 111.222.333.444 為你來源路徑

位址之設定支援以*代表未知的碼, 但一組IP裏, *只能用一次, 如
okip -a 140.113*	表示IP開頭只要是140.113的位址均可連入
okip -a *.hinet.net 	表示IP只要最後兩碼的位址是hinet.net的均
			可連入
TEXT
        );
        return 1;
}
