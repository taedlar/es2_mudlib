/*  logind.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#pragma save_binary

#include <ansi.h>
#include <command.h>
#include <login.h>

inherit F_CLEAN_UP;
inherit F_DBASE;

int wiz_lock_level = WIZ_LOCK_LEVEL;

string *user_race = ({
    "blackteeth",
    "human",
    "yenhold",
    "woochan",
    "jiaojao",
    "rainnar", 
    "ashura",
    "headless",
    "yaksa",
    "malik"
});

string *banned_name = ({
    "你", "妳", "我", "他", "她", "牠", "它",
});

string *banned_rude_name = ({
    "幹", "操", "娘", "媽", "屌", "屄", "插", "姦", "穴",
});

string *banned_ip = ({
// 暫時取消, 記錄先留著, 若是情況仍相同再考慮永久ban掉 -dragoon
//	"163.19.7.%*s", 	// mutli, dragoon
//	"140.96.148.%*s",	// mutli, dragoon
});

string *banned_hostname = ({
	"%*s.Dorm2.nsysu.edu.tw",	// mutli, rude words, dragoon
});

#ifdef ENABLE_ANTISPAM
mapping spammer_player = ([]);
mapping spammer_ip = ([]);
string *penalty_attr = ({
    "str", "int", "dex", "con", "spi", "cps", "wis", "cor"
});
#endif	/* ENABLE_ANTISPAM */

private void get_id(string arg, object ob);
private void confirm_id(string yn, object ob);
private void get_email(string email, object ob);
object make_body(object ob);
private void init_new_body(object link, object user);
varargs void enter_world(object ob, object user, int silent);
varargs void reconnect(object ob, object user, int silent);
object find_body(string name);
int check_legal_id(string arg);
int check_legal_name(string arg);
private int list_user_race(object link);
private void increment_visitor_count();
private int check_ip(object link);

private void
create() 
{
    seteuid(getuid());
    set("channel_id", "連線精靈");
}

private void
reset()
{
    object room, ob;

#if 0
    /* 清空在 void 裡的斷線玩家。 */
    room = find_object(VOID_OB);
    seteuid(getuid());
    if( objectp(room) )
	foreach(ob in all_inventory(room)) {
#ifdef	SAVE_USER
	    if( userp(ob) ) ob->save();
#endif
	    destruct(ob);
	}
#endif

    /* 記錄線上使用者人數 */
    log_file("USRGRAPH", sprintf("[%s] %d users\n",
	ctime(time()), sizeof(users())));

#ifdef ENABLE_ANTISPAM
    spammer_player = ([]);
    spammer_ip = ([]);
#endif
}

void
logon (object ob)
{
    object *usr;
    int i, wiz_cnt, ppl_cnt, login_cnt;

    if (getuid(ob) != ROOT_UID) {
	// only allow trusted login object
	return;
    }

#ifdef ENABLE_ANTISPAM
    if (spammer_ip[query_ip_number(ob)] >= 10) {
        write("從您連線的主機創造的人物太多了﹐您的主機將被拒絕往來一段時間。\n");
        destruct(ob);
        return;
    }
#endif

    seteuid (getuid());
    efun::write (read_file(WELCOME) + "\n");

    UPTIME_CMD->main();
    VISITOR_CMD->main();

    usr = users();
    wiz_cnt = 0;
    ppl_cnt = 0;
    login_cnt = 0;
    // invis wizard count in ppl in stead of wiz, by grain (03/25/1998)
    for(i=0; i<sizeof(usr); i++) {
        if( !environment(usr[i]) ) login_cnt++;
        else if( wizardp(usr[i]) ) {
            if( !usr[i]->link()->query("invis") ) wiz_cnt++;
        }
        else ppl_cnt++;
    }
    printf("目前共有 %d 位巫師、%d 位玩家在線上﹐以及 %d 位使用者嘗試連線中。\n\n",
        wiz_cnt, ppl_cnt, login_cnt );

    write ("您的使用者代號：");
    input_to ("get_id", ob);
}

private void
get_id (string arg, object ob)
{
    object ppl;

    // user id will be used as object uid for all objects created by the user.
    // the id string must be converted too all lower case, while objects created by backend are assigned
    // an uid with first character captialized. Thus we prevents a user called himself 'root' to become
    // privileged.
    arg = lower_case (arg);

    if (!check_legal_id(arg)) {
	write("您的使用者代號：");
	input_to ("get_id", ob);
	return;
    }

    if (getuid (ob) != ROOT_UID)
	return;
    seteuid (arg);
    export_uid (ob);
    seteuid (getuid());

    ob->set("id", arg);

#ifdef MAX_USERS
    if( wizhood(arg)=="(player)" && sizeof(users()) >= MAX_USERS )
    {
	ppl = find_body(arg);
	// Only allow reconnect an interactive player when MAX_USERS exceeded.
	if( !ppl || !interactive(ppl) ) {
	    write("對不起﹐" + MUD_NAME + "的使用者已經太多了﹐請待會再來。\n");
	    destruct(ob);
	    return;
	}
    }
#endif

#ifdef ENABLE_BAN_SITE
    // Rework by Annihilator (11/10/1999), support IP address and hostname
    if( wizhood(arg)=="(player)" )
    {
	string ip, pattern;

	ip = query_ip_number(ob);
	foreach(pattern in banned_ip)
	    if( ip==pattern || sscanf(ip, pattern) ) {
		write("您的連線位置目前不接受使用者登入。\n");
		destruct(ob);
		return;
	    }
	ip = query_ip_name(ob);
	foreach(pattern in banned_hostname)
	    if( ip==pattern || sscanf(ip, pattern) ) {
		write("您的連線位置目前不接受使用者登入。\n");
		destruct(ob);
		return;
	    }
    }
#endif	/* ENABLE_BAN_SITE */

#ifdef WIZ_LOCK_LEVEL
    if ((int)wiz_level(arg) < (int)wiz_lock_level) {
        write(MUD_NAME + "目前限制巫師等級 " + WIZ_LOCK_LEVEL
            + " 以上的人才能連線。\n");
        destruct(ob);
        return;
    }
#endif

    if (arg=="guest") {
        // If guest, let them create the character.
        get_email( "guest@" + query_ip_name(ob), ob);
        return;
    }

    if( file_size(login_data(arg)) != -1 ) {
        if( ob->restore() ) {
            write("請輸入密碼﹕");
            input_to("get_passwd", 1, ob);
            return;
        }
        write("對不起﹐您的人物儲存檔出了一些問題﹐請利用 guest 人物通知巫師處理。\n");
        destruct(ob);
        return;
    }

    write("使用 " + (string)ob->query("id") + " 這個名字將會創造一個新的人物﹐您確定嗎(y/n)﹖");
    input_to("confirm_id", ob);
}

private void
get_passwd(string pass, object ob)
{
    string my_pass;
    object user;

    write("\n");

    if( !check_ip(ob) ) {
	write("對不起，您的連線位置不正確。\n");
	destruct(ob);
	return;
    }

    my_pass = ob->query("password");
    if( crypt(pass, my_pass) != my_pass ) {
        write("密碼錯誤！\n");
        destruct(ob);
        return;
    }

    user = find_body(ob->query("id"));
    if (user) {
	/* 斷線的使用者重新連線 */
        if( !user->link() ) {
            reconnect(ob, user);
            return;
        }
        write("您要將另一個連線中的相同人物趕出去﹐取而代之嗎﹖(y/n)");
        input_to("confirm_relogin", ob, user);
        return;
    }

    user = make_body(ob);
    if( ! user ) {
	/* 如果沒有辦法製造使用者物件，直接切斷連線 */
	destruct(ob);
	return;
    }

    if( user->restore() )
    {
	log_file( "USAGE", sprintf("[%s] %s login from %s\n",
	    ctime(time()), (string)user->query("id"), query_ip_name(ob) ) );

	if( wizhood(ob)=="(admin)") {
	    if( (query_ip_name(ob) != "localhost")
	    &&	(query_ip_number(ob) != "127.0.0.1") ) {
		write("安全檢查失敗....自動登出。\n");
		destruct(user);
		destruct(ob);
		return;
	    }
	    write("安全檢查通過。\n");
	}
	enter_world(ob, user);
	return;
    } else {
	if( file_size(user->query_save_file())==-1 ) {
	    write(@NOTICE
系統找不到您的人物資料，可能的原因包括您在創造人物或人物在投胎轉世時
斷線，或者因為違反規定您的人物資料被刪除了。如果您確定這個人物並沒有
上述這些情況，請用 guest 帳號洽線上巫師查詢。

NOTICE
	    );
	    destruct(user);
	    write(HIY "您要重新創造這個人物嗎？(y/n) " NOR);
	    input_to("confirm_reincarnate", ob);
	} else {
	    write(@NOTICE
系統目前無法讀取您的人物資料，可能的原因包括系統正在備分或整理使用者
資料，請稍候再試。
NOTICE
	    );
	    destruct(user);
	    destruct(ob);
	}
    }
}

private void
confirm_reincarnate(string yn, object ob)
{
    if( yn=="" ) {
        write(HIY "您要重新創造這個人物嗎？(y/n) " NOR);
        input_to("confirm_reincarnate", ob);
        return;
    }

    if( yn[0]!='y' && yn[0]!='Y' ) {
        write("好吧﹐歡迎下次再來。\n");
        destruct(ob);
        return;
    }

    if( !list_user_race(ob) ) {
	/*  業力不足，無法投胎轉世的狀況。 */
#ifdef	SAVE_USER
	rm(ob->query_save_file());
#endif
        destruct(ob);
        return;
    }

    input_to("get_race", ob);
}

private void
confirm_relogin(string yn, object ob, object user)
{
    object old_link;

    if( yn=="" ) {
        write("您要將另一個連線中的相同人物趕出去﹐取而代之嗎﹖(y/n)");
        input_to("confirm_relogin", ob, user);
        return;
    }       

    if( yn[0]!='y' && yn[0]!='Y' ) {
        write("好吧﹐歡迎下次再來。\n");
        destruct(ob);
        return;
    } else {
        tell_object(user, "有人從別處( " + query_ip_number(ob)
            + " )連線取代您所控制的人物。\n");
        log_file( "USAGE", sprintf("[%s] %12s replaced @ %s\n",
            ctime(time()),
            (string)user->query("id"),
            query_ip_name(ob),  ) );
    }

    // Kick out tho old player.
    old_link = user->link();
    if( old_link ) {
	seteuid(getuid());
	if( interactive(user) ) exec(old_link, user);
        destruct(old_link);
    }

    reconnect(ob, user);    
}

private void
confirm_id(string yn, object ob)
{
    if( yn=="" ) {
        write("使用這個名字將會創造一個新的人物﹐您確定嗎(y/n)﹖");
        input_to("confirm_id", ob);
        return;
    }       

    if( yn[0]!='y' && yn[0]!='Y' ) {
        write("好吧﹐那麼請重新輸入您的英文名字﹕");
        input_to("get_id", ob);
        return;
    }

#ifdef ENABLE_ANTISPAM
    if( spammer_player[ob->query("id")] ) spammer_player[ob->query("id")]++;
    else spammer_player[ob->query("id")] = 1;
    if( spammer_ip[query_ip_number(ob)] ) spammer_ip[query_ip_number(ob)]++;
    else spammer_ip[query_ip_number(ob)] = 1;
#endif
    write("請設定您的密碼﹕");
    input_to("new_password", 1, ob);
}

private void
new_password(string pass, object ob)
{
    write("\n");
    if( strlen(pass)<5 ) {
        write("密碼的長度至少要五個字元﹐請重設您的密碼﹕");
        input_to("new_password", 1, ob);
        return;
    }
#ifdef	ENABLE_MD5_PASSWORD
    ob->set("password", crypt(pass, sprintf("$1$%d", random(99999999))));
#else
    ob->set("password", crypt(pass, 0) );
#endif
    write("請再輸入一次您的密碼﹐以確認您沒記錯﹕");
    input_to("confirm_password", 1, ob);
}

private void
confirm_password(string pass, object ob)
{
    string old_pass;

    write("\n");
    old_pass = ob->query("password");
    if( crypt(pass, old_pass)!=old_pass ) {
        write("您兩次輸入的密碼並不一樣﹐請重新設定一次密碼﹕");
        input_to("new_password", 1, ob);
        return;
    }

    write(@TEXT
為了避免您的人物遭人盜用，ES2 將採用 email 認證方式保護人物
所有權，所以請您輸入一個可供接收認證用電子郵件的地址。 這個
電子郵件地址除了巫師以外，不會被其他使用者看到。
TEXT
    );
    write("您的電子郵件地址：");
    input_to("get_email",  ob);
}

private void
get_email(string email, object ob)
{
    int c, delim=0, err=0;

    if( strlen(email) > 64 ) {
	write("電子郵件地址最多可以有 64 個字元。\n");
	write("您的電子郵件地址：");
	input_to("get_email",  ob);
	return;
    }

    foreach(c in email) {
	if( c=='@' && !delim ) { delim = 1; continue; }
	if( strsrch("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.", c) < 0 )
	    { err = 1; break; }
    }
    if( !delim || err ) {
	write("您的電子郵件格式錯誤，請輸入正確的電子郵件地址。\n");
	write("您的電子郵件地址：");
	input_to("get_email",  ob);
	return;
    }

    ob->set("email", email);

//  If ONE_GUEST is defined in /include/login.h, only permit one guest login.
#ifdef ONE_GUEST
    if ((string)ob->query("id") == "guest") {
        write("抱歉, 線上已經有一個 Guest 了.\n");
        destruct(ob);
        return;            }
#endif /* ONE_GUEST */

    // Complete non-body-specific initialization of new user here.
    ob->set("karma", 20);
    list_user_race(ob);
    input_to("get_race", ob);
}

private void
get_race(string race, object ob)
{
    int kar;
    string choice;
    if( sscanf(race, "? %s", race) ) {
        write( read_file(HELP_DIR + "help/" + race) );
        // add by ueiren ... 
        // list_user_race(ob);
        input_to("get_race", ob);
        return;
    }
    if( member_array(race, user_race)==-1 ) {
        write("沒有這種種族﹐請您重新選擇﹕");
        input_to("get_race", ob);
        return;
    }

    kar = (int)RACE_D(race)->query("karma");
    if( wizhood(ob)=="(player)" && (int)ob->query("karma") < kar ) {
        write("您的業力不夠﹐請您重新選擇﹕");
        input_to("get_race", ob);
        return;
    }
    ob->add("karma", -kar);

    write("您要扮演男性(m)的角色或女性(f)的角色﹖");
    input_to("get_gender", ob, race);
}

private void
get_gender(string gender, object ob, string race)
{
    object body;
    string body_file;

    if( gender=="" ) {
        write("您要扮演男性(m)的角色或女性(f)的角色﹖");
        // fix 選性別錯誤造成 race 為 human bug  .... by ueiren
        // input_to("get_gender", ob, body);
        input_to("get_gender", ob, race);           
        return;
    }

    if( gender[0]=='m' || gender[0]=='M' )      gender = "male";
    else if( gender[0]=='f' || gender[0]=='F' )     gender = "female";
    else {
        write("對不起﹐您只能選擇男性(m)或女性(f)的角色﹕");
        // fix 選性別錯誤造成 race 為 human bug  .... by ueiren 
        // input_to("get_gender", ob, body);
        input_to("get_gender", ob, race);
        return;
    }

    if( !stringp(body_file = RACE_D(race)->query("default_body")) )
        body_file = USER_OB;
        ob->set("body", body_file);
        if( !(body = make_body(ob)) ) {
            destruct(ob);
            return;
        }

        body->set("gender", gender);
        body->set_race(race);
        init_new_body(ob, body);
        // Remember it so we can dest it if we go linkdead before finishing
        // chraracter creation.
        ob->set_temp("temp_body", body);

        write("您的中文名字﹕");
        input_to("get_name", ob, body);
}

private void
get_name(string arg, object ob, object user)
{
    if( !check_legal_name(arg) ) {
        write("您的中文名字﹕");
        input_to("get_name", ob, user);
        return;
    }

    ob->set("name", arg);
    user->set("name", arg);
    if( !ob->query("creation_time") ) {
        log_file( "USAGE", sprintf("[%s] %12s created  @ %s\n",
            ctime(time()),
            user->query("id"),
            query_ip_name(ob), ) );
            ob->set("creation_time", time());
            // default open chat and rumor channel -- by dragoon
            ob->set("channels", ({"chat","rumor"}));
    }
    enter_world(ob, user);
}

object
make_body(object link_ob)
{
    string err;
    object user;
    int n;

    seteuid(getuid());
    err = catch(user = new(USER_OB));
    if(err || !user) {
        write("現在可能有巫師正在修改使用者物件的程式﹐請稍候再試。\n");
        return 0;
    }

    /* 設定使用者的權限 */
    seteuid(getuid(link_ob));
    export_uid(user);
    seteuid(getuid());

    user->set("id", link_ob->query("id"));
    user->set_name( link_ob->query("name"), link_ob->query("id") );

    return user;
}

private int
check_ip(object link)
{                                                                               
    string okip, cur_ip, cur_ip_num, ip_part, num_part, be_checked;
    int len, ed;

    okip = link->query("okip");
    if( !arrayp(okip) || !sizeof(okip) ) return 1;

    cur_ip = query_ip_name(link);
    cur_ip_num = query_ip_number(link);
    foreach(string ip in explode(okip, ":") - ({ "" }))
    {
	if (sscanf(ip, "%s*", be_checked) && be_checked!="")
	{
	    len = strlen(be_checked);
	    ip_part = cur_ip[0..len-1];
	    num_part = cur_ip_num[0..len-1];
	}
	else if (sscanf(ip, "*%s", be_checked) && be_checked!="")       
	{
	    len = strlen(be_checked);
	    ed = strlen(cur_ip);
	    ip_part = cur_ip[ed-len..<1];
	    num_part = cur_ip_num[ed-len..<1];
	}
	else
	{
	    be_checked = ip;
	    ip_part = cur_ip;
	    num_part = cur_ip_num;
	}
       
	if (lower_case(be_checked) == lower_case(ip_part)
	|| lower_case(be_checked) == lower_case(num_part))
	{
	    write(HIW "檢查通過。\n" NOR);
	    return 1;
	}
    }

    return 0;
}

private void
init_new_body(object link, object user)
{
#ifdef ENABLE_ANTISPAM
    int penalty;
    string a;
#endif

    user->set("birthday", time() );
    user->set_class("commoner");
    user->set_level(1);

#ifdef ENABLE_ANTISPAM
    penalty = spammer_player[user->query("id")] - 1;
    if( penalty < spammer_ip[query_ip_number(link)] )
        penalty = spammer_ip[query_ip_number(link)];
        while(penalty-- > 0) {
            a = penalty_attr[random(sizeof(penalty_attr))];
            if( user->query_attr(a) > 1 )
                user->set_attr(a, user->query_attr(a)-1);
        }
#endif

    CHAR_D->setup_char(user);
}

varargs void
enter_world(object ob, object user, int silent)
{
    object room;
    string startroom, err;

    user->set_link(ob);
    ob->set_body(user);

    // finished logon, transfer the interactive user from logon to user body
    exec (user, ob);

    if (!silent)
	write ("目前權限﹕" + wizhood(user) + "\n");

    user->setup();
    increment_visitor_count();

#ifdef SAVE_USER
    // save newly created user
    user->save();
#endif

    if (silent)
	return;

    cat(MOTD);
    IDENT_D->query_userid((string)user->query("id"));

    /* 以安全的方法載入使用者登入的房間 */
    startroom = user->query("startroom");
    if( !startroom ) startroom = START_ROOM;
    err = catch(room = load_object(startroom));
    if( !room ) err = catch(room = load_object(VOID_OB));

    if( !room || !user->move(room) ) {
	write(@NOTICE
對不起，目前系統正在整修一些登入地點的程式，請稍候再試。
NOTICE
	);
	destruct(ob);
	destruct(user);
	return;
    }

    /* 檢查使用者是否有未讀的信件 */
    if( ob->query("new_mail") ) {
	write( HIW "\n有您的信！請到驛站來一趟 ...\n\n" NOR);
	ob->delete("new_mail");
    }
 
    // if detect mark: pker, set the time mark -dragoon
    if( user->query("pker") ) {
	user->set("last_pk_time", time());
	user->delete("pker");
    }

    /* 宣告使用者登入的消息 */
    if( !wizardp(user) && !user->query("invis") ) {
	message("vision", user->query("name") + "連線進入這個世界。\n",
		room, user);
	CHANNEL_D->do_channel( this_object(), "sys",
		sprintf("%s(%s)由%s連線進入。",
		user->name(1),
		user->query("id"),
		query_ip_name(user))
	);
    }
}

varargs void
reconnect(object ob, object user, int silent)
{
    /* 將使用者從連線物件 exec 到使用者物件上 */
    user->set_link(ob);
    ob->set_body(user);
    exec(user, ob);

    user->reconnect();
    IDENT_D->query_userid((string)user->query("id"));   

    if( silent ) return;

    // if detect pking, reset time mark to now -dragoon
    if( time() - (int)user->query("last_pk_time") < 60 * 60 )
	user->set("last_pk_time", time());

    /* 宣告使用者重新連線的消息 */
    if( !wizardp(user) && !user->query("invis") ) {
	message("vision", user->query("name") + "重新連線回到這個世界。\n",
		environment(user), user);
	CHANNEL_D->do_channel( this_object(), "sys",
		sprintf("%s(%s)由%s重新連線進入。",
		user->name(1),
		user->query("id"),
		query_ip_name(user))
	);
    }
}

void
net_dead(object ob)
{
    CHANNEL_D->do_channel( this_object(), "sys",
        sprintf("%s(%s)斷線了。", ob->name(1), ob->query("id")));
    ob->move("/obj/void");
}

int
check_legal_id (string id)
{
    int i;

    i = strlen (id);
    if ((i < 3) || (i > 12 )) {
        write ("您的使用者代號必須是 3 到 12 個英文字母。\n");
        return 0;
    }
    while (i--)
        if ((id[i]<'a') || (id[i]>'z')) {
            write("您的使用者代號只能由 a 到 z 的英文字母組成。\n");
            return 0;
        }

    return 1;
}

int
check_legal_name(string name)
{
    int i;
    string bname;

    i = strlen(name);

    if( (strlen(name) < 3) || (strlen(name) > 18 ) ) {
        write("您的顯示名稱必須使用長度 3 到 18 的合法 UTF-8 文字。\n");
        return 0;
    }
    while(i--) {
        if( name[i]<=' ' ) {
            write("您的顯示名稱不能用控制字元。\n");
            return 0;
        }
        if( name[i..i+1]=="　" ) {
            write("請不要用空白字元取名字。\n");
            return 0;
        }
    }
    if( member_array(name, banned_name)!=-1 ) {
        write("這種名字會造成其他人的困擾。\n");
        return 0;
    }
    foreach(bname in banned_rude_name)
        if( strsrch( name, bname )>-1 ) {
            write("這種名會引起不必要的誤會。\n");
            return 0;
        }
    return 1;
}

object
find_body(string name)
{
    object ob, *body;

    if( objectp(ob = find_player(name)) ) return ob;
    foreach(ob in children(USER_OB))
        if( clonep(ob)
        &&  userp(ob)
        &&  ob->is_character()
        &&  getuid(ob) == name )
            return ob;

    return 0;
}

int
set_wizlock(int level)
{
    if( wiz_level(this_player(1)) <= level ) return 0;
    if( geteuid(previous_object()) != ROOT_UID ) return 0;

    wiz_lock_level = level;
    return 1;
}

static int
list_user_race(object link)
{
    string msg, race;
    int karma;

    karma = link->query("karma");
    msg = "";
    foreach(race in user_race) {
	if( RACE_D(race)->query("karma") > karma ) continue;
	msg += sprintf("%-25s %d 點業力\n",
		to_chinese(race) + "(" + race + ")",
		RACE_D(race)->query("karma") );
    }

    if( msg=="" ) {
	write("你所剩的業力已經沒有辦法投胎轉世了！\n");
	return 0;
    }

    msg = "您現在共有 " + karma + " 點業力，可以選擇以下的種族：\n" + msg;
    msg += "您的選擇(用 '? <種族名>' 可查閱\說明)﹕";

    write(msg);
    return 1;
}

/*  reincarnate()
 *
 *  這個函數是用來讓一個 player 投胎轉世用的。原則上只有 CHAR_D 才需要
 *  叫用這個功能，叫用這個函數的物件必須具有 ROOT 權限。
 */

void
reincarnate(object ob)
{
    object link;
    int max_karma_gain, karma_gain;

    if( previous_object() && geteuid(previous_object()) != ROOT_UID )
	return;

    seteuid(getuid());

    /* 如果進行轉世的人物斷線了，等下次連線的時候再轉世 */
    link = ob->link();
    if( ! link ) {
#ifdef	SAVE_USER
        ob->save();
#endif
        destruct(ob);
        return;
    }

    /* 每次獲得的業力為 MIN(上線天數*2, 轉世時人物的等級) */
    max_karma_gain = link->query("time_aged") / 43200;
    karma_gain = ob->query_level();
    if( karma_gain > max_karma_gain ) karma_gain = max_karma_gain;
    link->add("karma", karma_gain);
#ifdef	SAVE_USER
    link->save();
#endif

    /* 將使用者 exec 到連線物件上，刪除人物資料檔 */
    exec(link, ob);
#ifdef	SAVE_USER
    rm(ob->query_save_file());
#endif
    destruct(ob);

    /* 如果所剩業力已經不夠轉世成任何種族，宣告 GAME OVER */
    if( !list_user_race(link) ) {
        write("請您重新創造一個人物，重新再來吧。\n");
#ifdef	SAVE_USER
        rm(link->query_save_file());
#endif
        destruct(link);
        return;
    }

    input_to("get_race", link);
}

#define VISITOR_COUNTER_FILE	"/adm/etc/visitor.cnt"

private void
increment_visitor_count()
{
    int t, cnt;
    string s = read_file (VISITOR_COUNTER_FILE);

    if (!s) 
        s = sprintf("%d 1", time());
    else {
        sscanf(s, "%d %d", t, cnt);
        if (! t) { t = time(); cnt = 0; }
        s = sprintf("%d %d", t, cnt+1);
    }
    write_file (VISITOR_COUNTER_FILE, s, 1);
}
