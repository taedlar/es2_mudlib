// bastardsword.c

#include <weapon.h>

inherit F_SWORD;

void create()
{
	set_name("大劍", ({ "bastardsword", "sword" }) );
	set_weight(16000);
	setup_sword(3, 9, 80, 2);

	if( !clonep() ) {
		set("wield_as", ({ "twohanded sword", "sword" }) );
		set("unit", "把");
		set("value", 8500);
		set("rigidity", 25);
		set("long",
			"一把約五尺長的重劍﹐像這麼巨大的重劍多半是背在背後，只能由膂力\n"
			"過人的壯漢使用。\n");
		set("wield_msg", "$N將背後的劍囊一扯，「呼」地一聲抽出一把大劍。\n");
	}
	setup();
}

void varify_template(object owner)
{
	int i = owner->query_level();
	
	ENHANCE_D->enhance_weapon(this_object(), i*i + 10, "劍");
}
