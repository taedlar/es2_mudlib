

varargs void init_damage(int, int, int, int, mixed...);

void
setup_axe(int x, int y, int z, int r)
{
	if( x < 1 || x > 5 )
		error("damage multiplier out of range.\n");
	if( y < 1 || y > 25 )
		error("damage range out of range.\n");
	if( z < 1 || z > 200 )
		error("damage strength bonus out of range.\n");
	if( r < -x || r > 10 )
		error("damage roll out of range.\n");

	init_damage(x, y, z, r, "axe");
	init_damage(x, y/2, z, r, "secondhand axe");
	init_damage(x, y*2, z*4/3, r, "twohanded axe");
}

