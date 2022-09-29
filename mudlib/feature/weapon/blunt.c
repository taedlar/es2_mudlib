/*  blunt.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

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
