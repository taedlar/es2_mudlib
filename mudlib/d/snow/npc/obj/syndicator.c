// syndicator.c

#include <ansi.h>
#include <weapon.h>

inherit F_DAGGER;
inherit F_UNIQUE;

void create()
{
	set_name("「尺娘」", ({ "syndicator" }));
	set_weight(3000);
    setup_dagger(3, 14, 100, 5);

	if( !clonep() ) {
		set("wield_as", ({ "dagger", "secondhand dagger" }));
		set("unit", "把");
		set("value", 32000);
		set("rigidity", 40);
		set("long",
			"這是一把長約四寸的匕首﹐刃上用精細的雕工鏤空出一個美貌\n"
			"女子的臉﹐奇特的是這女子的表情像是伸出舌頭在舔嘴邊的血\n"
			"跡﹐匕首的柄奇短﹐只能用兩隻手指捏住。\n");
	}
	setup();
}

int inflict_damage(string strength, object victim)
{
	int dam = ::inflict_damage(strength, victim);

    if( dam && random(100) < 40 ) {
		environment()->add_temp("fight_msg", HIW "尺娘忽然發出一陣格格的笑聲，" NOR);
        victim->act(15);
	}
	return dam;
}
