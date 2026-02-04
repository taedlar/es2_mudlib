// vim: set ts=4 sw=4 syntax=lpc

#include <command.h>

inherit F_CLEAN_UP;

int
main(object me, string arg)
{
    string wiz_status;
    object *user, link_ob;
    int i;

    SECURED_ADMIN_COMMAND;

    seteuid(geteuid(this_player(1)));

    message("system", "遊戲重新啟動﹐請稍候一分鐘再 login 。\n", users() );

    user = users();
    for(i=0; i<sizeof(user); i++) {
        if( !environment(user[i]) ) continue;
        user[i]->save();
    }

    shutdown();
    return 1;
}
int help (object me)
{
        write(@HELP
指令格式: shutdown
 
馬上重新起動遊戲。
 
HELP
);
        return 1;
}
 

