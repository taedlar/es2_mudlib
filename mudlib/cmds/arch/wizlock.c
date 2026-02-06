

#include <command.h>

inherit F_CLEAN_UP;

int
main(object me, string arg)
{
    int lvl;

    SECURED_ARCH_COMMAND;

    if( !arg || arg=="" )
        return notify_fail("指令格式﹕wizlock <巫師等級>\n");

    if( sscanf(arg, "%d", lvl)!=1 ) lvl = wiz_level(arg);
    seteuid(getuid());
    if( LOGIN_D->set_wizlock(lvl) ) {
        write("Ok.\n");
        return 1;
    } else
        return notify_fail("上線權限設定失敗。\n");
}

int help (object me)
{
        write(@HELP
指令格式: wizlock <巫師等級>
 
限制某個等級以下的使用者進入遊戲。
其中巫師等級分別為 (immortal) (wizard) (arch) (admin), 若要取
消限制, 則輸入 (player).
 
HELP
);
        return 1;
}
 

