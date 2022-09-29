// quiet command..

inherit F_CLEAN_UP;

int main(object me, string str)
{
        object ob;
        int i;

        seteuid(getuid(me));
        if(!str) return notify_fail("指令格式: quiet <使用者>\n");
        ob = LOGIN_D->find_body(str);
        if(!ob) return notify_fail("線上的使用者中沒有這個人。\n");

	seteuid(geteuid(ob));
	if (ob->query("no_chat") == 1) {
		ob->set("no_chat", 0) ; 
		tell_object(ob, "巫師("+me->query("id")+")取消你的禁言\n");
		tell_object(me,"你取消了玩家("+ob->query("id")+")的禁言\n");
		}
	else {
		ob->set("no_chat", 1) ;
		tell_object(ob, "巫師("+me->query("id")+")禁止你使用公共頻道\n");
                tell_object(me,"你啟動玩家("+ob->query("id")+")的禁言\n");
		}
        return 1;
}

int help(object me)
{
write(@HELP
指令格式 : quiet <某人>

被禁言者再次使用quite 就可以取消禁言
此指令可讓你(妳)將某人禁止使用公用頻道。
HELP
    );
    return 1;
}
