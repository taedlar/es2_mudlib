/*  sneak.c - the alternate move command as 'go' that moves secretly.

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/


// This command is another command for movement. Those NPCs who intercept
// players by add_action(..., "go") won't intercept this command.

#include <ansi.h>

inherit F_CLEAN_UP;

int do_sneak(object me, object dest, string dir, string exit_dir, int type);
int halt_sneak(object me, object who, string how);

mapping default_dirs = ([
    "north":		"北",
    "south":		"南",
    "east":		"東",
    "west":		"西",
    "northup":		"北邊",
    "southup":		"南邊",
    "eastup":		"東邊",
    "westup":		"西邊",
    "northdown":	"北邊",
    "southdown":	"南邊",
    "eastdown":		"東邊",
    "westdown":		"西邊",
    "northeast":	"東北",
    "northwest":	"西北",
    "southeast":	"東南",
    "southwest":	"西南",
    "up":		"上",
    "down":		"下",
    "out":		"外",
]);

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    mixed dest;
    string mout, min, dir;
    object env, obj, *observer, obsr;
    int skill, obsp, type;

    if( !arg ) return notify_fail("你要往哪個方向移動﹖\n");

    if( me->is_busy() )
        return notify_fail("要移動請你先用 halt 終止你正在做的事。\n");

    env = environment(me);
    if( !env ) return notify_fail("你哪裡也去不了，請找一位巫師處理。\n");

    if( me->is_fighting() )
	return notify_fail("你還在戰鬥！\n");

    if( !(dest = env->query("exits/" + arg)) )
	return notify_fail("這個方向沒有出路。\n");

    // Load destination room and check its existence.
    if( stringp(dest) ) obj = load_object(dest);
    else obj = dest;
    if( !objectp(obj) )
	return notify_fail("這一個方向的出口有問題﹐請通知巫師處理。\n");

    skill = me->query_skill("sneak");

    observer = all_inventory(obj);
    obsp = 0;
    // The sneak formula:
    //   postive factors: sneak skill, player's gin
    //   negative factors: gin of characters in both room, and their
    //                     sneak/force skill.
    foreach(obsr in observer) {
        if( !living(obsr) || obsr==me ) continue;
        obsp += random(obsr->query_stat("gin") / 3);
        obsp += obsr->query_skill("force");
    }

    skill = skill * 3 + me->query_stat("gin") / 10 + me->query_skill("move");

//    printf("潛行技巧 %d，觀察者 %d。\n", skill, obsp);

    // Test random match. If fail, move with go.
    if( random(skill*3 + me->query_stat("gin")) < random(obsp) )
        type = 0;
    else type = 1;
    
    if( !env->valid_leave(me, arg) ) return 0;

    if( !undefinedp(default_dirs[arg]) )    dir = default_dirs[arg];
    else                                    dir = arg;

    write("你開始躡\手躡\足地往" + dir + "移動。\n");

    me->set_temp("sneak_countdown", 3);
    me->start_busy( (: do_sneak, me, obj, dir, arg, type :), (: halt_sneak :) );

    return 1;
}

int do_sneak(object me, object obj, string dir, string exit_dir, int type)
{
    int rating;
    object old_env;

    if( !me || !obj ) return 0;
    if( me->query_temp("sneak_countdown") > 0 ) {
	me->add_temp("sneak_countdown", -1);
	tell_object(me, CYN "小心翼翼 ...\n" NOR);
	me->improve_skill("sneak", 1);
	return 1;
    }

    me->delete_temp("sneak_countdown");
    old_env = environment(me);
    me->interrupt(me, "ready");
    switch(type) {
    case 0:
        me->force_me("go " + exit_dir);
        break;
    case 1:
	/* 潛行的隱藏程度 */
	rating = 1 + random( me->query_skill("sneak") * 2 );
        tell_object(me, "看起來，似乎還沒有人發覺你的行動。\n");
	me->set_temp("pending/hidden", rating);
        me->improve_skill("sneak", me->query_attr("int") );
        if( !me->move(obj) ) me->set_temp("pending/hidden", 0);
        break;
    }

    return 0;
}

int halt_sneak(object me, object who, string how)
{
    if( who==me && how=="ready" ) return 1;
    tell_object(me, "你停止潛行。\n");
    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : sneak <方向>
 
讓你往指定的方向潛行﹐潛行需要一些技巧，當你使用潛行的方式移動，你自己
並不會知道別人有沒有發現你──除非他們因為發現你鬼鬼祟祟而發動攻擊。
 
HELP
    );
    return 1;
}

