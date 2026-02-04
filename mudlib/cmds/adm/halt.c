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

    message("system", "遊戲關閉。\n", users() );

    user = users();
    for(i=0; i<sizeof(user); i++) {
        if( !environment(user[i]) ) continue;
        user[i]->save();
    }

    shutdown(-1);
    return 1;
}
int help (object me)
{
        write(@HELP
指令格式: halt
 
馬上關閉遊戲。
 
HELP
);
        return 1;
}
 

