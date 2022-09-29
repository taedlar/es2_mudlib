/*  visitior.c

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
    string s;
    int cnt, t;

    s = read_file("/adm/etc/visitor.cnt");
    if( !s ) return 0;
    sscanf(s, "%d %d", t, cnt);
    printf("從 %s 以來共有 %d 使用者人次。\n", ctime(t), cnt);
    return 1;
}

int help()
{
    write(@TEXT
指令格式：visitor

這個指令會顯示系統目前統計的使用者人次資料。
TEXT
    );
    return 1;
}
