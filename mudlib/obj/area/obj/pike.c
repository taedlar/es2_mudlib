// pike.c

#include <weapon.h>

inherit F_PIKE;

void create()
{
	set_name("長槍", ({ "long pike", "pike"}));
	set_weight(11000);
	setup_pike(2,11,60,1);

	if( !clonep() ) {
		set("wield_as", ({ "twohanded pike", "pike" }));
		set("unit", "桿");
		set("value", 6000);
		set("rigidity", 25);
		set("long",
"一把槍頭用精鋼鑄造而成的長槍，槍身筆直，槍頭槍身銜接的地方綁著一條紅巾，膂
力大的人常常使用的武器。\n"
		);
		set("wield_msg",
		"$N用雙手從身後抽出一桿長槍握在手上。\n"
		);
	}
	setup();
}

void varify_template(object owner)
{
	int i = owner->query_level();
	
		ENHANCE_D->enhance_weapon(this_object(), i*i + 10, "槍");
}
