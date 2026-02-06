

#include <login.h>
#include <obj.h>
#include <command.h>

inherit F_CLEAN_UP;

int
main(object me, string arg)
{
    string wiz_status;
    object npc;

    SECURED_ARCH_COMMAND;

    seteuid(getuid());

    npc = new(NPC_DEMOGORGON);
    npc->move(START_ROOM);
    npc->start_shutdown();

    write("Ok。\n");

    return 1;
}

int help (object me)
{
        write(@HELP
指令格式: reboot
 
重新起動遊戲, 系統會開始倒數計時, 十五分鐘後重新起動。
 
HELP
);
        return 1;
}
 

