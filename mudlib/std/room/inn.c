// inn.c

#include <daemon.h>

inherit ROOM;

void setup()
{
	::setup();
	set("valid_startroom", 1);
	set("no_fight", 1);

	TRAVELER_D->register_travel_site();
}
