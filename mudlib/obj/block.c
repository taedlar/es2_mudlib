inherit ROOM;

void create()
{
	set("short","綠島");
	set("long",@LONG
這裡是 ES2 的「離島」管訓所, 如果你被送到這裡, 表示:
你違反了遊戲規則, 請好好的反省一下你的所作所為, 想離
開的話只要斷線超過 15 分鐘即可 :-) 
LONG
	);
	setup();
}

void init()
{
	if (!wizardp(this_player()) ) 
	add_action("block_cmd","",1);
	call_out("green", 2, this_player());
}

int block_cmd()
{
	if((string)query_verb() == "look") return 0;
	if((string)query_verb() == "help") return 0;
	return 1;
}

void green()
{
	tell_room(environment(this_player()),"你聽到耳邊傳來綠島小夜曲的歌聲: \n\n"+
		"這綠島像一隻船  在月夜裡搖啊搖\n"+
		"姑娘呀妳也在我的心海裡飄啊飄\n"+
		"讓我的歌聲隨那微風  吹開了妳的窗簾\n"+
		"讓我的衷情隨那流水  不斷地向妳傾訴\n"+
		"椰子樹的長影掩不住我的情意\n"+
		"明媚的月光更照亮了我的心\n"+
		"這綠島的夜已經這樣沉靜\n"+
		"姑娘呦～妳為什麼還是默默無語\n\n");
	return;
}
