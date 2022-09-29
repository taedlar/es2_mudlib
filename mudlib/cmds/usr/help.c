/*  help.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include "/doc/help.h"

inherit F_CLEAN_UP;

static string *default_search = DEFAULT_SEARCH_PATHS;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string file, *search;

    if( !arg ) {
        cat(HELP_DIR + "help/topics");
        return 1;
    }

    arg = replace_string(arg, " ", "_");

    // Else, try if a command name is specified.
    seteuid(getuid());
    if( stringp(file = me->find_command(arg)) ) {
        notify_fail("有這個指令存在﹐但是並沒有詳細的說明文件。\n");
        return file->help(me);
    }

    // Support efun/lfun help with same name as other topics such as
    // ed() and ed command.
    sscanf(arg, "%s()", arg);

    search = DEFAULT_SEARCH_PATHS;
    if( wizardp(me) )
        search += WIZARD_SEARCH_PATHS;
        
    foreach(string path in search)
    {
	if( file_size(path + arg) < 0 ) continue;
	if( wizardp(me) )
	    write("說明文件：" + path + arg
		+ "\n---------------------------------------------------------------------\n");
	me->start_more( read_file(path + arg) ); 
	return 1;
    }

    return notify_fail("沒有針對這項主題的說明文件。\n");
}

int help(object me)
{
    write(@HELP
指令格式﹕help <主題>              例如﹕> help all
          help <函數名稱>()        例如﹕> help call_out()

這個指令提供你針對某一主題的詳細說明文件﹐若是不指定主題﹐則提供你有關主題
的文件﹐所有的主題與函數名稱皆為英文。
HELP
    );
    if( wizardp(me) )
        write(@HELP
本指令會先搜尋主題是否為 command, ( 若呼叫者為巫師, 則接著搜尋 /doc/help.h
中的 WIZARD_SEARCH_PATHS ), 再搜尋 DEFAULT_SEARCH_PATHS。 
若有同檔名之文件處於這些搜尋位置中, 則位於後搜路徑者將不被搜尋, 即只看到先
前搜尋之 doc, 編製新文件時請留意。
HELP
        );
    return 1;
}
