// weapon.h

#ifndef __WEAPON_H__
#define __WEAPON_H__

//#pragma no_warnings

#define F_SWORD		"/feature/weapon/sword.c"
#define F_BLADE		"/feature/weapon/blade.c"
#define F_BLUNT		"/feature/weapon/blunt.c"
#define F_AXE		"/feature/weapon/axe.c"
#define F_DAGGER	"/feature/weapon/dagger.c"
#define F_NEEDLE     "/feature/weapon/needle.c"
#define F_STAFF		"/feature/weapon/staff.c"
#define F_PIKE		"/feature/weapon/pike.c"
#define F_WHIP		"/feature/weapon/whip.c"

#ifndef __ARMOR_H__
inherit ITEM;
inherit F_ATTRIBUTE;
inherit F_EQUIP;
inherit F_STATISTIC;
#endif

#endif
