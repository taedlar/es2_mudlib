/*  id.h

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

int main(object me, string arg)
{
    int i, j;
    object obj, *inv;
    string *id;

    if( !arg ) {
	inv = all_inventory(me);
	if( !sizeof(inv) )
	    return notify_fail("你身上沒有任何東西。\n");
	write( "你(妳)身上攜帶物品的別稱如下(右方) :\n");
	for(i=0; i<sizeof(inv); i++) {
	    if( !inv[i]->visible(me) ) continue;
	    printf("%-20s = %s\n", inv[i]->name(),
		implode(inv[i]->parse_command_id_list(), ", ") );
	}
	return 1;
    }

    if( arg=="here" ) {
	inv = all_inventory(environment(me));
	if( !sizeof(inv) )
	    return notify_fail("這裡沒有任何東西。\n");
	write( "在這個房間中, 生物及物品的(英文)名稱如下 :\n");
	for(i=0; i<sizeof(inv); i++) {
	    if( !inv[i]->visible(me) ) continue;
	    printf("%-20s = %s\n", inv[i]->name(),
		implode(inv[i]->parse_command_id_list(), ", ") );
	}
	return 1;
    }
}

int help(object me)
{
    write(@HELP
指令格式 : id [here]
 
這個指令可以讓你知道物品的英文名稱及名字. 只打 id 會顯示
你身上所攜帶物品的名稱. 'id here' 則顯示所有跟你在同一個
環境裡的物件名稱.
HELP
    );
    return 1;
}
 
