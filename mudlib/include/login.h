/*  login.h

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#ifndef LOGIN_H
#define LOGIN_H

#define LOGIN_TIMEOUT	300

#define WELCOME		"/adm/etc/welcome"
#define NEW_PLAYER_INFO	"/adm/etc/new_player"
#define MOTD		"/adm/etc/motd"

#define WIZLIST		"/adm/etc/wizlist"

#define START_ROOM	"/d/snow/inn_hall"
#define DEATH_ROOM	"/obj/void"
#define REVIVE_ROOM	"/obj/void"

#define ONE_GUEST
// This is how much users can 'enter' the mud actually. Maintained
// by LOGIN_D. The max number of connections can be built between
// server(MudOS) and users' client(telnet) is specified in MudOS
// config file, which must be larger than MAX_USER to allow users
// including wizards and admins have chance to connect.
// By Annihilator (02-22-95)
#define MAX_USERS	300

// This defines the minimum wiz_level of users that are allowed to 
// enter the mud. Note players has wiz_level 0.
#define WIZ_LOCK_LEVEL  0

// If this is defined, login daemon will check for banned sites and prevent
// players from login on these sites. Note that wizards are not banned by
// these settings, this only banish players.
#define ENABLE_BAN_SITE

// Define this to enable md5 encrypt of password. This allows user to use
// a password longer than 8 characters and offers slightly better security.
#define ENABLE_MD5_PASSWORD

#define SAVE_USER

// Define this to give penalty to those create character again and again
// to choose good attributes.
#define ENABLE_ANTISPAM

#endif

