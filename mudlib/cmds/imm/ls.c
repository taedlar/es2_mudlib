

#include <ansi.h>
#include <command.h>

inherit F_CLEAN_UP;

int
main(object me, string arg)
{
    int i, w, col;
    string dir, listing;
    mixed *file, *dirent;
    object ob;

    SECURED_COMMAND;
    seteuid(geteuid(this_player(1)));

    dir = resolve_path(me->query("cwd"), arg);
    if( file_size(dir)==-2 && dir[<1] != '/' ) dir += "/";

    file = get_dir(dir, -1);
    if( !sizeof(file) ) {
	if (file_size(dir) == -2) write("目錄是空的。\n");
	else write("沒有這個目錄。\n");
	return 1;
    }

    i = sizeof(file);
    w = 0;
    while(i--) {

	if( file[i][1]==-2 ) file[i][0] += "/";

	if( strlen(file[i][0]) > w ) w = strlen(file[i][0]) + 1;
    }

    listing = sprintf("目錄：%s\n", dir );
    col = 70 / (w+6);
    i = 0;
    foreach( dirent in file ) {
	ob = find_object( dir + dirent[0] );
	listing += sprintf("%4d ", 1 + dirent[1]/1024);
	if( ob ) listing += HIG;
	else if( dirent[1]==-2 ) listing += HIB;
	listing += sprintf("%-*s" NOR, 1+w, dirent[0]);
	listing += (++i%col) ? "  " : "\n";
    }

    if( listing[<1] != '\n' ) listing += "\n";
    me->start_more( listing );

    return 1;    
}

int
help(object me)
{
    write(@HELP
指令格式: ls [<路徑名>]
 
列出目錄下所有的子目錄及檔案, 如果沒有指定目錄, 則列出所在目錄
的內容﹐所列出的檔案中前面標示 * 號的是已經載入的物件。
 
範例:
'ls /' 會列出所有位於根目錄下的檔案及子目錄.
 
HELP
    );
    return 1;
}

