/*  alias.c

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

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    int i;
    mapping alias;
    string verb, replace, *vrbs;
    object link;

    SECURED_COMMAND;

    if( !(link = me->link()) )
	return notify_fail("你沒有連線物件。\n");

    if( !arg ) {
	alias = link->query_all_alias();
	if( !sizeof(alias) ) {
	    write("你目前並沒有設定任何 alias。\n");
	    return 1;
        } else  {
	    write("你目前設定的 alias 有﹕\n");
	    vrbs = keys(alias);
	    for(i=0; i<sizeof(vrbs); i++)
		printf("%-15s = %s\n", vrbs[i], alias[vrbs[i]]);
	    return 1;
	}
    }

    if( sscanf(arg, "%s %s", verb, replace)!=2 )
	link->set_alias(arg, 0);
    else if( verb=="alias" )
	return notify_fail("你不能將 \"alias\" 指令設定其他用途。\n");
    else if( verb=="" )
	return notify_fail("你要設什麼 alias﹖\n");
    else
	link->set_alias(verb, replace);

    write("Ok.\n");
    return 1;
}

int help (object me)
{
    write(@HELP
指令格式 : alias <欲設定之指令> <系統提供之指令>
 
有時系統所提供之指令需要輸入很長的字串, 在使用時(尤其是經常用到的)
或許會感覺不方便, 此時你(妳)即可用此一指令設定並替代原有之指令。
 
範例:
    'alias sc score' 會以 sc 取代 score 指令。
    'alias' 後不加參數則列出你所有的替代指令。
    'alias sc' 會消除 sc 這個替代指令。 (如果你有設的話)
 
其中可以用 $1, $2, $3 .... 來取代第一、第二、第三個參數﹐或是 $* 取代
所有的參數﹐如﹕
    'alias pb put $1 in $2'

則每當你(妳)打﹕
    pb bandage bag

就會取代成﹕
    put bandage in bag
    
HELP
    );
    return 1;
}
