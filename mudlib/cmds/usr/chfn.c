

#include <command.h>

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    object link;

    SECURED_COMMAND;

    if( link = me->link() ) {
	write("您的電子郵件信箱地址 [" + link->query("email") + "]: ");
	input_to("get_email", link);
	return 1;
    }
    return notify_fail("斷線使用者不能修改個人資料。\n");
}

void get_email(string new_email, object link)
{
    if( new_email != "" )
	link->set("email", new_email);

    write("您的自我介紹: (finger時顯示)\n");
    link->body()->edit((: call_other, __FILE__, "get_profile", link :));
}

void get_profile(object link, string str)
{
    link->set("profile", str);
    write("您的簽名格式: (post時用)\n");
    link->body()->edit((: call_other, __FILE__, "get_signature", link :));
}

void get_signature(object link, string str)
{
    if( strlen(str) > 400 )
	write("簽名格式太長﹗請重新設定。\n");
    else
	link->set("signature", str);
    write("請記得用 save 儲存您所作的修改。\n");
}

int help(object me)
{
    write(@HELP
指令格式 : chfn

用來修改你的個人資料，包括電子郵件，自我介紹，以及簽名。
HELP
    );
    return 1;
}

