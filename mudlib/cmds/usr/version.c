// vim: set ts=4 sw=4 syntax=lpc

#include <mudlib.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
  string name;
  printf("遊戲伺服器：%s %s\n", capitalize(__DRIVER__), __VERSION__ );
  printf("Mudlib 版本：%s\n", MUDLIB_VERSION );
  return 1;
}

int help(object me)
{
  write(@HELP
指令格式 : version
 
這個指令會顯示遊戲目前所用的 LP driver 版本以及 mudlib 版本。
HELP
  );
  return 1;
}

