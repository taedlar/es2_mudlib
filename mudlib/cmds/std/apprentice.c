// apprentice.c

#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object ob;

	if( !arg ) return notify_fail("你要拜誰為師﹖\n");

	ob = present(arg, environment(me));
	if( !ob || !ob->is_character() ) return notify_fail("這裡沒有這個人。\n");
	// add by dragoon
        if( objectp(ob) && ob->query("no_fight") ) 
		return notify_fail("什麼??\n");

	if( ob==me ) return notify_fail("拜你自己為師﹖\n");

	if( me->is_apprentice_of(ob) )
		return notify_fail("你已經是" + ob->name() + "的弟子了。\n");

	if( !living(ob)	)
		return notify_fail(ob->name() + "現在沒有辦法回答你。\n");

	notify_fail(ob->name() + "似乎沒有收你為徒的打算。\n");
	if( !ob->accept_apprentice(me) )
		return 0;

	message_vision(CYN "\n$N跪下來恭恭敬敬地向$n磕了四個響頭﹐叫道﹕師父﹗\n\n" NOR,
		me, ob);
	ob->init_apprentice(me);

	return 1;
}

int help(object me)
{
	write(@Help
指令格式: apprentice <某人>

拜某人當師父, 如果某人已是你(妳)師父, 則是向師父請安。
Help
	);
	return 1;
}
