// guard.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
        object ob;

	if( me->query("life_form") == "ghost" )
		return notify_fail("人死了就一了百了, 快去找城隍復活吧!!\n");

        if( !arg || arg=="" ) {
                if( ob = me->query_guarding() ) {
                        write("你現在保護的對象是" + ob->name() + "。\n");
                        return 1;
                }
                write("你現在並沒有選定保護的對象。\n");
                return 1;
        }

        if( arg=="cancel" ) {
                write("Ok.\n");
                me->guard_ob(0);
                return 1;
        }

        ob = present(arg, environment(me));
        if( !ob ) return notify_fail("這裡沒有這個人。\n");
        if( ob==me ) return notify_fail("你「理所當然」的會保護自己。\n");
        if( !userp(ob) )
                return notify_fail("他不需要你的保護。\n");
        if( me->query("level") < 5)
                return notify_fail("等你長大點再來保護別人\n");
        if( me->is_fighting(ob) )
                return notify_fail("你想保護正在跟你戰鬥的對手？\n");

        me->guard_ob(ob);
        write("Ok.\n");

        return 1;
}

int help(object me)
{
        write(@TEXT
指令格式﹕guard [<戰鬥目標>|cancel]

你可以用這個指令設定你要保護的對象，當你保護的對象遭受攻擊時，你的人物會在
第一時間加入戰鬥助戰。

要取消 guard 模式﹐可以用 guard cancel﹐不加任何參數則顯示你目前所保護的對
象‧
TEXT
        );
        return 1;
}
