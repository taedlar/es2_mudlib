

inherit F_CLEAN_UP;

int help(object me);

int main(object me, string arg)
{
    string file, text;
    object ob;

    seteuid(geteuid(this_player(1)));
    if (!arg)
        return notify_fail("指令格式 : more <檔名>|<物件名> \n");
    file = resolve_path(me->query("cwd"), arg);
    if( file_size(file) < 0 ) {
        ob = present(arg, me);
        if( !ob ) ob = present(arg, environment(me));
        if( !ob ) return notify_fail("沒有 "+file+" 這個檔案。\n");
        file = base_name(ob) + ".c";
    }
    if( !text = read_file(file) ) text = "";
    write("檔案：" + file + "\n");
    me->start_more(text);
    return 1;
}

int help(object me)
{
write(@HELP
指令格式 : more <檔案>|<物件名>
 
分頁顯示檔案內容。
 
more 模式下的指令:
q      : 離開 more.
b      : 顯示前一頁的內容.
[ENTER]: 顯示下一頁的內容.

HELP
    );
    return 1;
}

