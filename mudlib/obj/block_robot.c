//block_robot.c	the npc in this room will ask player some question
//and log some data to file: robot_check

inherit ROOM;

void create()
{
	set("short","機器人島");
	set("long","\n
\t這裡是 ES2 的機器人島, 如果你被送到這裡, 
\t那表示你的行為類似為機器人 (help robot).
\t若你已醒了, 請回答機器人小精靈的問話, 或找巫師反應.\n\n\n");
	set("no_fight", 1);
	set("objects", ([
		__DIR__"spirit": 1,
	]));
	setup();
}

void init()
{
	if(!wizardp(this_player()))
		add_action("do_quit", "quit");
}

int do_quit()
{
	write("抱歉, 你現在不能 quit , 請回答機器人小精靈的問話或找巫師反應.\n");
	return 1;
}
