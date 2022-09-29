// reagent.c

inherit ITEM;

string weight_string();
int reagent_value();

void setup()
{
	set("value", (: reagent_value :) );
	if( !clonep() ) {
		set("measurable", 1);
	}
	::setup();
}

int reagent_value()
{
	return to_int( (float)query("unit_value") * weight() );
}

string short(int raw)
{
	string s = ::short(raw);
	int w = weight();
	if( raw ) return s;
	if( query("measured_flag") )
		s += " <" + weight_string() + ">";
	else {
		if( w < 100 ) s += " <少許\>";
		else if( w < 250 ) s += " <若干>";
		else if( w < 500 ) s += " <不到半斤>";
		else if( w < 1000 ) s += " <不到一斤>";
		else s += " <約" + chinese_number((w-500)/1000 + 1) + "斤>";
	}
	return "藥材: " + s;
}

string weight_string()
{
	int w = weight();
	string s = "";

	if( w%10 ) s = chinese_number(w%10) + "分" + s;
	w /= 10;
	if( w%10 ) s = chinese_number(w%10) + "錢" + s;
	w /= 10;
	if( w%10 ) s = chinese_number(w%10) + "兩" + s;
	w /= 10;
	if( w > 0 ) s = chinese_number(w%10) + "斤" + s;
	return s;
}
