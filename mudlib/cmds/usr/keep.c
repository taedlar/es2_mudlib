

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    object ob, *all_ob;
    int i;

    if( !arg ) return notify_fail("指令格式：keep <身上物品>\n");
    if( arg=="all" ) {
        all_ob = all_inventory(me);
        i = sizeof(all_ob);
        while( i-- )
            all_ob[i]->set("keep_this", 1);
        write("你將所有身上的東西記了個小記號◎。\n");
        return 1;
    }
    if( arg=="none" ) {
        all_ob = all_inventory(me);
        i = sizeof(all_ob);
        while( i-- )
            all_ob[i]->set("keep_this", 0);
        write("你將所有身上的東西之小記號◎清掉。\n");
        return 1;
    }
    ob = present(arg, me);
    if( !ob ) return notify_fail("你身上沒有這樣東西。\n");
    if( userp(ob) ) return notify_fail("不行。\n");
    if( ob->query("keep_this") ) {
        ob->delete("keep_this");
        write("你把 " + ob->name() + " 上面的小記號◎清掉了。\n");
    }
    else {
        ob->set("keep_this", 1);
        write("你在 " + ob->name() + " 上面打了個小記號◎。\n");
    }
    return 1;
}

int help(object me)
{
    write(@HELP
指令格式：keep <身上物品>|<all>|<none>

例如:
keep none    將所有物品解除 keep
keep sword    針對 sword 設定 keep 或解除 keep

設定身上重要物品不會被不小心丟棄(如 drop all 或 sell all)，可以 inventory
指令查看是否已設定 keep ◎。

設定不隨意丟棄的東西只有當你指定丟棄/賣出這個東西時才會被丟棄或賣出，並且其
設定會被解除。若你將設定不隨意丟棄的物品丟到地上，再撿起來必須重新設定一次。
HELP
    );
    return 1;
}

