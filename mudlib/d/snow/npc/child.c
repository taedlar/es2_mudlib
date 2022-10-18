// child.c

#include <npc.h>

inherit F_VILLAGER;

void timeto_eat();

void create()
{
	set_name("阿寶", ({ "arbao", "__ID_ARBAO__" }) );
	set_attr("spi", 18);
	set_attr("int", 19);
	set_attr("dex", 19);
	set_attr("con", 17);
	set_race("human");
	set_class("commoner");
	set_level(10);

	set_skill("stealing", 40);
	set_skill("dodge", 35);
	set_skill("lightswift", 45);
	set_skill("dagger", 40);
	set_skill("sword", 30);
	
	map_skill("dodge", "lightswift");

	set("age", 16);
	set("gender", "female");
	set("long",
		"這個十五、六歲的小姑娘就是「阿寶」了﹐她身上雖然穿著粗布\n"
		"衣服﹐但卻十足是個美人胚子﹐一雙水汪汪的大眼睛骨碌碌地轉\n"
		"著﹐還不時盯著你看﹐使你不禁覺得有些發窘。\n");

	// set arbao to consume food -dragoon
	set("schedule", ([
	    700: (: timeto_eat :),
	    1200: (: timeto_eat :),
	    1900: (: timeto_eat :),
	]));

	setup();
	carry_money("coin", 30);
	carry_object(__DIR__"obj/blue_cloth")->wear();
	carry_object("/obj/area/obj/dagger")->wield();
}

int hungry = 1;

void timeto_eat()
{
	this_object()->consume_stat("food", 
	    this_object()->query_stat("food")+1);
	return;
}

void init()
{
	::init();
	if( is_fighting() || is_chatting() ) return;
	
	if( this_player()->query_temp("try/fon")==10 ){
		do_chat("阿寶開心的向你搖搖手, 急問道: 見到柳大哥了嗎 ? 他好不好?\n");
		//this_player()->set_temp("try/fon",13);
	}

	// if trying fon, wont join thief
	if( this_player()->query_temp("try/fon") )
		return;

	if( hungry )
		do_chat((: command, "say 喂 ... 我肚子好餓﹐有沒有吃的﹖" :));
	else if( this_player()->query_class() == "thief" ) {
		do_chat(({
			"阿寶望著你，忽然說道：咦？你不是上次給我東西吃的那位 ...\n",
			"阿寶微笑說道：又在這裡遇到你，真巧。\n"
		}));
	}
	else
		do_chat((: command, "say 噓 ... 別讓婆婆知道我在這裡 ..." :));
}

private void give_comb(object player)
{
	object comb;

	if( !player || environment(player)!=environment() ) {
		command("say 咦 ... 人呢﹖");
		return;
	}
	comb = new(__DIR__"obj/comb");
	comb->move(this_object());
	command("give comb to " + player->query("id"));
}

private void exchange(string food_name, object player)
{
	set_stat_current("food", 0);
	command("eat " + food_name);
	if( !player || environment(player) != environment() ) return;

	command("say 對了 ... 不該白吃你的東西﹐這個送給你吧﹗");
}

private void do_follow(object me,object ob)
{
	if( !ob->move(me) )	ob->move(environment(me));
	message_vision("阿寶把一件物品遞給了$N.\n",me);
	command("follow "+me->query("id"));
	me->set_temp("try/fon",43);
}

int accept_object(object me, object ob)
{
	if(me->query_temp("try/fon")!=40){
		if( !hungry || !inherits(F_FOOD, ob) ) {
			do_chat((: command, "say 給我這個幹嘛﹖" :));
			return 0;
		}

		hungry = 0;
		command("smile");
		command("say 啊 .. 太好了﹗真謝謝你﹗ ...");
		do_chat(({ (: exchange, ob->query("id"), me :), (: give_comb, me :) }));
		return 1;
	}
	if(ob->id("__ID_CHALLIE__")){
		do_chat(({
			"阿寶仔細看了看破布.\n",
			"阿寶說道: 這....嗚..我就知道跟柳家無關..嗚...爺爺..\n",
			"阿寶傷心的哭了起來.\n",
			"阿寶哭了好一陣子, 才又對你說道: 謝謝你, 我們這就去見奶奶吧.\n",
			(: do_follow,me,ob :),
		}));
		hungry=0;		// stop talking about another quest by init
		return 1;
	}
}

void relay_say(object ob,string msg)
{
	int tmp = ob->query_temp("try/fon");

	// communicate with gammer
	if( !userp(ob) ){
		if( ob->id("__ID_GAMMER__") ){
			if( strsrch(msg,"不會再去找他尋仇")>= 0 ) {
				do_chat(
(:command("say 真的! 那太好了!"):));
				return;
			}
			if( strsrch(msg,"喜歡上柳淳風")>= 0 ) {
				do_chat(({
"阿寶臉紅了一下.\n",
(:command("say 是."):),
				}));
				return;
			}
			if( strsrch(msg,"幫你爺爺報了仇")>= 0 ) {
				all_inventory(environment())->set_fon_temp(this_player());
				call_out((:do_chat(
"阿寶說道: 奶奶心情不好, 我要去陪陪她, 請你幫忙我去告訴柳大哥這個好消息, 
多謝你了.\n"):),4);
				call_out((:do_chat(
"阿寶說完, 也急急忙忙追著瞎眼老太婆離去.\n"):),5);
				call_out((:destruct(this_object()):),8);
			}
		}
		return;
	}
	// communicate with user
	switch(tmp){
		case 0:
		case 1:
			if( (strsrch(msg, "柳大哥今年中秋會回來") >= 0) ) {
				do_chat(({
"阿寶臉一紅﹐說道：你 .. 你說的是封山派的柳淳風柳大哥嗎﹖\n",
"阿寶說道：這可糟了﹐他這時候回來﹐婆婆定要殺了他的 ... 怎麼辦呢﹖\n",
"阿寶說道: 這位"+ob->rank("respectful")+"可否幫小女子一個忙?\n",
				}));
				ob->set_temp("try/fon",3);
			}
			break;
		case 3:
			if( strsrch(msg,"可")>=0 || strsrch(msg,"好")>=0 ){
				do_chat(({
"阿寶道: 如果你能遇上柳大哥的話, 請你告訴他暫時還是別來雪亭鎮, 不然, 要是他
和婆婆起了衝突,我會很為難的.還有.....\n",
"阿寶紅著臉, 囁聲道: ...請你告訴他阿寶很想他....\n",
				}));
				ob->set_temp("try/fon",5);
			}
			break;
		case 10:
			if( strsrch(msg,"見到")>=0 || strsrch(msg,"好")>=0 ) {
				do_chat(
"阿寶臉上泛起陣陣紅暈, 喃喃唸著柳大哥, 而後問道: 路上有甚麼問題嗎?\n"
				);
				ob->set_temp("try/fon",13);
			}
			break;
		case 13:
			if( strsrch(msg,"死鬼")>=0 ) {
				do_chat(({
"阿寶道: 喔, 柳大哥跟你提了這件事喔, 奶奶最討厭了, 怎麼都講不聽, 人家都不要
理她了.\n",
"阿寶道: 我爺爺叫趙丰, 江湖上的好友稱他丰爺, 一身輕功\ <冷燕飄> 絕步江湖, 奶
奶雖然叫他死鬼, 可是他倆感情很好, 大家都知道呢.\n",
"阿寶突然眼睛紅了起來, 道: 可是三年多前爺爺不知被誰暗算, 死在天邪國外郊...\n",
"阿寶接著道: 人家不知道了, 嗚..  柳大哥年紀比較大, 一定記得我爺爺趙丰的事.\n",
				}));
				ob->set_temp("try/fon",15);
			}
			break;
		default:	break;
	}
}

void set_fon_temp(object me)
{
	if( me->query_temp("try/fon")!=48 )	return;
	me->set_temp("try/fon",50);
}
