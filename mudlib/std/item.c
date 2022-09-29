// item.c

inherit F_CLEAN_UP;
inherit F_DBASE;
inherit F_MOVE;
inherit F_NAME;

void setup()
{
	seteuid(getuid());
	if( clonep() && !query_default_object())
		set_default_object(base_name(this_object()));
	else {
		if( !query("unit") ) set("unit", "個");
	}
}
