/*  globals.h

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#define MUD_NAME		"天朝帝國"
#define MUD_NAME_INTERMUD	"Celestial Empire"
#define MUD_NAME_ABBR		"es2"
#define MUD_HOSTNAME		"es2.muds.net"
#define MUD_PORT		4000
#define HTTP_PORT		4015

// Directories

#define BOARD_DIR	"/daemon/board/"
#define COMMAND_DIR	"/cmds/"
#define CONFIG_DIR	"/adm/etc/"
#define DATA_DIR	"/data/"
#define HELP_DIR	"/doc/"
#define LOG_DIR		"/log/"

// Daemons

#define ALIAS_D		"/adm/daemons/aliasd.c"
#define CHANNEL_D	"/adm/daemons/channeld.c"
#define CHAR_D		"/adm/daemons/chard.c"
#define CHINESE_D	"/adm/daemons/chinesed.c"
#define COMBAT_D	"/adm/daemons/combatd.c"
#define COMMAND_D	"/adm/daemons/cmd_d.c"
#define DAEMON_D	"/adm/daemons/daemond.c"
#define ENTER_D		"/adm/daemons/enterd.c"
#define EMOTE_D		"/adm/daemons/emoted.c"
#define ENHANCE_D	"/adm/daemons/enhanced.c"
#define FINGER_D	"/adm/daemons/fingerd.c"
#define IDENT_D		"/adm/daemons/userid.c"
#define LOGIN_D		"/adm/daemons/logind.c"
#define NATURE_D	"/adm/daemons/natured.c"
#define PROFILE_D	"/adm/daemons/profiled.c"
#define SECURITY_D	"/adm/daemons/securityd.c"
#define SMTP_D		"/adm/daemons/smtpd.c"
#define VIRTUAL_D	"/adm/daemons/virtuald.c"

#define CLASS_D(x)	(DAEMON_D->query_daemon("class:"+(x)))
#define CONDITION_D(x)	(DAEMON_D->query_daemon("condition:"+(x)))
#define OBJECT_D(x)	(DAEMON_D->query_daemon("domain:"+(x)))
#define RACE_D(x)	(DAEMON_D->query_daemon("race:"+(x)))
#define SKILL_D(x)	(DAEMON_D->query_daemon("skill:"+(x)))

// Useful macros

#define STOCK_WEAPON(x)		("/obj/area/obj/" + (x))
#define STOCK_ARMOR(x)		("/obj/area/obj/" + (x))
#define STOCK_ITEM(x)		("/obj/area/obj/" + (x))
#define STOCK_MEDICATION(x)	("/obj/medication/" + (x))

// Clonable/Non-inheritable Standard Objects

#define COIN_OB		"/obj/money/coin"
#define CORPSE_OB	"/obj/corpse"
#define LOGIN_OB	"/obj/login"
#define HTTP_OB		"/obj/http"
#define MASTER_OB	"/adm/obj/master"
#define MAILBOX_OB      "/adm/obj/mailbox"
#define SILVER_OB	"/obj/money/silver"
#define SIMUL_EFUN_OB	"/adm/obj/simul_efun"
#define USER_OB		"/obj/user"
#define VOID_OB		"/obj/void"

// Inheritable Standard Objects

#define BULLETIN_BOARD	"/std/bboard"
#define CHARACTER	"/std/char"
#define COMBINED_ITEM	"/std/item/combined"
#define CONTAINER_ITEM	"/std/item/container"
#define PHARMACY_ITEM	"/std/item/pharmacy"
#define CONDITION	"/std/condition"
#define ITEM		"/std/item"
#define LIQUID_ITEM	"/std/item/liquid"
#define MONEY		"/std/money"
#define NPC		"/std/char/npc"
#define REAGENT_ITEM	"/std/item/reagent"
#define ROOM		"/std/room"
#define DOOR_ROOM       "/std/room/doorroom"
#define SKILL		"/std/skill"

// User IDs

#define ROOT_UID	"Root"
#define BACKBONE_UID	"Backbone"
#define DOMAIN_UID	"Domain"
#define MUDLIB_UID	"Mudlib"

// Features

#define F_ATTRIBUTE	"/feature/attribute.c"
#define F_CLEAN_UP	"/feature/clean_up.c"
#define F_DBASE		"/feature/dbase.c"
#define F_EQUIP		"/feature/equip.c"
#define F_FOOD		"/feature/food.c"
#define F_DRINK		"/feature/drink.c"
#define F_MOVE		"/feature/move.c"
#define F_NAME		"/feature/name.c"
#define F_SAVE		"/feature/save.c"
#define F_STATISTIC	"/feature/statistic.c"
#define F_STUDY		"/feature/study.c"
#define F_TREEMAP	"/feature/treemap.c"
#define F_UNIQUE	"/feature/unique.c"

