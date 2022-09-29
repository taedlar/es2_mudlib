// field.c
//
// This is the virtual room daemon for fieldlands.

mapping map_cache = ([]);

void create()
{
	seteuid(getuid());
	DAEMON_D->register_object_daemon("/d/field");
}

void reset()
{
	string mn;
	mapping m;

	foreach(mn, m in map_cache) {
		// Remove the cached map that is only referenced by map_cache
		// and local variable m.
		if( refs(m) <= 2 )
			map_delete( map_cache, mn );
	}
}

mapping load_map(string map_name)
{
	reset();

	if( file_size(__DIR__"field_map/" + map_name) > 0 )
		map_cache[map_name] = restore_variable( read_file(__DIR__"field_map/" + map_name) );
	else
		map_cache[map_name] = ([
			"terrain" : repeat_string("p", 10000),
		]);

	return map_cache[map_name];
}

object virtual_create(string args)
{
	int x, y, lx, ly;
	string map_name;
	object ob;
	mapping m;

	if( sscanf(args, "X%d,Y%d", x, y) != 2 ) return 0;

	map_name = sprintf("field%02d%02d.map", x/100, y/100);
	if( undefinedp( m = map_cache[map_name] ) )
		m = load_map(map_name);

	lx = x % 100;
	ly = y % 100;

	ob = new(ROOM);
	switch( m["terrain"][ly*100+lx] ) {
	case 'p':
	default:
		ob->set("short", "原野");
		ob->set("long", "你現在正漫步在一處空曠的原野上。\n");
		ob->set("outdoors", "fieldland");
		ob->set("exits/east", sprintf("/d/field:X%d,Y%d", (x+1) % 10000, y) );
		ob->set("exits/west", sprintf("/d/field:X%d,Y%d", (x+9999) % 10000, y) );
		ob->set("exits/north", sprintf("/d/field:X%d,Y%d", x, (y+9999) % 10000) );
		ob->set("exits/south", sprintf("/d/field:X%d,Y%d", x, (y+1) % 10000) );
		// Keep a reference to the map. So we won't clean up the map that is
		// referenced by any room.
		ob->set("virtual_map", m);
		break;
	}
	return ob;
}

string* query_cached_map() { return keys(map_cache); }
