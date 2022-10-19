// vim: syntax=lpc

#include <ansi.h>

inherit F_CLEAN_UP;

object *all, *target;
string msg;
int i, j;

void msg_format(string arg);

private void
create()
{
  seteuid(getuid());
}

mixed
main(object me, string arg, int remote)
{
    int center_lv;

    target = ({});
    msg = "";

    /* 先過濾尚未登入完成的使用者，與隱形的上級巫師。
     * 注意在被 rwho_q 呼叫的狀況下，this_player() 為 0。
     */
    if( this_player(1) )
	all = filter(users(), (:
	    objectp(environment($1))
	    && (!wizardp($1) || wiz_level($2)>wiz_level($1) || $1->visible($2))
	:), this_player(1));
    else
	all = filter(users(), (: objectp(environment($1)) && !wizardp($1) :));

    i = sizeof(all);
    
    if( !arg || arg=="" ) {
	center_lv = me->query_level();
        while( i-- ) {
            if( all[i]->query("level") - center_lv < 6
            &&  center_lv - all[i]->query("level") < 6
            && !wizardp(all[i]) )
                target += ({ all[i] });
        }
        msg_format("1");
    } else {
        switch(arg){
        case "-l":
            if( me->is_character() && !wizardp(me) ) {
                write("為了減低系統負荷, 目前只有巫師可以使用 who -l。\n");
                return 1;
            }
            target = all;
            msg_format("5");
            break;
	case "-1":
	    target = filter(all, (:($1->query_level()-1)/10==0 && !wizardp($1) :));
            msg_format("1");
            break;
	case "-2":
	    target = filter(all, (:($1->query_level()-1)/10==1 && !wizardp($1) :));
            msg_format("1");
            break;
        case "-3":
	    target = filter(all, (:($1->query_level()-1)/10==2 && !wizardp($1) :));
            msg_format("1");
            break;
        case "-4":
	    target = filter(all, (:($1->query_level()-1)/10==3 && !wizardp($1) :));
            msg_format("1");
            break;
        case "-5":
	    target = filter(all, (:($1->query_level()-1)/10==4 && !wizardp($1) :));
            msg_format("1");
            break;
	case "-w":
	    target = filter(all, (: wizardp :));
            msg_format("5");
            break;
        case "-i":
            msg_format("41");
            break;
        case "-n":
            msg_format("42");
            break;
        case "-bh":
	    target = filter(all, (: $1->query_race()=="blackteeth" :));
	    msg_format("50");
	    break;
        case "-hn":
	    target = filter(all, (: $1->query_race()=="human" :));
	    msg_format("50");
	    break;
	case "-wn":
	    target = filter(all, (: $1->query_race()=="woochan" :));
	    msg_format("50");
	    break;
        case "-jo":
	    target = filter(all, (: $1->query_race()=="jiaojao" :));
	    msg_format("50");
	    break;
        case "-yd":
	    target = filter(all, (: $1->query_race()=="yenhold" :));
	    msg_format("50");
	    break;
        case "-rr":
	    target = filter(all, (: $1->query_race()=="rainnar" :));
	    msg_format("50");
	    break;
        case "-aa":
	    target = filter(all, (: $1->query_race()=="ashura" :));
	    msg_format("50");
	    break;
        case "-hs":
	    target = filter(all, (: $1->query_race()=="headless" :));
	    msg_format("50");
	    break;
        case "-ya":
	    target = filter(all, (: $1->query_race()=="yaksa" :));
	    msg_format("50");
	    break;
        case "-ml":
	    target = filter(all, (: $1->query_race()=="malik" :));
	    msg_format("50");
	    break;
        case "-at":
	    target = filter(all, (: $1->query_class()=="alchemist" :));
	    msg_format("50");
	    break;
        case "-cr":
	    target = filter(all, (: $1->query_class()=="commoner" :));
	    msg_format("50");
	    break;
        case "-fr":
	    target = filter(all, (: $1->query_class()=="fighter" :));
	    msg_format("50");
	    break;
        case "-sr":
	    target = filter(all, (: $1->query_class()=="soldier" :));
	    msg_format("50");
	    break;
        case "-tf":
	    target = filter(all, (: $1->query_class()=="thief" :));
	    msg_format("50");
	    break;
        case "-tt":
	    target = filter(all, (: $1->query_class()=="taoist" :));
	    msg_format("50");
	    break;
        case "-sc":
	    target = filter(all, (: $1->query_class()=="scholar" :));
	    msg_format("50");
	    break;
	default :
	    if( objectp(me) && wizardp(me) &&  arg[0]=='@' ) {
		//RWHO_Q->send_rwho_q(arg[1..sizeof(arg)], me, 0);
		//write("網路訊息已送往 " + arg[1..sizeof(arg)] + " 請稍候。\n");
		return notify_fail("遠端查詢功能已關閉。\n");
	    }
	    return notify_fail("指令格式更新﹕請看 help who。\n");
        }
    }

    msg = sprintf("◎ %s\n"
	"───────────────────────────────────────\n"
	"%s───────────────────────────────────────\n"
	"共列出 %d/%d 位使用者﹐系統負荷﹕%s\n",
	MUD_NAME, msg, sizeof(target), sizeof(all), query_load_average());

    if( me && me->query_stat_current("sen") > 5 )
        me->consume_stat("sen", 5);
    
    if( remote ) return msg;

    me->start_more(msg);
    return 1;
}

int
sort_user(object ob1, object ob2)
{
    int ord;

    ord = wiz_level(ob1) - wiz_level(ob2);
    if( ord==0 )
        ord = (int)ob1->query_level() - (int)ob2->query_level();
    return ord;
}

void
msg_format(string arg)
{
    if( !arg || arg=="" )    return;

    j = sizeof(target);
    target = sort_array(target, "sort_user", this_object());

    switch(arg){
    case "1":
    case "5":
    case "50":
        // detail message
        while( j-- ) {
            // <ansi.h> will confuse sprintf's format
            if( wizardp(target[j]) )
                msg = sprintf("%s%s【 %|10s 】%+42s %-14s%s\n",
                    msg,
                    BOLD,
                    target[j]->rank(),
                    (target[j]->query("title") ? target[j]->query("title") : "") + (target[j]->query("nickname") ? "「"+ target[j]->query("nickname") +"」" : "") + (target[j]->query("name", 1)),
                    "(" + capitalize(target[j]->query("id")) + ")",
                    NOR
                );
            else
      		msg = sprintf("%s【 %2d%|8s 】%-12s%30s %-14s\n",
                    msg,
                    target[j]->query_level(),
                    target[j]->rank(),
                    target[j]->query("title") ? target[j]->query("title") : "",
		    (target[j]->query("nickname") ? "「"+ target[j]->query("nickname") +"」" : "")
			+ target[j]->query("name", 1),
                    "(" + capitalize(target[j]->query("id")) + ")",
                );
        }
        break;
    case "41":
        // who i, id of all
        target = sort_array(all, "sort_user", this_object());
        j = sizeof(target);
        while( j-- ) {
            msg = sprintf("%s%-13s%s",
                msg,
                capitalize(target[j]->query("id")),
                (( i - j ) % 6 ) ? "" : "\n"
            );
        }
        if( i % 6 )    msg += "\n";
        break;
    case "42":
        // who n, name of all
        target = sort_array(all, "sort_user", this_object());
        j = sizeof(target);
        while( j-- ) {
            msg = sprintf("%s%-13s%s",
                msg,
                target[j]->query("name", 1),
                (( i - j ) % 6 ) ? "" : "\n"
            );
        }
        if( i % 6 )    msg += "\n";
        break;
    default:
        msg = "Error at msg_format.\n";
    }
}

int
help(object me)
{
    write(@HELP
指令格式 : who [選項]
這個指令可以列出在線上的使用者及其等它相關資訊。神 > 5 時會損神。
選項有下列幾種, 一次只可使用一種選項, 即 who -i -w 視同 who -i。

    無選項時, 列出與你等級相差 5 級以內之使用者。
-1    列出等級  1 到等級 10 的使用者。
-5    列出等級 41 到等級 50 的使用者。
-w    只列出巫師, 有系統設計上的問題請與巫師溝通。
-i    只列出使用者英文 id。
-n    只列出使用者中文名稱。
-l    列出所有使用者詳細資訊 (巫師專用)。

-hn    列出 race 為  human 之使用者。    bh => blackteeth
    hn => human        wn => woochan    jo => jiaojao    yd => yenhold
    rr => rainnar    aa => ashura    hs => headless    ya => yaksa
    ml => malik
-at    列出 class 為 alchemist 之使用者。
    at => alchemist    cr => commoner    fr => fighter    sr => soldier
    tf => thief        tt => taoist	 sc => scholar

相關指令﹕ finger
HELP
    );
    if( wizardp(me) )
	write(@HELP
巫師可以利用 who @celestial.empire  來得知其他連線站之線上人數, 已連線站可
以mudlist 得知。
HELP
	);
    return 1;
}
