// combined_item.c

inherit ITEM;

// variables

int amount;

// implementations

int query_amount() { return amount; }

private void destruct_me() { destruct(this_object()); }

void set_amount(int v)
{
    if( v < 0 ) error("combine:set_amount less than 1.\n");
    amount = v;

    if( v==0 )	call_out("destruct_me", 0);
    else	this_object()->set_weight((int)(v * (float)query("base_weight")));
}

void add_amount(int v) { set_amount(amount+v); }

varargs string short(int raw)
{
    if( raw ) return ::short(raw);
    return chinese_number(query_amount()) + query("base_unit")
	+ ::short(raw);
}

varargs int move(mixed dest, int silent)
{
    if( ! ::move(dest, silent) ) return 0;

    if( environment()->is_character() )
    {
	object ob;
	string file = base_name(this_object());

	foreach(ob in all_inventory(environment()))
	{
	    if( ob==this_object() ) continue;
	    if( base_name(ob)==file ) {
		amount += (int)ob->query_amount();
		destruct(ob);
	    }
	}
	set_amount(amount);
    }

    return 1;
}

int render_value()
{
    return to_int(query("base_value") * amount);
}

void setup()
{
    ::setup();
    if( !amount ) set_amount(1);
    set("value", (: render_value :));
}

