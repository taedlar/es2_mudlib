// vim: syntax=lpc
#include <ansi.h>
#include <command.h>
#include <dbase.h>
#include <origin.h>
#include <login.h>

/* 指令搜尋路徑。 */
private string *path = ({});

string
find_command(string verb)
{
  return (string)COMMAND_D->find_command(verb, path);
}

private nomask int
command_hook(string arg)
{
    string verb, file;

#ifdef ENABLE_BENCHMARK
    int mem, utime, stime;
    mapping info;

    mem = memory_info();
    info = rusage();
    utime = info["utime"];
    stime = info["stime"];
#endif

    verb = query_verb();

    /* 嘗試指令 */
    if( stringp(file = find_command(verb)) 
    &&	call_other(file, "main", this_object(), arg));

    /* 嘗試情緒指令 */
    else if( EMOTE_D->do_emote( this_object(), verb, arg ) );

    /* 嘗試頻道指令 */
    else if( CHANNEL_D->do_channel( this_object(), verb, arg ) );

    /* 嘗試 go */
    else if( (verb != "go") && environment()
    &&	stringp(environment()->query("exits/" + verb))
    &&	GO_CMD->main(this_object(), verb) );

    else return 0;

#ifdef ENABLE_BENCHMARK
    info = rusage();
    PROFILE_D->log_command(verb, memory_info() - mem, info["stime"] - stime,
	info["utime"] - utime);
#endif

    return 1;
}

private void
set_path(string *p)
{
    path = p;
}

/*  add_path()
 *
 *  這個函數用來提供一個增加指令搜尋路徑的介面。
 */
nomask void
add_path(string p)
{
    if( previous_object() && (geteuid(previous_object())!=ROOT_UID) )
	error("Permission denied\n");

    if( arrayp(path) && (member_array(p, path) < 0) ) {
	path += ({ p });
	/* 保護特權使用者的設計：指令搜尋路徑改變時，印出訊息讓使用者知道。 */
	if( wizardp(this_object()) )
	    receive(HIR "新增指令搜尋路徑：" NOR + p + "\n");
    }
}

mixed *
query_commands() { return commands(); }

int
force_me(string cmd)
{
    if( geteuid(previous_object()) != ROOT_UID )
	error("Permission denied\n");

    return command( userp(this_object()) ? this_object()->process_input(cmd) : cmd );
}

protected void
enable_player()
{
    /* 設定生物名稱 */
    if( stringp(query("id")) ) set_living_name(query("id"));
    else if( stringp(query("name")) ) set_living_name(query("name"));
    else set_living_name(getuid());

    /* 連接指令處理函數 */
    enable_commands();
    delete("disable_type");
}

protected void
init_command()
{
    if( !userp(this_object()) )
        set_path(NPC_PATH);
    else
	switch( wizhood(this_object()) )
	{
	case "(admin)":		set_path(ADM_PATH); enable_wizard(); break;
	case "(arch)":		set_path(ARC_PATH); enable_wizard(); break;
	case "(wizard)":	set_path(WIZ_PATH); enable_wizard(); break;
	case "(apprentice)":	set_path(APR_PATH); enable_wizard(); break;
	case "(immortal)":	set_path(IMM_PATH); break;
	default:		set_path(PLR_PATH); break;
	}

    add_action("command_hook", "", 1);
    add_action("cmd_quit", "quit");

    if( wizardp(this_object()) ) add_action("cmd_update", "update");
}

// cmd_quit()
//
// 這個是用來在萬一 quit 指令壞掉時，備用的 quit。
protected int
cmd_quit(string arg)
{
    if( !catch(call_other(QUIT_CMD, "main", this_object(), arg)) ) {
	// 若成功，this_object() 已經被 destruct 了，立即返回。
	return 1;
    }

#ifdef SAVE_USER
    this_object()->save();
#endif
    destruct(this_object());
}

protected int
cmd_update(string arg)
{
    if( !wizardp(this_object()) ) return 0;
    return call_other("/cmds/wiz/update", "main", this_object(), arg);
}

protected void
disable_player(string type)
{
    if( origin()==ORIGIN_CALL_OTHER
    &&	geteuid(previous_object())!=ROOT_UID
    &&	previous_object()!=this_object())
	return;

    set("disable_type", type);
    disable_commands();
}

