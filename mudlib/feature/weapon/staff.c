// vim: set ts=4 sw=4 syntax=lpc

varargs void init_damage(int, int, int, int, mixed...);

void
setup_staff(int x, int y, int z, int r)
{
	if( x < 1 || x > 3 )
		error("damage multiplier out of range.\n");
	if( y < 1 || y > 25 )
		error("damage range out of range.\n");
	if( z < 1 || z > 250 )
		error("damage strength bonus out of range.\n");
	if( r < -x || r > 10 )
		error("damage roll out of range.\n");

	init_damage(x, y, z, r, "staff");
	init_damage(x, y, z/2, r, "secondhand staff");
	init_damage(x+1, y*3/2, z*3/2, r, "twohanded staff");
}

