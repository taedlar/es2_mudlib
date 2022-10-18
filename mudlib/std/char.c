// vim: syntax=lpc
#pragma save_binary

#include <login.h>
#include <action.h>
#include <ansi.h>
#include <char.h>
#include <command.h>
#include <condition.h>
#include <dbase.h>
#include <move.h>
#include <name.h>
#include <skill.h>
#include <user.h>

inherit F_ACTION;
inherit F_ATTACK;
inherit F_ATTRIBUTE;
inherit F_COMBAT;
inherit F_COMMAND;
inherit F_CONDITION;
inherit F_DBASE;
inherit F_FINANCE;
inherit F_MESSAGE;
inherit F_MASTER;
inherit F_MOVE;
inherit F_NAME;
inherit F_SCORE;
inherit F_SKILL;
inherit F_STATISTIC;
inherit F_TEAM;

// variables

private int tick;

// implementations

int is_character() { return 1; }

/* setup()
 *
 * 這個函式用來完成一個人物的初始化，並啟動所有人物物件可以接受的事件，包括
 * 心跳(heart beat)，指令(command)等。
 * setup 還會呼叫 CHAR_D 來完成 game-specific 的人物初始化。
 */

void
setup()
{
    seteuid(getuid());

    /* 只有複製的物件需要打開 heart beat 及以下的設定，用來節省 CPU time */
    if( !clonep(this_object()) ) return;

    enable_player();
    init_command();
    CHAR_D->setup_char( this_object() );

    set_heart_beat(1);
    tick = 5 + random(10);
}

/* heart_beat()
 *
 * 人物的心跳函式，這個函式每兩秒鐘會被 driver 呼叫一次，讓人物得以進行
 * 戰鬥等動作；使用者以及 NPC 特有的功能分別定義在 NPC 及 USER 的物件中
 * 。
 *
 * 由於這個函式被執行的次數是整個 mudlib 中最高的，因此若不作相當的最佳
 * 化，很容易導致效能的大幅降低，如果你對 LPC 還沒有很熟，請不要隨意更動
 * 這個函式的內容。
 */

private void
heart_beat()
{
  mapping f;
  int f_altered = 0;

  if (mapp(f=query_destroyed()) && sizeof(f))
    {
      f_altered++;
      RACE_D(query_race())->statistic_destroyed(this_object(), f);
      if (!this_object())
	return;
    }

  if (mapp(f=query_exhausted()) && sizeof(f))
    {
      f_altered++;
      RACE_D(query_race())->statistic_exhausted(this_object(), f);
      if (!this_object())
	return;
    }

  if (mapp(f=query_notified()) && sizeof(f))
    {
      f_altered++;
      this_object()->notify_stat(f);
      if (!this_object())
	return;
    }

  if (f_altered)
    clear_statistic_flags();

  /* 戰鬥回合的控制 */
  if (living(this_object()))
    {
      if (is_fighting())
	clean_up_enemy();

      if (is_busy())
	continue_action();
      else if (is_fighting())
	attack();
    }

  if (tick--)
    return;

  // shuffle number of ticks until next update
  tick = 5 + random(10);

  update_condition();
  regenerate();
}

void
lose_fight (object opponent)
{
    if( opponent ) opponent->win_fight(this_object());
}

// notify_stat()
//
// This is an apply support by F_STATISTIC which is called by heart_beat()
// when statistic_notification is detected.
int
notify_stat (mapping flag)
{
    mapping exits;
    string *dirs, default_dir;

    if( !living(this_object())
    ||	!environment()
    ||	!mapp(exits = environment()->query("exits"))
    ||	!sizeof(dirs = keys(exits)) )
        return 0;

    /* 如果有設定預設逃跑方向，則先嘗試往這個方向逃跑，否則隨機選一個
     * 方向跑。
     */
    default_dir=query("wimpy_dir");

    if( stringp(default_dir) && member_array(default_dir, dirs) 
    &&	command("go " + default_dir) )
	return 1;

    return command("go "+ dirs[random(sizeof(dirs))]);
}

varargs void
revive (int quiet)
{
    object ob;
    string arg, arg2;

    remove_call_out("revive");

    /* 如果這個人物被其他人物攜帶著，移動到攜帶者的外面 */
    while (environment()->is_character())
        this_object()->move(environment(environment()));

    enable_player();
    delete_temp("block_msg/all");

    if (!quiet) {
        COMBAT_D->announce(this_object(), "revive");
        message("system", HIY "\n慢慢地你終於又有了知覺....\n\n" NOR,
            this_object());
    }
}

/*  人物的死亡
 */
void
die()
{
    object corpse, ob;

    /* 停止一切的戰鬥、保護、攻擊選擇的關係 */
    remove_all_killer();
    guard_ob(0);
    charge_ob(0);
    dismiss_team();

    /* 停止任何正在進行的動作 */
    if (is_busy())
	interrupt(this_object(), INTR_DEATH);

    /* 清除所有的 condition */
    clear_condition();

    /* 宣告這個人物的死亡 */
    COMBAT_D->announce(this_object(), "dead");

    /* 如果這名人物在昏迷中，讓他「安靜地」醒過來，這樣才看得到自
     * 己死亡的訊息
     */
    if (!living(this_object()))
	revive(1);

    if (query("life_form") == "ghost") {
        tell_object(this_object(), HIR "\n你已經魂飛魄散了！！！\n\n" NOR);
        message("vision", this_object()->name() + "化成一陣煙，消失了 ...\n",
            environment(this_object()), this_object());
        return;
    }

    // 加上死亡提示敘述 -dragoon
    tell_object(this_object(), HIR "\n你死了！！\n\n" NOR);
    if (objectp(corpse = CHAR_D->make_corpse(this_object())) )
	corpse->move(environment());
}

varargs void
unconcious (string reason)
{
    object ob, me=this_object();

    /* 已經昏迷 ? */
    if (!living(me))
	return;

    /* 巫師不能昏迷 */
    if (wizardp(this_object())) {
        message("system", HIW "\n你的巫師能力治癒了身上全部的傷害 ....\n\n" NOR,
		me);
        me->set_stat_current("gin", me->query_stat_maximum("gin"));
        me->set_stat_current("kee", me->query_stat_maximum("kee"));
        me->set_stat_current("sen", me->query_stat_maximum("sen"));
        me->set_stat_current("HP", me->query_stat_maximum("HP"));
        me->set_stat_current("water", me->query_stat_maximum("water"));
        me->set_stat_current("food", me->query_stat_maximum("food"));
        return;
    }

    /* 停止所有 fight 中的敵人 */
    remove_all_enemy();

    /* 中斷正在進行的動作 */
    if( is_busy() ) interrupt(me, INTR_UNCONCIOUS);

    message("system", HIR "\n你的眼前一黑，接著什麼也不知道了....\n\n" NOR,
        this_object());

    disable_player(stringp(reason) ? reason : " <昏迷不醒>");
    set_temp("block_msg/all", 1);

    COMBAT_D->announce(this_object(), "unconcious");

    /* 設定一分鐘後甦醒 */
    remove_call_out("revive");
    call_out("revive", 60);
}

// overrides

int
receive_object(object ob, int from_inventory)
{
    if( from_inventory ) return 1;

    if( query_encumbrance() + ob->weight() > query_strength("carriage") )
        return notify_fail(ob->name() + "現在對你來說太重了。\n");

    if( sizeof(all_inventory(this_object())) >= 20 )
        return notify_fail("你身上已經沒有空位再攜帶其他東西了。\n");

    return 1;
}

varargs int
move(mixed dest, int silent)
{
    if( interactive(this_object()) && is_busy() )
	return notify_fail("你現在無法移動﹗\n");

    if( ::move(dest, silent) ) {
	if( interactive(this_object()) && !silent )
	    environment()->do_look(this_object());
	return 1;
    }

    return 0;
}

string
query_id()
{
    mixed* applied;

    if( arrayp(applied = query_temp("apply/id"))
    &&	sizeof(applied)
    &&	arrayp(applied[<1])
    &&	stringp(applied[<1][0]) )
        return applied[<1][0];

    return query("id");
}

int
id(string str)
{
    mixed* applied;

    if( arrayp(applied = query_temp("apply/id"))
    &&	sizeof(applied)
    &&	arrayp(applied[<1]) )
	return member_array(str, applied[<1]) >= 0;
    return ::id(str);
}

varargs string
name(int raw)
{
    string *applied_name;

    if( !raw
    &&	arrayp(applied_name = query_temp("apply/name"))
    &&	sizeof(applied_name)
    &&	stringp(applied_name[<1]) )
	return applied_name[<1];
    return ::name(raw);
}

/*  short()
 *
 *  這個函數傳回人物的簡短敘述，使用者在 look 一個房間中，若有其他人物，則
 *  顯示的就是這個字串。
 */
varargs string
short(int raw)
{
    string str, title, nick, *applied;

    str = ((title = query("title")) ? title : "")
        + ((nick = query("nickname")) ? ("「"+nick+"」") : (title?" ":""))
        + ::short(1);

    if( raw ) return str;

    /* 附加的短敘述 */        
    if(	arrayp(applied = query_temp("apply/short"))
    &&	sizeof(applied)
    &&	stringp(applied[<1]) ) {
	str = applied[<1];
	if( ! this_player()->query("option/BRIEF_SHORT") )
	    str += "(" + capitalize(query_id()) + ")";
    }

    if( !living(this_object()) ) str += HIR + query("disable_type") + NOR;

    if( !userp(this_object()) ) return str;

    // These user only case are defined here because the case that wizards
    // exec into an NPC.

    if( query("life_form")=="ghost" ) str = HIK + str + "的鬼魂" NOR;
    else if( query("life_form")=="undead" ) str = CYN + str + "的僵屍" NOR;

    if( query_temp("pending/hidden") )
	str += "正鬼鬼祟祟地躲在暗處 ...";

    if( !interactive(this_object()) ) str += HIG " <斷線中>" NOR;
    else if( query_idle( this_object() ) > 120 ) str += HIM " <發呆中>" NOR;

    if( in_input() ) str += HIC " <輸入文字中>" NOR;
    if( in_edit() ) str += HIY " <編輯檔案中>" NOR;

    return str;
}

varargs string
long(int raw)
{
    string str, *applied;

    if( !raw
    &&	arrayp(applied = query_temp("apply/long")) 
    &&	sizeof(applied)
    &&	stringp(applied[<1]) )
	str = applied[<1];
    else str = ::long(raw);

    if( !raw
    &&	arrayp(applied = query_temp("apply/description")) 
    &&	sizeof(applied)
    &&	stringp(applied[<1]) ) {
	str += implode(applied, "\n");
	str = replace_string(str, "$N", this_object()->name(raw));
    }

    return str;
}

varargs string
rank (string politeness, int raw)
{
    string c, r;

    c = this_object()->query_class();
    if (c)
        r = CLASS_D(c)->query_rank(this_object(), politeness);
    else
        r = ::rank(politeness, raw);

    if( !politeness && !raw)
        switch(wizhood(this_object())) {
        case "(admin)": return "大神";
        case "(arch)": return "大巫師";
        case "(wizard)": return "巫師";
        case "(apprentice)": return "見習巫師";
        case "(immortal)": return "客座巫師";
        }
    return r;
}

nomask int
visible(object ob)
{
    int a, b;

    /* 任何人物永遠都可以看到自己 */
    if( ob==this_object() ) return 1;

    /* 巫師永遠可以看到巫師等級比自己低的物件 */
    a = wiz_level(ob);
    b = wiz_level(this_object());
    if( wizardp(ob) && a>b ) return 1;
    if( query("invis") && a<=b ) return 0;

    if( !::visible(ob) ) return 0;

    /* 隱藏的物件需要足夠的能力才能發現 */
    if( query_temp("pending/hidden") > ob->query_ability("awarness") )
	return 0;

    /* 鬼魂需要特殊能力才能看見 */
    if( query("life_form")=="ghost"
    &&	!ob->query_temp("apply/vision_of_ghost") ) return 0;

    return 1;
}
