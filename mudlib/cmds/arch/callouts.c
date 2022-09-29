/*  callouts.c

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
    mixed *info;
    int i;
    string output;

    output = sprintf("%-30s %-20s %5s \n", "物件", "函數", "延遲時間"); 
    if( arg && arg!="" )
	info = filter_array(call_out_info(), (: $1[1]==$2 :), arg);
    else
	info = call_out_info();

    for(i=0; i<sizeof(info); i++)
	output += sprintf("%-30O %-20s %5d \n",
	    info[i][0], info[i][1], info[i][2] );

    me->start_more(output);
    return 1;
}

int help()
{
        write(@LONG
指令格式﹕callouts [<函數名稱>]
LONG
        );
        return 1;
}
