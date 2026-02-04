// vim: set ts=4 sw=4 syntax=lpc

#include <command.h>
#include <login.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    int last_backup, delta_t;
    string save_file;

    SECURED_COMMAND;

    seteuid(getuid());

    if( (last_backup = me->query_temp("last_backup"))
    &&  (delta_t = (time()/60 - last_backup)) < 5 ) {
	write( sprintf("不用急，%d 分鐘前你才剛備份過，%d 分鐘後再試吧。\n",
		delta_t, (5-delta_t)) );
	return 1;
    }

#ifdef SAVE_USER
    save_file = me->query_save_file();
    if( cp(save_file, save_file+".backup") ) {
	write("備份完畢。\n");
	me->set_temp("last_backup", (int)(time()/60) );
    }
    else
	write("備份失敗！\n");
    return 1;
#else
    return notify_fail("系統目前不儲存使用者資料。\n");
#endif
}

int help(object me)
{
    write(@HELP
指令格式﹕backup

把你辛苦奮鬥的結果備份起來。為了提升系統效率，目前兩次 backup 之時間間距最
短為五分鐘。

這個指令的目的是為了避免使用者遭受到 <非設計者原意且無法避免的因素> ，而造
成無法彌補之重大損失。

當大巫師或神認為有必要時，會將你的備份檔取回。
HELP
    );
    return 1;
}

