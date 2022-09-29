/*  option.h

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <command.h>

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string term;
    mixed v;

    if( !arg )
    {
        mapping opt = me->query("option");
        string str = "你設定的選項；\n";

        if( !mapp(opt) || !sizeof(opt) )
            str += "    無。\n";
        else
            foreach(term, v in opt)
                str += sprintf("    %-20s：%O\n", term, v);
        write(str);
        return 1;
    }

    if( sscanf(arg, "%s %d", term, v)==2
    ||	sscanf(arg, "%s %s", term, v)==2 ) {
	if( !v ) me->delete("option/" + term);
	else me->set("option/" + term, v);
    } else
	return notify_fail("指令格式：option <選項> <設定值>\n");

    write("Ok.\n");
    return 1;
}

int help()
{
    write(@TEXT
指令格式：option <選項> <設定值>

這個指令讓你設定一些有關使用者介面的選項。

BRIEF_ROOM
  0 - 移動時觀察房間的詳細敘述。(預設值)
  1 - 移動時不顯示房間的詳細敘述。

BRIEF_SHORT
  0 - 顯示物品及人物的中文與英文名稱。(預設值)
  1 - 只顯示物品及人物的中文名稱。
TEXT
    );
    return 1;
}
