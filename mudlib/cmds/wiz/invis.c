/*  invis.c

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

int main(object me, string arg)
{
    object link;

    SECURED_WIZARD_COMMAND;

    seteuid(getuid());
    if( (me!=this_player(1)) || !objectp(link = me->link()) ) return 0;

    if( !arg ) 
        link->set("invis", (int)link->query("invis") == 0 );
    else if( arg=="on" )
        link->set("invis", 1);
    else if( arg=="off" )
        link->set("invis", 0);
    else
        return notify_fail("指令格式﹕invis [on|off]\n");

    write("Ok。\n");
    return 1  ;
}

int help()
{
    write(@TEXT
指令格式﹕invis [on|off]

打開或關閉巫師的隱形功能。
TEXT
    );
    return 1;
}

