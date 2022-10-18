// miller.c

#include <npc.h>

inherit F_VILLAGER;

void create()
{
	set_name("楊大嬸", ({ "miller" }));
	set_race("human");
	set_level(2);

	set("gender", "female");
	set("long",
		"楊大嬸是前幾年才搬到這個地方的﹐據說他的丈夫是朝中的高官﹐但是\n"
		"得了功\名富貴之後卻拋棄妻兒﹐但是楊大嬸自己從不承認﹐只說丈夫在\n"
		"京城經商。\n");
	setup();
	carry_object("/obj/area/obj/cloth")->wear();
}
