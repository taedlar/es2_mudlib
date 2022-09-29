// search.c

inherit F_CLEAN_UP;

int searching(object me, object env);
int find_drug(object me,int drug_num);

int main(object me, string arg)
{
	if(!arg)
		return notify_fail("你仔細四處翻翻找找, 但沒看到啥特別的東西。\n");

	if(arg!="drug" || !wizardp(me))
		return notify_fail("你仔細找了找 " + arg +" ﹐但是啥特別的東西也沒找到。\n");

/*
	if(me->is_fighting() || me->is_busy())
		return notify_fail("你正在忙著, 請試著用 halt 停止所忙的事。\n");

	message_vision("$N露出謹慎的神情, 開始四處翻翻找找。\n", me);
	me->start_busy((:searching, me, environment(me):));
*/
	return 1;
}

/*
int searching(object me, object env)
{
	int sk_me, tmp, searched;
	
	if(!me || !env)	return 0;

	env->add_temp("searched", 10);
	sk_me = me->query_skill("alchemy-medication",1);
	searched = env->query_temp("searched");

	// the larger it is, the better drug will apear
	// 挖過:room searched descrease
	//  max (100 + 0), min(0 -200)
	tmp = (sk_me - searched)%10;

	if(tmp<2)	tmp=2;
	else if(tmp>50)	tmp=50;

	switch(random(tmp)){
	case 2:
		return find_drug(me,2);
	case 1:
		if( random(me->query_score("explorer fame") + 2)<1 ) {
			message_vision("東翻西挖的, $N似乎樂在其中。\n",me);
			return 1;
		}
		return find_drug(me,1);
	case 0:
	default:
		message_vision("東翻西挖的, 除了一陣陣的灰塵, $N什麼都沒找到。\n",me);
		return 0;
	}
}

int find_drug(object me,int num)
{
	object ob;
	ob=new(DRUG_OB +"d_"+num);
	if(!ob){
		tell_object(me, "這種草藥被巫師下了毒, 碰到人便枯\萎了, 請找巫師反應.\n");
		return 0;
	}
	message_vision(ob->query("found_msg")? ob->query("found_msg") : "$N在陰濕角落中, 發現了一" + ob->query("base_unit") + ob->name() + "。\n",me);
	if( ob->move(environment(this_player())) )
		environment(this_player())->add_temp("searched", 100);
	return 0;
}
*/

int help(object me)
{
	write(@HELP
指令格式﹕search [<某物>]

這個指令可以讓你找找你的環境中是否有特殊的東西, 許多的任務、暗房、特殊物品
等可用 search 來穫得一些端倪或尋穫意外的東西。

HELP
	);
	return 1;
}
/*
室內無
找過兩次 searched=200, random( sk_me - searched ) { 0 no, 200 best}
searched 的值要 show 給 ppl ?
*/