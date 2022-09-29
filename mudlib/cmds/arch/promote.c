/*  promote.c

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

static void create() { seteuid(getuid()); }

int
main(object me, string arg)
{
    object ob;
    string old_status, new_status;

    SECURED_ARCH_COMMAND;

    if( !arg || sscanf(arg, "%s %s", arg, new_status)!=2 )
        return notify_fail("指令格式﹕promote <使用者> <等級>\n");

    if( wiz_level(new_status) < 0 ) return notify_fail("沒有這種等級。\n");
    if( wiz_level(me) < wiz_level(new_status) )
        return notify_fail("你沒有這種權力。\n");

    if( !objectp(ob = find_player(arg))) {
        if( !(SECURITY_D->set_status(arg, new_status)) )
            return notify_fail("修改失敗。\n");
        write("Ok.\n");
        return 1;
    }

    old_status = wizhood(ob);

    if( !(SECURITY_D->set_status(ob, new_status)) )
        return notify_fail("修改失敗。\n");

    message_vision("$N將$n的權限從 " + old_status + " 改為 " + new_status + " 。\n", me, ob);
    seteuid(getuid());
    ob->setup();

    return 1;
}

int help(object me)
{
write(@HELP
指令格式 : promote <某人> (權限等級)

用來提升權限等級, (player) (immortal) (apprentice) (wizard) (arch) (admin)
一個 admin 能提升權限至任何等級, 而 arch 只能提升至 arch。
HELP
    );
    return 1;
}
