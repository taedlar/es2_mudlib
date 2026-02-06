

varargs void init_damage(int, int, int, int, mixed...);

void
setup_sword(int x, int y, int z, int r)
{
    if( x < 1 || x > 5 )
        error("damage multiplier out of range.\n");
    if( y < 1 || y > 20 )
        error("damage range out of range.\n");
    if( z < 1 || z > 150 )
        error("damage strength bonus out of range.\n");
    if( r < -x || r > 10 )
        error("damage roll out of range.\n");

    init_damage(x, y, z, r, "sword");
    init_damage(x, y/2, z, r, "secondhand sword");
    init_damage(x, y*3/2, z*4/3, r, "twohanded sword");
}

