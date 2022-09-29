// magic.c

// This object is used as virtual weapon by spell-skills daemon when hitting
// a character. This object is moved into the inventory of skill daemon and
// passed as from_ob to resist_attack().

inherit ITEM;

void create()
{
	seteuid(getuid());
}
