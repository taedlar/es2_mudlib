/*---
description: Process global aliases.
author: Annihilator <taedlar@gmail.com>
---*/

#pragma save_binary

mapping global_alias = ([
    "l":	"look",
    "n":	"go north",
    "e":	"go east",
    "w":	"go west",
    "s":	"go south",
    "u":	"go up",
    "d":	"go down",
    "nu":	"go northup",
    "eu":	"go eastup",
    "wu":	"go westup",
    "su":	"go southup",
    "nd":	"go northdown",
    "ed":	"go eastdown",
    "wd":	"go westdown",
    "sd":	"go southdown",
    "ne":	"go northeast",
    "se":	"go southeast",
    "nw":	"go northwest",
    "sw":	"go southwest",
    "i":	"inventory",
    "tt":	"team talk",
    "sc":	"score",
    "k":	"kill"
]);

void create() {
    // Do not set EUID since the alias daemon never load objects and does not need to write files.
    // seteuid(getuid());
}

/*
 * Process global aliases.
 * The alias daemon only process global aliases, and the user defined aliases are processed
 * in the user object. This provides a newbie-friendly initial set of aliases and also allows
 * veteran players to override the global aliases with their own ones.
 *
 * NOTE: This is called for EVERY commands from EVERY user, we only do simple string replacement
 * here without argument parsing. The user defined aliases are processed in the user object,
 * which allows more complex alias definitions with argument parsing.
 */
string process_global_alias (string cmd) {
    string verb = query_verb();

    if (cmd[0]=='\'')
        return "say " + cmd[1..];

    if (!undefinedp(global_alias[verb]))
        return global_alias[verb] + cmd[verb.len()..];

    return cmd;
}

