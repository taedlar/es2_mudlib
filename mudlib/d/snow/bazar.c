// ROOM: /d/snow/bazar.c
#include <room.h>
inherit BANK;

void create()
{
	set("short", "魚記小鋪");
	set("long", @LONG
這裡是一間小店鋪, 右側擺了個長架, 不甚起眼的幾件日常用品零
零疏疏地擺在架上, 你看到了有瓶罐, 棉被, 碗盤竹筷等雜物, 不過上
面隱隱有些灰塵黃土, 似乎這兒老闆不常清掃. 正廳有個大圓桌, 上面
擺了些布匹服飾. 左側有個高大的櫃臺, 上面橫掛了個匾額  (look 匾
額 ), 看來像是個小錢莊.
LONG
	);
	set("detail", ([ /* sizeof() == 2 */
		"匾額" :
			"『	小	魚	吃	四	海\n"
			"	金	銀	文	都	來	』\n\n"
			"存款﹕	deposit  <數量> <錢幣>,	開戶﹕	open account\n"
			"提款﹕	withdraw <數量> <錢幣>,	兌換﹕	convert <數量> <錢幣> to <另類錢幣>\n"
			"金契請妥善保管﹐如有遺失﹐至本莊補發﹐需扣一成保證金。\n",
		"櫃臺" : "一座漆黑紅杉所鋸雕而成, 散發出一種濃郁的杉木味, 看來價值不斐.\n",
	]));
	set("exits", ([ /* sizeof() == 1 */
		"west" : __DIR__"sstreet1",
	]));
	set("objects", ([
		__DIR__"npc/yu": 1,
	]));

	setup();
	replace_program(BANK);
}
