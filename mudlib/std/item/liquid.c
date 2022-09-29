// liquid.c

inherit ITEM;

static int volume;

int query_volume() { return volume; }

void set_volume(int v)
{
	if( v < 0 ) error("liquid: volume less than zero.\n");
	volume = v;
	if( v==0 ) destruct(this_object());
	else this_object()->set_weight( to_int(v * query("density")) );
}

void add_volume(int v) 
{ 
	if( (volume+v) < 0)  // fix by tears 
	   set_volume(0);
	else 
	   set_volume(volume+v); 
}

string short(int raw)
{
	if( raw ) return ::short(raw);
	if( volume < 500 )
		return "不到一升的" + ::short(raw);
	else
		return "大約" + chinese_number((volume+500)/1000) + "升左右的"
		+ ::short(raw);
}

void dissolve_with(object ob)
{
	// Checking existence of ob is nessessary because we call dissolve_with()
	// in the form:
	// all_environment(environment())->dissolve_with(this_object());
	// after moved, so we might go dissolved by another liquid which has more
	// volume than us before calling another object dissolve_with().

	if(!ob || ob==this_object()) return;

	if(base_name(ob)==base_name(this_object())) {
		ob->add_volume(volume);
		set_volume(0);
		return;
	}

	// This algorithm of inter-dissolve of liquid objects is somewhat rough,
	// may need refine in the future.
	if( ob->query_volume() <= volume ) {
		set("density", (float)(weight() + ob->weight())  / (volume + ob->query_volume()) );
		set_name("不明液體", ({ "unknown liquid", "liquid"}) );
		set("long", "一些不明成份的液體。\n");
		add_volume(ob->query_volume());
		ob->set_volume(0);
	} else {
		ob->set("density", (float)(weight() + ob->weight())  / (volume + ob->query_volume()) );
		ob->set_name("不明液體", ({ "unknown liquid", "liquid"}) );
		ob->set("long", "一些不明成份的液體。\n");
		ob->add_volume(ob->query_volume());
		set_volume(0);
	}
}

varargs int move(mixed dest, int silent)
{
	object env, *inv;
	int i, total;
	string file;

	if( !dest->query("liquid_container") )
		return notify_fail( name() + "必須放在容器裡。\n");

//	write("weight = " + weight() + "\n");
	if( ::move(dest, silent) ) {
		if( (env = environment()) )
			all_inventory(env)->dissolve_with(this_object());
		return 1;
	}
	return 0;
}

int render_value()
{
	return to_int(query("base_value") * volume);
}

void setup()
{
	::setup();
	set("no_get", 1);
	if( !volume ) set_volume(1);
	set("value", (: render_value :));
}

