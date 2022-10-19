// vim: syntax=lpc

#include <command.h>
#include <login.h>
#include <action.h>

inherit F_DBASE;
inherit F_CLEAN_UP;

private void
create() 
{
  seteuid(getuid());
  set("channel_id", "離線指令");
}

int
main(object me, string arg)
{
    int i;
    object link_ob, ob;

    if (me->is_busy()) {
// 此功能可使玩家用來消去start_busy的作用, cancel interrupt by dragoon
//        me->interrupt(me, INTR_QUIT);
        write("你正忙著呢, 請試著停下所忙的事。\n");
        return 1;
    }

    if (me->query("life_form") == "ghost" && arg != "!")
	return notify_fail("你現在離開將會無法復活﹐如果確定要離開請用 'quit !'。\n");

    if (me->query_team())
	me->dismiss_team();
        
    if( !wizardp(me) ) {
	foreach(ob in all_inventory(me)) {
	    if( ob->query_autoload() ) continue;	/* 自動儲存的物件不丟棄 */
	    if( !ob->query("value") ) continue;		/* 沒有價值的物件不丟棄 */
	    DROP_CMD->do_drop(me, ob);
	}
    }

    link_ob = me->link();

    // We might be called on a link_dead player, so check this.
    if( link_ob ) {

        // Are we possessing in others body ?
        if( link_ob->is_character() ) {
            write("你的魂魄回到" + link_ob->name(1) + "的身上。\n");
            exec(link_ob, me);
            link_ob->setup();
            return 1;
        }

        link_ob->set("last_on", time());
        link_ob->set("last_from", query_ip_name(me));
#ifdef SAVE_USER
        link_ob->save();
#endif
        destruct(link_ob);
    }

    // if pking, set a pker mark before quit -dragoon
    if( time() - (int)me->query("last_pk_time") < 60 * 60 )
	me->set("pker", 1);

    write("歡迎下次再來﹗\n");
    message("system", me->name() + "(" + me->query("id") + ") 離開遊戲。\n",
	environment(me), me);

    CHANNEL_D->do_channel(this_object(), "sys",    me->name() + "("+ me->query("id") +") 離開遊戲了。");

#ifdef SAVE_USER
    me->save();
#endif
    // log in USAGE include login and logout 
    log_file( "USAGE", sprintf("[%s] %12s quit\n",
        ctime(time()), (string)me->query("id")));

    // if ghost quit, log it -Dragoon
    if( me->query("life_form") == "ghost" )
	log_file( "GHOST_QUIT", sprintf("[%s] %s quit in ghost form\n",
	    ctime(time()), (string)me->query("id")));

    destruct(me);

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : quit

當你(妳)想暫時離開時, 可利用此一指令。
HELP
    );
    return 1;
}
