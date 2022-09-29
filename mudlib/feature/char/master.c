// master.c

#define IDENTITY(ob)	(ob->query("id") + "#" + ob->query("birthday"))

mapping masters = ([]);
mapping apprentices = ([]);

mapping query_masters() { return masters; }
mapping query_apprentices() { return masters; }

int is_master_of(object ob) { return !undefinedp(apprentices[IDENTITY(ob)]); }
int is_apprentice_of(object ob) { return !undefinedp(masters[IDENTITY(ob)]); }

void delete_master(object ob)
{
	map_delete(masters, IDENTITY(ob));
	if( ob->is_master_of(this_object()) )
		ob->delete_apprentice(this_object());
}

void delete_apprentice(object ob)
{
	map_delete(apprentices, IDENTITY(ob));
	if( ob->is_apprentice_of(this_object()) )
		ob->delete_master(this_object());
}

int add_master(object ob)
{
	string key;

	// This key identifies players too.
	key = ob->query("id") + "#" + ob->query("birthday");
	if( !undefinedp(masters[key]) ) return 0;

	masters[key] = ob->short(1);
	ob->add_apprentice(this_object());

	return 1;
}

int add_apprentice(object ob)
{
	string key;

	// This key identifies players too.
	key = ob->query("id") + "#" + ob->query("birthday");
	if( !undefinedp(apprentices[key]) ) return 0;

	apprentices[key] = ob->short(1);
	ob->add_master(this_object());

	return 1;
}

// init_apprentice()
//
// This function adds the argument object to one of the apprentices of
// this_object(). This function is supposed to be overriden to do more
// initialization.
int init_apprentice(object ob) { return add_apprentice(ob); }

// accept_apprentice()
//
// This is a confirm interface for "apprentice" command that should be
// defined to return 1 if you allow a NPC to have this_player() as hir
// apprentice.
// int accept_apprentice(object ob) { return 1; }
