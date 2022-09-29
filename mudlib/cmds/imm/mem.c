/*  mem.c

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

string memory_expression(int m);

int main(object me, string arg)
{
    object obj;

    if (!arg) {
        printf( "%s目前共使用 %s bytes 記憶體。\n",
            MUD_NAME, memory_expression(memory_info()) );
        return 1;
    }

    if( arg=="-m" ) {
        write(malloc_status());
        return 1;
    }

    seteuid(geteuid(me));
    obj = find_object(arg);
    if (!obj) obj = present(arg, me);
    if (!obj) obj = present(arg, environment(me));
    if (!obj) obj = find_object( resolve_path(me->query("cwd"), arg) );
    if (!obj) return notify_fail("沒有這樣物件....。\n");

    printf( "%O 共使用 %s bytes 記憶體。\n", obj, memory_expression(memory_info(obj)));
    return 1;
}

string memory_expression(int m)
{
    float mem;

    mem = m;
    if( mem<1024 ) return m + "";
    if( mem<1024*1024 )
        return sprintf("%.2f K", (float)mem / 1024);
    return sprintf("%.3f M", (float)mem / (1024*1024));
}

int help(object me)
{
  write(@HELP
指令格式 : mem <物件之名稱或檔名>

這個指令告訴你某個物件佔用的記憶體數量。
若沒有指明物件, 則會顯示目前遊戲所佔用的記憶體.
HELP
    );
    return 1;
}
