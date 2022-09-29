// npc.h

#ifndef __NPC_H__
#define __NPC_H__

// Reaction control modules

#define F_BANDIT		"/feature/npc/bandit.c"
#define F_FIGHTER		"/feature/npc/fighter.c"
#define F_SOLDIER		"/feature/npc/soldier.c"
#define F_SCHOLAR		"/feature/npc/scholar.c"
#define F_VILLAGER		"/feature/npc/villager.c"

// Other functional modules

#define F_TRAVELER		"/feature/npc/traveler.c"
#define F_VENDOR		"/feature/npc/vendor.c"

inherit "/std/char/npc.c";

#endif
