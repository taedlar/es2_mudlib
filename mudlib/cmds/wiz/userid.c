// tail.c

inherit F_CLEAN_UP;

int help(object me);

int main(object me, string arg)
{
        object *ob;
        int i;
        ob=users();
        i = sizeof(ob);
        while (i--) {
        write(sprintf("%15s : %s\n",ob[i]->query("id"),
        ob[i]->query("userid")));
        }
        return 1;
}           

int help(object me)
{
write(@HELP
指令格式 : usrid <玩家 id>

此指令可讓你(妳)查看某玩家或所有玩家的 userid.
HELP
    );
    return 1;
}
