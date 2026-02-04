// vim: set ts=4 sw=4 syntax=lpc

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string str)
{
    object ob, where, *ob_list;
    int old_size, i;

    if (!str) return notify_fail("指令格式﹕cleanup <檔名>\n"); 

    str = resolve_path(me->query("cwd"), str);
    ob_list = children(str);
    old_size = sizeof(ob_list);
    ob_list->clean_up();
    foreach(ob in ob_list)
        if( ob && clonep(ob) && !environment(ob) )
//        if( ob && ob->query_volume()==0 )
            destruct(ob);
    ob_list -= ({ 0 });
    printf("清除 %d 個物件。\n", old_size - sizeof(ob_list));
    return 1;
}

int help(object me)
{
    write(@HELP
指令格式﹕cleanup <檔名>

對所有指定檔名的物件呼叫 clean_up﹐並且清除所有被複製出來的﹐但是又沒有
被放在另一個物件中的散失物件。
HELP
    );
    return 1;
}


