// Room: /adm/guild/academy.c

inherit ROOM;

void create()
{
	set("short", "巫師學院");
	set("long", @LONG
這裡是巫師們研究 LPC 的地方﹐如果你是新手巫師﹐歡迎來到這裡
和其他人互相討論﹐如果你是老手﹐也請不吝回答新手的問題。
LONG
	);
	set("exits", ([ /* sizeof() == 1 */
		"west" : __DIR__"guildhall",
	]));

	setup();
	load_object("/daemon/board/lpc");
	replace_program(ROOM);
}
