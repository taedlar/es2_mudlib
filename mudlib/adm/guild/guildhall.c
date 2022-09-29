// Room: /adm/guild/guildhall.c

inherit ROOM;

void create()
{
	set("short", "巫師公會");
	set("long", @LONG
這裡就是巫師公會了﹐一個半球型的大浮島漂浮在七彩的雲間﹐平
坦的地面鋪著一層碧綠草皮﹐草地盡頭是一棟純白的大型建築﹐希臘式
的圓柱跟波斯風格的圓頂﹐營造出一種莊嚴的氣氛﹐往北是開會用的會
議廳﹐東邊是研究 LPC 的巫師學院﹐往西則是通往世界各地的傳送站。
LONG
	);
	set("exits", ([ /* sizeof() == 2 */
		"east" : __DIR__"academy",
		"north" : __DIR__"meetingroom",
]));

	setup();
	load_object("/daemon/board/wizard");
	replace_program(ROOM);
}
