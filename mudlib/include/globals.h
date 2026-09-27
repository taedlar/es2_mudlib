// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 1994-2026 Annihilator <taedlar@gmail.com>

#define MUD_NAME "天朝帝國"
#define MUD_NAME_INTERMUD "Celestial Empire"
#define MUD_NAME_ABBR "es2"
#define MUD_HOSTNAME "es2.muds.net"
#define MUD_PORT 4000
#define HTTP_PORT 4015
#define STD_WRAP_WIDTH 76

// Directories

#define BOARD_DIR "/daemon/board/"
#define COMMAND_DIR "/cmds/"
#define CONFIG_DIR "/adm/etc/"
#define DATA_DIR "/data/"
#define HELP_DIR "/docs/help/"
#define LOG_DIR "/log/"

// Daemons

#define ALIAS_D "/adm/daemons/aliasd"
#define CHANNEL_D "/adm/daemons/channeld"
#define CHAR_D "/adm/daemons/chard"
#define CHINESE_D "/adm/daemons/chinesed"
#define COMBAT_D "/adm/daemons/combatd"
#define COMMAND_D "/adm/daemons/cmd_d"
#define DAEMON_D "/adm/daemons/daemond"
#define ENTER_D "/adm/daemons/enterd"
#define EMOTE_D "/adm/daemons/emoted"
#define ENHANCE_D "/adm/daemons/enhanced"
#define FINGER_D "/adm/daemons/fingerd"
#define IDENT_D "/adm/daemons/userid"
#define LOGIN_D "/adm/daemons/logind"
#define NATURE_D "/adm/daemons/natured"
#define PROFILE_D "/adm/daemons/profiled"
#define SECURITY_D "/adm/daemons/securityd"
#define SMTP_D "/adm/daemons/smtpd"
#define VIRTUAL_D "/adm/daemons/virtuald"

#define CLASS_D(x) (DAEMON_D->query_daemon("class:"+(x)))
#define CONDITION_D(x) (DAEMON_D->query_daemon("condition:"+(x)))
#define OBJECT_D(x) (DAEMON_D->query_daemon("domain:"+(x)))
#define RACE_D(x) (DAEMON_D->query_daemon("race:"+(x)))
#define SKILL_D(x) (DAEMON_D->query_daemon("skill:"+(x)))

// Useful macros

#define STOCK_WEAPON(x) ("/obj/area/obj/" + (x))
#define STOCK_ARMOR(x) ("/obj/area/obj/" + (x))
#define STOCK_ITEM(x) ("/obj/area/obj/" + (x))
#define STOCK_MEDICATION(x) ("/obj/medication/" + (x))

// Clonable/Non-inheritable Standard Objects

#define COIN_OB "/obj/money/coin"
#define CORPSE_OB "/obj/corpse"
#define LOGIN_OB "/obj/login"
#define HTTP_OB "/obj/http"
#define MASTER_OB "/adm/obj/master"
#define MAILBOX_OB      "/adm/obj/mailbox"
#define SILVER_OB "/obj/money/silver"
#define SIMUL_EFUN_OB "/adm/obj/simul_efun"
#define USER_OB "/obj/user"
#define VOID_OB "/obj/void"

// Inheritable Standard Objects

#define BULLETIN_BOARD "/std/bboard"
#define CHARACTER "/std/char"
#define COMBINED_ITEM "/std/item/combined"
#define CONTAINER_ITEM "/std/item/container"
#define PHARMACY_ITEM "/std/item/pharmacy"
#define CONDITION "/std/condition"
#define ITEM "/std/item"
#define LIQUID_ITEM "/std/item/liquid"
#define MONEY "/std/money"
#define NPC "/std/char/npc"
#define REAGENT_ITEM "/std/item/reagent"
#define ROOM "/std/room"
#define DOOR_ROOM       "/std/room/doorroom"
#define SKILL "/std/skill"

// User IDs

#define ROOT_UID "Root"
#define BACKBONE_UID "Backbone"
#define DOMAIN_UID "Domain"
#define MUDLIB_UID "Mudlib"

// Features

#define F_ATTRIBUTE "/feature/attribute"
#define F_CLEAN_UP "/feature/clean_up"
#define F_DBASE "/feature/dbase"
#define F_EQUIP "/feature/equip"
#define F_FOOD "/feature/food"
#define F_DRINK "/feature/drink"
#define F_MOVE "/feature/move"
#define F_NAME "/feature/name"
#define F_SAVE "/feature/save"
#define F_STATISTIC "/feature/statistic"
#define F_STUDY "/feature/study"
#define F_TREEMAP "/feature/treemap"
#define F_UNIQUE "/feature/unique"
