// map.c

inherit ITEM;

void create()
{
	set_name("地圖", ({ "map" }));
	set_weight(10);
	if( !clonep() ) {
		set("unit", "張");
		set("value", 300);
		set("long", "一張可以顯示你附近地形的地圖，用 read map 使用。\n");
	}
	setup();
}

void init()
{
	add_action("do_read", "read");
}

int do_read(string arg)
{
	mapping m;
	string mstr;

	if( !id(arg) ) return 0;
	if( !mapp(m = environment(this_player())->query("virtual_map")) )
		return notify_fail("地圖只有在大區域中才有效。\n");
	mstr = "┌─────────┐\n";
	mstr += repeat_string("│                  │\n", 9);
	mstr += "└─────────┘\n";
	write(mstr);
	return 1;
}
