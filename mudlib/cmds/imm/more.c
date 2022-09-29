/*  more.c - the more command

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
