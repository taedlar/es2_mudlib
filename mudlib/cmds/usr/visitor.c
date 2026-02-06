

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    string s;
    int cnt, t;

    seteuid(getuid());
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

