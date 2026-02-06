

varargs void init_damage(int, int, int, int, mixed...);

void
setup_blunt(int x, int y, int z, int r)
{
	if( x < 1 || x > 6 )
		error("damage multiplier out of range.\n");
	if( y < 1 || y > 15 )
		error("damage range out of range.\n");
	if( z < 1 || z > 300 )
		error("damage strength bonus out of range.\n");
	if( r < -x || r > 10 )
		error("damage roll out of range.\n");

	init_damage(x, y, z, r, "blunt");
	init_damage(x, y, z*2/3, r, "secondhand blunt");
	init_damage(x+1, y, z*2, r, "twohanded blunt");
}

