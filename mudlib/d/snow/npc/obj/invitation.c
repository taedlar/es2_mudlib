// invitation.c

inherit ITEM;

void create()
{
	set_name("英雄帖", ({ "invitation" }) );
	set_weight(20);
	if( clonep() )
		set_default_object(__FILE__);
	else {
		set("unit", "張");
		set("long",
			"一張由武林盟主於蘭兼毅所發的英雄帖﹐只有一流身分地位的武林\n"
			"人物才能收到這張英雄帖。\n");
		set("value", 1);
	}
}
