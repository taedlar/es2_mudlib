// spirit.c
// ask player some question, and log some data to check he is robot or not
#include <npc.h>
#include <ansi.h>

//default question and answer, can be changed by setq seta
string question = "請問你現在在那裡?";
string answer = "機器人島";

void ask_q();

void create()
{
	set_name("小精靈", ({ "spirit","robot" }) );
	set("long", "
小精靈對看著你, 對你露出詭異的笑容。
小精靈拿著一個小簿子, 在上面記錄著不回答問題的 robot, 一付要找巫師報告的神情.
	");
	set("chat_chance",20);
	set("chat_msg", ({
		(: ask_q :),
	}));
	setup();
}

void init()
{
	int arr_time = time();
	object me = this_player();
	
	if(!userp(me))
		return;
	if(wizardp(me)) {
		add_action("set_question", "setq");
		add_action("set_answer", "seta");
		add_action("about_robot", "about");
		add_action("robot_help", "help");
		call_out((:message("tell_ob", HIY+ "<help spirit> to complement robot check\n" +NOR, $1):), 1, me);
		return;
	}
	if(!me->query_temp("robot/time")){
		call_out((:message("tell_en", HIY+ "小精靈笑道: " +$1+ " 來陪我了, 耶! 現在是 " +ctime($2)+ ".\n" +NOR, $3):), 2, me->query("id"), arr_time, environment());
		me->set_temp("robot/time", arr_time);
		me->set_temp("robot/question", question);
		me->set_temp("robot/answer", answer);
		log_file("block_robot", sprintf("%s %s %12s ====init====\n",
			ctime(time()), ctime(arr_time), me->query("id")));
	}
}

//隨便找一個在場的 player, 告訴他 他的問題
void ask_q()
{
	object *all, target;
	int i, tmp;

	all = all_inventory(environment(this_object()));
	i = sizeof(all);
	target = all[random(i)];
	if(!target || target == this_object())
		return;
	if( wizardp(target)
	|| target->query_temp("robot/time") <=0 )
		return;

	if( (tmp=(time() - target->query_temp("robot/time"))/60) > 20 )
		tell_object(target, tmp+ " 分鐘了, 你屬於 robot 喔.\n");
	tell_object( target, "小精靈說道: 請用 say 回答我下面的問題: \n" +question+ "\n>");
}

//針對 who, 設計他的 question
int set_question(string arg)
{
	string who, que;
	object target;
	if(!arg || arg==""){
		write("SYNTAX: setp <ID> <question>\n");
		return 1;
	}
	if(sscanf(arg, "%s %s", who, que)!=2){
		write("SYNTAX: setp <ID> <question>\n");
		return 1;
	}
	if( !(target=present(who, environment())) ){
		write(who+ " 並不在這, 先把他 summon 來吧.\n");
		return 1;
	}
	target->set_temp("robot/question", que);
	write("你把 " +target->name()+ "的問題設為: " +que+ "\n");
	return 1;
}

//針對 who 設其 answer
int set_answer(string arg)
{
	string who, ans;
	object target;
	if(!arg || arg==""){
		write("SYNTAX: setp <ID> <answer>\n");
		return 1;
	}
	if(sscanf(arg, "%s %s", who, ans)!=2){
		write("SYNTAX: setp <ID> <answer>\n");
		return 1;
	}
	if( !(target=present(who, environment())) ){
		write(who+ " 並不在這, 先把他 summon 來吧.\n");
		return 1;
	}
	target->set_temp("robot/answer", ans);
	write("你把 " +target->name()+ "之問題 <"
		+target->query_temp("robot/question")+ "> 的答案設為 <" +ans+ ">\n");
	return 1;
}

//查某人 之 robot 資訊
int about_robot(string who)
{
	object target;
	int arr_time;
	if(!who){
		write("SYNTAX: about <ID>\n");
		return 1;
	}
	if( !(target=present(who, environment())) ){
		write(who+ " 並不在這, 先把他 summon 來吧.\n");
		return 1;
	}
	if( !(arr_time=target->query_temp("robot/time")) ){
		write(who+ " 的到達時間已消毀, 可能有離線過, 或為巫師或...\n");
		return 1;
	}
	write(who+ " 開始時間: " +ctime(arr_time)+ "\n");
	write(who+ " 觀察時間: 已有 " +(time() - arr_time)/60+ " 分鐘沒答對\n");
	write(who+ " 的問題為: " +target->query_temp("robot/question") +"\n");
	write(who+ " 應要回答: " +target->query_temp("robot/answer") +"\n");
	return 1;
}

int robot_help(string arg)
{
	if(arg!="spirit")	return 0;
	write(@HELP
1. summon <ID>
2. setq <ID> <question>
3. seta <ID> <answer>
4. about <ID>
若要查某 player 是否為 robot, 可讓小精靈問他一個問題, 此 robot 何時到, 何
時 say 了那些話, 均會記錄成檔, 大巫師以上會在有空時查看記錄並處理.
App, Wiz 懷疑 robot 時均可利用此小精靈, 但請確定此玩家在被你抓來之前是處於
練功的狀態, 並以 post 描述一下情形.
HELP
	);
	return 1;
}

//以 say 回答, 20 min 內若答對會清掉 temp mark 並 move 至 snow
void relay_say(object me,string msg)
{
	int tmp;
	log_file("block_robot", sprintf("%s %12s %8s <%s>\n", 
		ctime(time()),
		me->query("id"),
		me->query_temp("robot/time")>0 ? 
			(time() - me->query_temp("robot/time"))/60+ " min" : " * min",
		msg,
	));

	if( (tmp=(time() - me->query_temp("robot/time"))/60) > 20 ){
		tell_object(me, 
"你已超過 " +tmp+ " 分鐘沒答對, 大巫師以上會在有空時處理(砍檔), 若有疑議請及早反
應, 並詳述狀況理由. 若你要 suicide 重練, 記得 post 通知巫師, 否則新練角色
被砍不負責. 若要離開, 請斷線 15 分鐘\n"
		);
		return;
	}
	if( me->query_temp("robot/answer") ){
		if( strsrch(msg, me->query_temp("robot/answer"))>=0 ){
//		CHANNEL_D->do_channel( this_object(), "chat", "那個巫師有空, 來綠島一下.") );
			tell_object(me, "小精靈笑道: 你答對了, 下次有空再來玩.\n小精靈向你吹了口氣, 你輕飄飄地飛了出去.\n\n");
			if( me->move("/d/snow/inn_hall") ){
				tell_room(environment(me), me->name()+ "被小精靈吹了過來.\n");
				log_file("block_robot", sprintf("%s %12s %8s 後答對離開\n",
					ctime(time()),
					me->query("id"),
					(int)(time() - me->query_temp("robot/time"))/60+ " min",
				));
				this_player()->delete_temp("robot/");	//clean robot record
				return;
			}
			tell_object(me, "小精靈道: 好重喔,  吹不動你, 你去找別人幫忙把你吹出去, 人家不管了.\n");
			return;
		}
	}
	tell_object(me, "小精靈道: 你好像答錯了, 還是你說的話我聽不懂, 我再問一次:\n" +me->query_temp("robot/question")+ "\n");
}
