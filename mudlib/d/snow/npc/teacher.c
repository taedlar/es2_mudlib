// teacher.c

#include <npc.h>

inherit F_VILLAGER;

string *literate_text = ({
    "人之初", "性本善",
    "性相近", "習相遠",
    "苟不教", "性乃遷",
    "教之道", "貴以專",
    "昔孟母", "擇鄰處",
    "子不學", "斷機杼",
    "竇燕山", "有義方",
    "教五子", "名俱揚",
    "養不教", "父之過",
    "教不嚴", "師之惰",
    "為人子", "方少時",
    "親師友", "習禮儀",
});

int text_cnt = 0;
object picked_student;

void teach_literate();
void pick_student(object ob);
void cancel_pick(object ob);

void create()
{
    set_name("王懷芝", ({ "wang whaiz", "wang", "whaiz" }) );
    set_race("human");
    set_level(5);
    set_stat_maximum("sen", 50);

    set_skill("literate", 60);
    set("long",
        "王懷芝是鎮上王員外的獨子﹐前幾年上京考試落第﹐碰巧王太夫人過世\n"
        "﹐因此留在家鄉服喪﹐閒暇無事﹐便開了間私塾教鎮上孩子讀書。\n");
    set("chat_chance", 8);
    set("chat_msg", ({
        (: teach_literate :)
    }));
    setup();
}

void init()
{
    add_action("do_sit", "sit");
}

int do_sit(string arg)
{
    // add by dragoon
    if( this_player()->query_skill("literate") > 29 ) 
	return notify_fail("你的讀書識字根基不錯，不需要再聽三字經了。\n");
	
    if( this_player()->query_temp("pending/sit_learning_literate") )
        return notify_fail("你已經坐著專心聽講了。\n");
    message_vision("$N找了一個空位坐了下來﹐開始跟著王老師唸書。\n",
        this_player() );
    this_player()->set_temp("pending/sit_learning_literate", 1);
    return 1;
}

void teach_literate()
{
    object *students, ob;
    string txt;

    if( text_cnt >= sizeof(literate_text) ) {
        command("say 嗯 ... 一次教太多你們也記不住﹐好吧﹐我們再從頭複習一次。\n");
        text_cnt = 0;
    }
    txt = literate_text[text_cnt++];
    command("say 大家跟著我念﹕" + txt + "。\n");

    students = all_inventory(environment());
    foreach(ob in students)
    {
	if( !ob->query_temp("pending/sit_learning_literate")
	||  !living(ob)
	||  ob->is_fighting()
	||  ob->query_skill("literate",1) >= 30 ) continue;

	ob->improve_skill("literate", random(ob->query_attr("int")+1));
	message_vision("$N跟著念道﹕" + txt + "。\n", ob);

	/* 挑選書生的條件 */
	// 加個random, 不然每講一句就問 -Dragoon
	if( interactive(ob) && random(10) > 7 
	&&  ob->query_class()=="commoner"
	&&  ob->query_skill("literate") >= 5
	&&  member_array(ob->query_race(), ({
		"human", "woochan", "jiaojao", "rainnar"
	    })) != -1
	&&  ob->query_attr("int") > random(20) )
	    do_chat( (: pick_student, ob :) );
    }
}

int accept_fight()
{
    do_chat("王懷芝說道﹕這 ...武功\我可是半點不會﹐這位朋友不如去找我們家王教頭切磋切磋﹖\n");
    return 0;
}

string rank(string politeness, int raw)
{
    if( raw ) return ::rank(politeness, raw);
    switch(politeness) {
        case "self": return "小生";
        case "respectful": return "王老師";
        default:
            return ::rank(politeness, raw);
    }
}

int accept_info(object ob, string type)
{
    return type=="skills";
}

int acquire_skill(object who, string skill)
{
    if( skill=="literate" ) {
	command("say 想讀書識字嗎﹖在這裡坐著(sit)聽吧。\n");
	command("smile");
	command("say 讀書可不容易﹐不過學了多少一輩子都是你的。\n");
	return 1;
    }
    return 0;
}

void pick_student(object ob)
{
    if( ! (ob && present(ob) && interactive(ob)) ) return;

    command("say " + ob->name() + "，你在我這兒唸書有一段時間了。\n");
    command("say 我看你對讀書頗有興趣，天份也不錯。\n");
    command("say 想不想和老師一樣，當一個書生？\n");

    ob->set_temp("pending/picked_by_whaiz", 1);
    call_out( (: cancel_pick, ob :), 20);
}

void cancel_pick(object ob)
{
    if( ! ob ) return;

    ob->delete_temp("pending/picked_by_whaiz");
    if( present(ob) ) {
	command("pat " + ob->query("id"));
	command("say 也罷，人各有志。");
    }
}

void make_new_scholar(object ob)
{
    if( !ob || !present(ob) || !interactive(ob)) return;

    ob->set_class("scholar");
    command("smile");
    tell_object(ob, "( 你成為一位書生 )\n");
}

void relay_say(object who, string msg)
{
    if( ! who->query_temp("pending/picked_by_whaiz") ) return;
    if( member_array(msg[0..1], ({"想", "是", "好"})) != -1 ) {
	do_chat((: make_new_scholar, who :));
	who->delete_temp("pending/picked_by_whaiz");
    }
}
