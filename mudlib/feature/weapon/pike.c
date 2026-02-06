

varargs void init_damage(int, int, int, int, mixed...);

void
setup_pike(int x, int y, int z, int r)
{
	if( x < 1 || x > 5 )
		error("damage multiplier out of range.\n");
	if( y < 1 || y > 20 )
		error("damage range out of range.\n");
	if( z < 1 || z > 100 )
		error("damage strength bonus out of range.\n");
	if( r < -x || r > 20 )
		error("damage roll out of range.\n");

	init_damage(x, y, z, r, "pike");
	init_damage(x, y, z/2, r, "secondhand pike");
	init_damage(x+1, y*4/3, z, r, "twohanded pike");
}

