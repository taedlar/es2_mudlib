// dust.c

inherit COMBINED_ITEM;

void create()
{
	set_name("化屍粉", ({"dust"}));
	set("long",
		"這是一包用來毀屍滅跡的化屍粉﹐只要灑一丁點就可以\n"
		"化去(dissolve)一具屍體﹗\n");
	set("unit", "包");
	set("base_unit", "份");
	set("base_weight", 1);
	set_amount(1);
}

void init()
{
	if( this_player()==environment() )
		add_action("do_dissolve", "dissolve");
}

int do_dissolve(string arg)
{
	object ob;

	if( !arg ) return notify_fail("你要用化屍粉溶解什麼東西﹖\n");
	if( !objectp(ob = present(arg, environment(this_player()))) )
		return notify_fail("這裡沒有這樣東西。\n");
	if( living(ob) || !ob->id("corpse") )
		return notify_fail("化屍粉只能用來溶解屍體。\n");
	message_vision(
		"$N用指甲挑了一點化屍粉在$n上﹐只聽見一陣「嗤嗤」聲響帶著\n"
		"一股可怕的惡臭﹐$n只剩下一灘黃水。\n", this_player(), ob);
	destruct(ob);
	add_amount(-1);
	return 1;
}

