// traveler.c

#include <attack.h>
#include <daemon.h>
#include <move.h>

int do_travel()
{	
	string destination;

	if( is_fighting() ) return 0;

	destination = TRAVELER_D->request_travel_site();
	if( !destination ) return 0;

	return move(destination);
}

void reset()
{
	do_travel();
}
