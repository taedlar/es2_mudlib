/*  object.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

varargs int
getoid(object ob)
{
    int id;

    if (!ob) ob = previous_object();
    sscanf(file_name(ob), "%*s#%d", id);
    return id;
}

// Get the owner of a file.  Used by log_error() in master.c.
string
file_owner(string file)
{
    string name, rest, dir;

    if (file[0] != '/') file = "/" + file;
    if (sscanf(file, "/u/%s/%s/%s", dir, name, rest) == 3) {
        return name;
    }
    return 0;
}

/*  creator_file()
 *
 *  這個函數是 ES2 安全系統的核心，每當一個物件被 load 的時候，系統會
 *  將這個物件的檔名傳給 simul_efun 中的 creator_file() 函數，並且用
 *  這個函數的傳回值作為此一物件的 uid。在 ES2 的安全系統中，物件的
 *  uid 表示這個物件的最高權限( 物件可以經由 seteuid 來提高或降低自己
 *  的權限 )，這個最高權限是單純由物件的檔名決定的，因此你也可以說ES2
 *  的安全系統是根基在檔案讀寫權的管制上。
 *
 *  ES2 目前對 uid 的發放方式請參考程式碼中的註解。
 */
string
creator_file(string file)
{
    string *path, euid;

    path = explode(file, "/") - ({ "" });
    if( ! sizeof(path) )
	return 0;	/* 這個情況應該不會發生，只是預防萬一 */

    switch(path[0])
    {
	// ----------------------------------------------------------
	// 在 /adm 和 /cmds 下的物件擁有最高的權限 ROOT_UID，可以做任
	// 何事。
	// ----------------------------------------------------------
	case "adm":
	case "cmds":	return ROOT_UID;

	// -----------------------------------------------------------
	// 開放區域的權限為 DOMAIN_UID，可改變使用者的屬性，但不能寫入
	// 檔案。
	// -----------------------------------------------------------
	case "d":	return DOMAIN_UID;

	// -----------------------------------------------------------
	// 在 /u 及 /open 下的物件如果是由使用者載入，則獲得使用者的相
	// 同權限，如果不是由使用者載入，則獲得和載入它的物件相同的權
	// 限。
	// -----------------------------------------------------------
	case "u":
	case "open":
	    if( this_player(1) ) euid = getuid(this_player(1));
	    if( !euid ) euid = geteuid(previous_object(1));
	    if( euid && euid != ROOT_UID ) return euid;
            return "NONAME";

	// ------------------------------------------------------------
	// 在 /obj 下的物件其最高權限由載入該物件的權限(euid)決定，換句
	// 話說由具有 ROOT_UID 權限的物件載入的 /obj 物件，就具有 ROOT
	// 權限，由使用者載入的物件就具有使用者的權限。一般具有 ROOT 權
	// 限的物件如果要載入 /obj 物件，通常會先 seteuid 成適當的權限
	// 再進行載入。
	// ------------------------------------------------------------
	case "obj":
	    if( previous_object(1) ) return geteuid(previous_object(1));
	    return "NONAME";

	// ------------------------------------------------------------
	// 其他目錄下的物件都給予 MUDLIB_UID 的權限
	// ------------------------------------------------------------
	case "daemon":
	default:
	    return MUDLIB_UID;
    }
}

void
destruct(object ob)
{
    string err;

    if( !ob ) return;

    err = catch(ob->remove());

    if( err )
	log_file("destruct", sprintf("[%s] %O: %s", ctime(time()), ob, err));

    if( userp(ob)
    &&	(geteuid(previous_object())!=ROOT_UID)
    &&	(geteuid(previous_object())!=geteuid(ob)) ) {
	log_file("destruct", sprintf("[%s] %O: Destructed by %s denied\n",
	    ctime(time()), ob, err));
	error("Destruction denied.\n");
    }

    efun::destruct(ob);
}
