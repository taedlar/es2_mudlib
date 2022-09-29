/*  mudlist.c - list muds that are in contact with us over intermud

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <intermud.h>

inherit F_CLEAN_UP;

#define SYNTAX	"指令格式﹕mudlist [<Mud 名稱>]\n"

private void create() { seteuid(getuid()); }

int
main(object me, string arg)
{
    mapping mudlist;
    mixed *muds;
    string output;
    string mud;
    int list_all = 0;

    if( !find_object(INTERMUD_D) )
        return notify_fail("網路精靈並沒有被載入，請先將網路精靈載入。\n");

    mudlist = (mapping)INTERMUD_D->query_mudlist();

    if( !mapp(mudlist) )
        return notify_fail( MUD_NAME + "目前尚未和其他 Mud 取得聯繫。\n");

    muds = sort_array( keys(mudlist), 1);

    if( arg ) {
	string* args, mud_name;

	args = explode(arg, " ") - ({ "" });

	foreach(string name in args)
	{
	    string prop, str, msg;
	    mapping mudinfo;

	    if( mud_name && mapp(mudinfo)
	    &&	sscanf(name, "%s=%s", prop, str)==2 ) {
		write("設定 " + mud_name + " 的本地屬性：" + name + "\n");
		INTERMUD_D->set_mudinfo(mud_name, ([ prop : str ]) );
		continue;
	    }

	    if( sscanf(name, "%s->%s", prop, str)==2 ) {
		INTERMUD_D->set_mud_alias(prop, str);
		name = prop;
	    }

	    mud_name = name; // = htonn(name);
            if(!mapp( mudinfo = INTERMUD_D->query_mudinfo(name) )) {
                write(MUD_NAME + "並沒有和 \"" + name + "\" 取得聯繫。\n");
		continue;
            }

	    msg = "";
	    foreach(prop, str in mudinfo)
		msg += sprintf("  %15s: %O\n", prop, str);
	    write("有關 " + name + " 的資訊﹕\n" + msg);
        }
	if( mud_name ) return 1;
    }

    output = "";

    // Loop through mud list and store one by one
    foreach(mud in muds)
    {
//	if( !list_all && mudlist[mud]["MUDLIB"] != "Eastern Stories" )
//	    continue;
	output += sprintf("%-20s %-10s %s (%s)\n",
		mud,
		mudlist[mud]["ALIAS"] ? mudlist[mud]["ALIAS"] : "",
		mudlist[mud]["NAME"],
		mudlist[mud]["MUDLIB"]);
    }

    //    Display dumped mudlist output through user's more pager
    this_player()->start_more( output );

    return 1;
}

int help()
{
    write( @HELP
指令格式：mudlist [-a] [<MUD IP:PORT>|<代號>] {屬性=字串}
	  mudlist <代號>-><MUD IP:PORT>

這個指令讓你列出目前透過 intermud 協定取得聯繫的其他 Mud。

如果加上 -a 選項，所有的 Mud 都會被列出；否則只有使用 es2 mudlib 的 Mud
會被列出。

你可以指定單一的 MUD 名稱，mudlist 會列出該 MUD 的相關資訊。
mudlist 可以在本地端修改其他 MUD 的屬性，典型的用途是設定一個 MUD 的中文編
碼如：

> mudlist xajh ENCODING=GB

會將 xajh(笑傲江湖) 這個使用 GB 內碼的 MUD 的 ENCODING 設定成 GB，因為有不
少使用舊版 ES2 mudlib 的大陸 MUD 並沒有設定 ENCODING 的屬性，所以如果你看到
有些 MUD 送過來的 es 頻道訊息是亂碼，可以試著用上面的指令設定。

這些設定只有在本地端有效，並沒有辦法改變對方的設定，因此當 intermud 更新該
MUD 的屬性時，你可能必須重新設定。釜底抽薪的辦法是寫信給對方的管理者，請他
們正確設定送出來的 ENCODING 屬性。
HELP
    );
    return 1;
}

