/*  info.c

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

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    object obj;
    string str;
    mixed file_stat;

    if (!arg) return help(me);

    if( arg=="me" ) obj = me;
    else if( arg=="my_link" ) obj = me->link();
    else {
        obj = present(arg, me);
        if (!obj) obj = present(arg, environment(me));
        if (!obj) obj = find_object( resolve_path(me->query("cwd"), arg) );
        if (!obj) return notify_fail("沒有這樣物件....。\n");
    }

    printf("物件 [%O]\n-----------------------------------------------------\n", obj);
    file_stat = stat(base_name(obj) + ".c");
    write("檔案﹕\t\t" + base_name(obj) + ".c\n");
    if( file_stat[2] )
        write("載入時間﹕\t" + CHINESE_D->chinese_period(time()-file_stat[2]) + "\n");
    write("權限﹕\t\tuid = " + getuid(obj) + ", euid = " + geteuid(obj) + "\n");
    write("等級﹕\t\t" + wizhood(obj) + "\n");
    write("使用記憶體﹕\t" + memory_info(obj) + "\n");
    str = "";
    if( living(obj) )		str += "生物 ";
    if( userp(obj) )		str += "使用者 ";
    if( interactive(obj) )	str += "交談式 ";
    if( wizardp(obj) )		str += "巫師 ";
    if( clonep(obj) )		str += "複製 ";
    if( virtualp(obj) )		str += "虛擬 ";
    if( query_heart_beat(obj) )	str += "心跳:" + query_heart_beat(obj) + " ";
    write("屬性﹕\t\t" + str + "\n");
    write("複製個數﹕\t" + sizeof(children(base_name(obj)+".c")) + "\n");
    write("參考連結﹕\t" + refs(obj) + "\n");
    return 1;
}

int help(object me)
{
  write(@HELP
指令格式 : info <物件之名稱或檔名>

利用此一指令可得知一些有關該物件的資訊。
HELP
    );
    return 1;
}
