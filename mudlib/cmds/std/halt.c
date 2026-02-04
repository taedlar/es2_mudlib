// vim: set ts=4 sw=4 syntax=lpc

#include "/doc/help.h"
#include <action.h>
#include <command.h>

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    SECURED_COMMAND;

    if( me->is_busy() ) {
        me->interrupt(me, INTR_HALT);
        if( ! me->is_busy() ) {
            write("Ok。\n");
            return 1;
        }
        return 0;
    }

    if( !me->is_fighting() )
        return notify_fail("你現在並沒有在戰鬥。\n");

    if( wizardp(me) ) {
        me->remove_all_killer();
        message_vision("$N用巫師的神力停止了這場打鬥。\n", me);
    } else {
        me->remove_all_enemy();
        message_vision("$N向後一躍，離開戰圈罷手不鬥了。\n", me);
    }

    write("Ok.\n");
    return 1;
}

int help(object me)
{
   write(@HELP
指令格式：halt

可以停止所有與你(妳)正在進行的比試﹐但是正在跟你性命相搏的敵人通常
不會輕易言和。

這個指令有時候也可以用來停止一些需要較長時間的動作，如修練內功、打
坐等，不過能不能停止要視那個動作而定。
HELP
   );
   return 1;
}

