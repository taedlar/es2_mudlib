#include <room.h>
inherit ROOM;

void create()
{
	set("short", "小廣場");
	set("long", @LONG
這裏是個小廣場，幾棵樹稀稀落落的長在附近，中央有個小型的
留言版，可供民眾回報所遭遇的怪異現象，往南可回到人聲鼎沸的雪
亭鎮廣場。
LONG
	);
	set("exits", ([ /* sizeof() == 3 */
		"south" : "/d/snow/square_w",
	]));
	set("outdoors", "snow");
	set("no_fight", 1);			// prevent pk when post

	setup();
	load_object("/daemon/board/bug_board");
}
