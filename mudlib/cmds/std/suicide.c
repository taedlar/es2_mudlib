/*  suicide.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <ansi.h>
#include <command.h>

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    object link_ob;
    string passwd;

    SECURED_COMMAND;

    if( !(link_ob = me->link()) )
        return notify_fail("你的連線物件不見了﹐現在無法自殺。\n");

    if( !arg ) return notify_fail("指令格式﹕suicide <密碼>\n");

    passwd = link_ob->query("password");
    if( crypt(arg, passwd) != passwd )
        return notify_fail("密碼錯誤﹗\n");

    log_file("static/SUICIDE",
        sprintf("[%s] %s committed suicide from %s\n",
	    ctime(time()), geteuid(me), query_ip_name(me)) );
    CHANNEL_D->do_chnnel(me, "sys", me->name() + "自殺了。\n");

    rm( link_ob->query_save_file() );
    rm( me->query_save_file() );
    
    write("好吧，希望還能很快再 ES2 中看到您。\n");
    tell_room(environment(me), me->name() +
        "自殺了﹐以後你再也看不到這個人了。\n", ({me}));

    destruct(link_ob);
    destruct(me);

    return 1;
}

int help (object me)
{
    write(@HELP
指令格式﹕suicide <密碼>
 
如果因為某種原因你不想活了﹐你可以用這個自殺指令來刪除你的人物﹐自殺
以後會將你的人物存檔完全自硬碟上消除﹐無法救回了﹐所以自殺前請務必三
思。

這個指令並不是讓玩家拿來消極地威脅巫師或 mud管理者的﹐自殺一個人物對
玩家跟巫師而言﹐都不是樂意見到的﹐因為這和現實生活中的「死亡」在某種
層面上是具有相同意義的﹐因此選擇自殺的途徑之前請先確定你已經想清楚並
確實明白這個指令的功用。
HELP
    );
    return 1;
}
