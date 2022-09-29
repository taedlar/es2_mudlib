/*  aliasd.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#pragma save_binary

//inherit F_CLEAN_UP;

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

void create()
{
    seteuid(getuid());
}

string process_global_alias(string arg)
{
    string *word;

    if (arg[0]=='\'') return "say " + arg[1..];

    word = explode(arg, " ");
    if( sizeof(word) && !undefinedp(global_alias[word[0]]) ) {
        word[0] = global_alias[word[0]];
        return implode(word, " ");
    }

    return arg;
}
