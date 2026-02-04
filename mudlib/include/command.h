// vim: set syntax=lpc :

#ifndef	COMMAND_H
#define	COMMAND_H

#define ENABLE_BENCHMARK	1

#define ADM_PATH ({"/cmds/adm/", "/cmds/arch/", "/cmds/wiz/", "/cmds/imm/", "/cmds/usr/","/cmds/std/"})
#define ARC_PATH ({"/cmds/arch/", "/cmds/wiz/", "/cmds/imm/", "/cmds/usr/","/cmds/std/"})
#define WIZ_PATH ({"/cmds/imm/", "/cmds/usr/", "/cmds/std/", "/cmds/wiz/"})
#define APR_PATH ({"/cmds/imm/", "/cmds/usr/", "/cmds/std/", "/cmds/wiz/"})
#define IMM_PATH ({"/cmds/imm/", "/cmds/usr/", "/cmds/std/"})
#define PLR_PATH ({"/cmds/std/", "/cmds/usr/"})
#define NPC_PATH ({"/cmds/std/"})

// These are command objects that will also be called in those
// non-player objects.

#define DROP_CMD		"/cmds/std/drop"
#define GET_CMD			"/cmds/std/get"
#define GO_CMD			"/cmds/std/go"
#define KILL_CMD		"/cmds/std/kill"
#define SAY_CMD			"/cmds/std/say"
#define TELL_CMD		"/cmds/std/tell"
#define UPTIME_CMD		"/cmds/usr/uptime"
#define VISITOR_CMD		"/cmds/usr/visitor"
#define WHO_CMD			"/cmds/usr/who"
#define QUIT_CMD		"/cmds/usr/quit"

#define	SECURED_COMMAND	\
	if( (this_player() != this_player(1)) \
	||  (previous_object() != this_player(1)) ) \
	    return notify_fail("你沒有權力使用這個指令。\n")

#define	SECURED_WIZARD_COMMAND	\
	if( (this_player() != this_player(1)) \
	||  (previous_object() != this_player(1)) \
	||  (! wizardp(this_player(1)) ) \
	)   return notify_fail("你沒有權力使用這個指令。\n")

#define	SECURED_ARCH_COMMAND	\
	if( (this_player() != this_player(1)) \
	||  (previous_object() != this_player(1)) \
	||  (wiz_level(this_player(1)) < wiz_level("(arch)")) \
	)   return notify_fail("你沒有權力使用這個指令。\n")

#define	SECURED_ADMIN_COMMAND	\
	if( (this_player() != this_player(1)) \
	||  (previous_object() != this_player(1)) \
	||  (wiz_level(this_player(1)) < wiz_level("(admin)")) \
	)   return notify_fail("你沒有權力使用這個指令。\n")

#endif	/* COMMAND_H */
