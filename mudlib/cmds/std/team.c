/*  team.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <ansi.h>

inherit F_CLEAN_UP;

int
main(object me, string arg)
{
    object *t;

    if( !arg ) {
	if( !pointerp(t = me->query_team()) )
	    return notify_fail("你現在並沒有參加任何隊伍。\n");
	write("你現在隊伍中的成員有﹕\n  " + implode(t->short(1), "\n  ") + "\n");
	return 1;
    }

    if( arg=="dismiss" ) {
        if( !pointerp(t = me->query_team()) )
            return notify_fail("你現在並沒有參加任何隊伍。\n");
        if( me->is_team_leader() ) {
            message("team", me->name(1) + "將隊伍解散了。\n", t, me);
            write("你將隊伍解散了。\n");
        } else {
            message("team", me->name(1) + "決定脫離隊伍。\n", t, me);
            write("你脫離了你的隊伍。\n");
        }
        me->dismiss_team();
        return 1;
    }

    if( sscanf(arg, "with %s", arg)==1 ) {
        object ob;

        if( !(ob = present(arg, environment(me)))
        ||    !living(ob)
        ||    !ob->is_character()
        ||    ob==me )
            return notify_fail("你想和誰成為夥伴﹖\n");

        if( !pointerp(me->query_team()) || me->is_team_leader() ) {
            if( me==(object)ob->query_temp("pending/team") ) {
                if( !pointerp(me->query_team()) ) {
                    ob->add_team_member(me);
                    message_vision("$N決定加入$n的隊伍。\n", me, ob);
                } else {
                    me->add_team_member(ob);
                    message_vision("$N決定讓$n加入隊伍。\n", me, ob);
                }
                ob->dellete_temp("pending/team");
                return 1;
            } else {
                message_vision("$N邀請$n加入$P的隊伍。\n", me, ob);
                tell_object(ob, YEL "如果你願意加入﹐請用 team with " + me->query("id") + "。\n" NOR);
                me->set_temp("pending/team", ob);
                return 1;
            }
        } else
            return notify_fail("只有隊伍領袖可以邀請別人加入。\n");
    }

    if( sscanf(arg, "talk %s", arg)==1 ) {
        if( !pointerp(t=me->query_team()) )
            return notify_fail("你現在並沒有和別人組成隊伍。\n");
        message("team", HIW "【隊伍】" + me->name(1) + "﹕" + arg + "\n" NOR, t);
        return 1;
    }

    if( sscanf(arg, "form %s", arg)==1 ) {
        if( !pointerp(t=me->query_team()) || !me->is_team_leader() )
            return notify_fail("你必須是一個隊伍的領袖才能組織陣形。\n");
        if( !me->query_skill(arg, 1) )
            return notify_fail("這種陣形你沒學過。\n");
        return SKILL_D(arg)->form_array(me);
    }
}

int help(object me)
{
    write(@HELP
隊伍指令使用方法:

team with <某人> - 跟某人組成隊伍. 必須要雙方都同意加入才會生效。
team dismiss     - 離開隊伍. 若下此指令的是領隊, 則整個隊伍會解散。
team talk <訊息> - 跟隊伍裡其他的人談話, 可以用 tt <訊息> 替代。
team form <陣法> - 如果隊伍的領隊學過陣法的話﹐可以將隊伍中的成員組織成
                   某種陣形﹐組成陣形的狀態只要領袖一移動就會解除。

直接下 team 指令時則會顯示你目前是否有加入隊伍及隊員名單, 第一個是領袖。
 quit 時會自動脫離隊伍, 領袖 quit 時則整個隊伍解散。

註: team 跟 follow 是獨立的﹐你不一定要 follow 隊伍的領袖。
HELP
    );
    return 1;
}
