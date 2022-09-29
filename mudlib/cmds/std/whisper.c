// whisper.c

#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	string dest, msg;
	object ob;

	if( me->is_fighting() )
		return notify_fail("你正在戰鬥中﹗\n");

	if( !arg || sscanf(arg, "%s %s", dest, msg)!=2 )
		return notify_fail("你要對誰耳語些什麼﹖\n");

	ob = present(dest, environment(me));
	if( !ob || !ob->is_character() )
		return notify_fail("你要對誰耳語﹖\n");

	if( ob->is_fighting() )
		return notify_fail( ob->name() + "正在戰鬥中﹗\n");

	write( GRN "你在" + ob->name() + "的耳邊悄聲說道﹕" + msg + "\n" NOR);
	tell_room( environment(me), me->name() + "在" + ob->name()
		+ "耳邊小聲地說了些話。\n", ({ me, ob }) );
	if( !userp(ob) ) ob->relay_whisper(me, msg);
	else
		tell_object( ob, GRN + me->name() + "在你的耳邊悄聲說道﹕" + msg + "\n" NOR);
	return 1;
}

int help(object me)
{
	write( @TEXT
指令格式﹕whisper <某人> <訊息>

這個指令可以用來和同一房間中的人耳語﹐包括 NPC 在內。
TEXT
	);
	return 1;
}
