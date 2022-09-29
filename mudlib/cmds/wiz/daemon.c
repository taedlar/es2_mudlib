/*  daemon.c

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
    mapping dmap;
    string term, file;
    string list = "";
    string wiz_status;

    SECURED_WIZARD_COMMAND;

    dmap = DAEMON_D->query_daemons();
    if( !arg || sscanf(arg, "-l %s", arg) ) {
        string *t;
        int i;
        t = sort_array(keys(dmap), 1);
        for(i=0; i<sizeof(t); i++) {
            if( arg && strsrch(t[i], arg)!=0 ) continue;
            list += sprintf("%-30s %s\n", t[i], dmap[t[i]]);
        }
        me->start_more(list);
        return 1;
        } else if( sscanf(arg, "-d %s", term) ) {
        wiz_status = SECURITY_D->get_status(me);
            if( wiz_status != "(admin)" && wiz_status != "(arch)" ) {
                return notify_fail("只有 (arch) 以上的巫師才能刪除 daemon 的登記。\n");
            } else {
                map_delete(dmap, term);
                write("Ok.\n");
                return 1;
            }
        }
 
        return 0;
}
 
int help()
{
    write(@TEXT
指令格式﹕daemon                       列出所有登記的 daemon。
          daemon -l <domain>           列出所有登記的特定種類 daemon。
          daemon -d <domain>:<name>    刪除某一 daemon 的登記。
	                                (arch 以上)
TEXT
    );
    return 1;
}
