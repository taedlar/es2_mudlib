//t_whip.c
#include <weapon.h>
inherit F_WHIP;

void create()
{
	set_name("馬鞭", ({ "horse whip","whip" }));
	set_weight(8000);
        setup_whip(3, 5, 40, 3);
	if( !clonep() ) {
		set("wield_as", ({ "whip" }));
		set("unit", "條");
		set("value", 2000);
		set("long",
"一條尋長的馬鞭, 又黑又長, 編上沾滿泥土砂礫, 鞭上有股難聞的味道.\n"
		);
        }
	setup();
}
