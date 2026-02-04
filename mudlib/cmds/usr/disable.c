// vim: set ts=4 sw=4 syntax=lpc

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    object ob;
    mapping m;
    string sk, sk_m;

    if( !arg || !(ob = SKILL_D(arg)) || ob==SKILL_D("none") )
        return notify_fail("你要停用什麼技能？\n");

    if( me->is_busy() )
        return notify_fail("你正在忙著別的事, 無法專心。\n");

    if (!me->query_learn(arg)) 
        return notify_fail("你目前不會這項技能。\n");

    if( !function_exists("valid_enable", ob) ) {
        me->map_skill(arg, "none");
        write("你決定暫時停止使用所學有關「" + to_chinese(arg) + "」的技巧。\n");
    } else {
	int c = 0;
	m = me->query_skill_map();
	foreach(sk, sk_m in m) {
	    if( sk_m==arg ) {
		me->map_skill(sk);
		write("你決定停止在「" + to_chinese(sk) + "」方面停止使用所學有關「" + to_chinese(arg) + "」的技巧。\n");
		c++;
	    }
	}
	if( !c ) {
	    write("你決定停止使用所學有關「" + to_chinese(arg) + "」的技巧。\n");
	    me->map_skill(arg, "none");
	}
    }

    return 1;
}

int help()
{
    write(@TEXT
指令格式：disable <技能>

這個指令讓你停止使用某種技能，這裡所指的技能是比較廣義的，如劍法、刀法
等，如果你要停止使用某種劍法類武功，只要用 enable 設定劍法成其他武功即
可。

其他相關指令：enable、skills
TEXT
    );
    return 1;
}

