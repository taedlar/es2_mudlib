inherit BULLETIN_BOARD;

void create()
{
	set_name("XX板", ({ "X board" }) );
	set("long", "有個 XX 板");
	set("location", "/d/snow/X");
	set("board_id", "X");		//若檔名為 abc.c 則 X 填 abc
	set("capacity", 30);		//板面大小, 平常區域 30 
	setup();
	replace_program(BULLETIN_BOARD);
}
//寫好後此檔會搬到 /daemon/board 下放
//記得在此 location room 中 setup() 下要寫 load_object("/daemon/board/X");
