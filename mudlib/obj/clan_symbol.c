/* clan.c - established by tears, modified by dragoon

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
#include <armor.h>
inherit F_FINGER_EQ;

mapping levels = ([]);

varargs int clan_line(string msg,int emote);

void create()
{
    object me = this_player();
    set_name(HIY + me->query("clan/clan_name") + NOR + "信物",
	({"clan symbol", "symbol"}));
    set_weight(1);
    if( !clonep() ) {
	set("unit", "個");
	set("long", 
	    "這是一個幫派專用的信物: 鐵指環。使用方式請參照: info clan\n");
	set("wear_as", "finger_eq");
	set("apply_armor/finger_eq", ([
   	    "armor" : 2,
	]));
	set("no_steal", 1);
	set("no_sell", 1);
	set("no_drop",1);
	set("value", 1);
//	set("no_get", 1);
    }
    set("keep_this", 1);
    setup();
}

int query_autoload() { return 1; }

void owner_is_killed(object killer) { destruct(this_object()); }

void init()
{
    add_action("help", "help");
    add_action("info", "info");
    add_action("do_members","clans");
    add_action("do_chat", "clan");
    add_action("do_chat_emote", "clan*");
    add_action("do_clan_set", "clan_set");
    add_action("do_clan_reset", "clan_reset");
}

int do_members()
{
    object *usr, temp, me;
    string nick, title;
    int i, j;
    me = this_player();
    usr = users();
	
    if( !me->query("clan/clan_name") ) {
	log_file("CLAN", sprintf("[%s] destruct symbol of %s(%s)\n",
            ctime(time()), me->name(), me->query("id")));
 	destruct(this_object());
    }

    if( wiz_level(usr[i]) > wiz_level(me) && !usr[i]->visible(me) )
	usr -= ({ usr[i] });
    usr = filter_array( users(), "filter_clan", this_object() );
    usr = sort_array( usr, "sort_usrs", this_object() );
    if ( sizeof(usr) ) {
	i = sizeof(usr);
	write("目前共有 " + i + " 位幫派成員連線中:\n"
"========================================================================\n");
	while(i--) {
 	    if( strlen(usr[i]->query("clan/nick")) > 20
	    || usr[i]->query("clan/nick") == "無" ) {
		nick = "  無"; }
	    else nick = "「"+usr[i]->query("clan/nick")+"」";
		
	    if( usr[i]->query("title") ) {
		title = usr[i]->query("title"); }
            else title = "平民";

	    if( usr[i]->query("clan/clan_level") == 3 ) {
		printf(HIY
		"【%s】"NOR"[%-2d %|12s] %-25s %s(%s)\n"NOR, 
		usr[i]->query("clan/rank"),
		usr[i]->query("level"), title, nick, usr[i]->name(),
		capitalize(usr[i]->query("id")) ); }
	    else if( usr[i]->query("clan/clan_level") == 2  ) {
		printf(HIG
		"【%s】"NOR"[%-2d %|12s] %-25s %s(%s)\n"NOR,
		usr[i]->query("clan/rank"),
		usr[i]->query("level"),	title, nick, usr[i]->name(),
		capitalize(usr[i]->query("id")) ); }
	    else {
		printf(
		"【%s】[%-2d %|12s] %-25s %s(%s)\n"NOR,
		usr[i]->query("clan/rank"),
		usr[i]->query("level"),	title, nick, usr[i]->name(),
		capitalize(usr[i]->query("id")) ); } }
	} else 
	    write("似乎沒有任何幫派成員在線上....。\n");

    return 1;
}

int sort_usrs(object u1, object u2)
{
    return (int)levels[u1] - (int)levels[u2] ;
}

int filter_clan(object ob)
{        
    object me = this_player();
    if( ob->query("clan/clan_name") == me->query("clan/clan_name") ) {
	return 1;
    }
    return 0;
}

int do_chat(string arg)
{
    if( !arg ) return notify_fail("你想對其它的幫派成員說什麼﹖\n");
    return clan_line(arg);
}

int do_chat_emote(string arg)
{
    object me = this_player();
    string verb, args, msg;

    if( !arg ) return notify_fail("你想對其它的幫派成員說什麼﹖\n");

    if ( sscanf(arg,"%s %s",verb,args) != 2 ) {
	verb = arg;
	args = "";
    }

    msg = EMOTE_D->do_emote(this_player(),verb,args,1);
    return clan_line(msg,1);
}

int info(string arg)
{
    object me = this_player();

    if( arg != "clan" ) return 0;

    if( !me->query("clan/clan_name") ) {
	log_file("CLAN", sprintf("[%s] destruct symbol of %s(%s)\n",
	    ctime(time()), me->name(), me->query("id")));
	destruct(this_object());
    }

    tell_object(me, "
    ======================================
    幫派提供下列用途:

    clans	可以看線上有那些幫派的成員
    clan	幫派專用頻道\n");
    return 1;
}

varargs int clan_line(string msg,int emote)
{
    object *usr, me;
    int i, tmp;

    me = this_player();

    if(!msg) return notify_fail("你要對本幫派的同伴說什麼﹖\n");

    if( me->query_temp("pending/hidden") ) {
	tmp = me->query_temp("pending/hidden");
	me->delete_temp("pending/hidden");
    }

    usr = filter_array(users(), "filter_clan", this_object());
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

int do_clan_set(string arg)
{
    object me, obj;

    me = this_player();

    if( !wizardp(me) ) return notify_fail("此為巫師專用指令。\n");

    seteuid( getuid() );
    clan_line("Leaving this channel.......\n");

    if( !arg )
	me->set("clan/clan_name", "蘋果派");
    else {
	me->set("clan/clan_name", arg);
	clan_line("Join this channel now...\n");
    }
    return 1;
}

int do_clan_reset(string arg)
{
    object me, obj;
    string target;
    int elders;

    me = this_player();

    if( wiz_level(me) < 3 ) 
	return notify_fail("本功\能只有大巫師級以上才能使用。\n");

    if( sscanf(arg, "%s %d", target, elders) != 2 )
	return notify_fail("指令格式: clan_reset <對象> <長老數>。\n");

    if( !obj = find_player(target) ) 
	return notify_fail("這個人不在線上。\n");

    seteuid( getuid() );
    obj->set("clan/elder", elders);
    tell_object(obj, "重新設定所屬幫派長老數為: " + elders + "。\n");

    log_file("CLAN",sprintf("[%s] %s: %s reset elder number to %d\n",
        ctime(time()), obj->query("clan/clan_name"), 
	me->query("id"), elders) );

#ifdef SAVE_USER
    obj->save();
#endif

    return 1;
}
