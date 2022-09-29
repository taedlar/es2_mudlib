// army.c

inherit F_NAME;
inherit F_MOVE;
inherit F_DBASE;
inherit F_ATTRIBUTE;
inherit F_STATISTIC;

void setup()
{
	seteuid(getuid());
	if( clonep() ) {
//		set_heart_beat();
		enable_commands();
		if( !query_stat("surviver") )
			set_stat_maximum("surviver", 1);
	}
}

string short()
{
	int nmem;

	nmem = query_stat("surviver");

	return sprintf("%s [ 兵力%s人 ]",
		::short(),
		( nmem > 10 ) ? ("約" + chinese_number(nmem - nmem%10) + "多") : ("共" + chinese_number(nmem)) );
}
