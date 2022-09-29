/*  order.c

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
    int res;
    object ob;
    string who, cmd, verb, path;

    // No indirect mode for this command.
    if( me != this_player(1) ) return 0;

//    if( !wizardp(me) ) return 0;

    if( !arg || arg=="" || sscanf(arg, "%s to %s", who, cmd)!=2 )
        return notify_fail("你要命令誰做什麼事﹖\n");

    ob = present(who, environment(me));
    if( !ob || !ob->is_character())
        return notify_fail("這裡沒有這個人。\n");

    if( sscanf(cmd, "%s %*s", verb) != 2 ) verb = cmd;

    if( stringp(path = ob->find_command(verb))
    &&    path != "/cmds/std/" + verb )
        return notify_fail("你不能命令別人做這種事。\n");

    if( wizardp(ob) && wiz_level(me) <= wiz_level(ob) )
        return notify_fail("你不能指揮這個人。\n");

    write("你命令" + ob->name() + ": " + cmd + "\n");
    if( (object)ob->query_temp("possessed") != me
    &&    !ob->accept_order(me, verb) )
        return notify_fail( "看起來" + ob->name() + "並不想聽你指揮。\n");

    notify_fail( ob->name() + "無法完成這個命令。\n");
    seteuid(getuid());
//    snoop(me, ob);
    res = ob->force_me(cmd);
//    snoop(me);
    return res;
}

int help(object me)
{
    write(@TEXT
指令格式﹕order <某人> to <指令>

命令他人執行某個指令﹕至於如何使他人聽你的命令 ... 自己找 :P
TEXT
    );
    return 1;
}
