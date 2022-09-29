/*  emoted.c

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

#include <ansi.h>

inherit F_CLEAN_UP;
inherit F_SAVE;

mapping emote;

//    The emote mapping contains the data base of emote definitions. Each emote
//    is defined in a sub-mapping with the following structure:
//
//    myself    - the message emoter will see
//    target    - the message the target will see when targeted
//    others    - the message others will see when no argument.
//
//    The emote mapping is indexed with the emote 'pattern' instead of emote
//    verb. For example, typing "smile" without argument will cause the daemon
//    search the emote mapping with "smile" as key, and typing "smile someone"
//    will cause the daemon search the emote mapping with "smile $" pattern as
//    the key, while $ means one argument.
//
//    In the message string, the following substitutions will be applied before
//    sending the messages:
//
//    $N        - name of emoter
//    $n        - name of target
//    $P        - pronoun of emoter
//    $p        - pronoun of target
//
//    Original by Annihilator@ESII (11/09/94)

void create()
{
    seteuid(getuid());
    if( !restore() && !mapp(emote) )
        emote = ([]);
}

int remove()
{
    save();
    return 1;
}

string query_save_file() { return DATA_DIR + "emoted"; }

// command_hook() of F_COMMAND call this as an alternate command processor.
// CHANNEL_D also calls this to get emote message.
varargs mixed
do_emote(object me, string verb, string arg, int channel_emote)
{
    string str, my_gender, target_gender, msg_postfix;
    object target;

    if( !environment(me) ) return 0;

    if ( !mapp(emote) || undefinedp(emote[verb]) ) return 0;

    // Determine the pattern to be searched and the target.
    if ( stringp(arg) && arg != "" ) {
        target = present(arg, environment(me));

        if(!objectp(target)) {
            // If not channel emote, only search target in our environment.
            if(!channel_emote) return 0;
            target = find_player(arg);
            if( !objectp(target) || !me->visible(target) || !target->id(arg)) return 0;
        }
        
        if( !target->is_character() )
            return notify_fail("你要對誰做這個動作﹖\n"); 

        target_gender = target->query("gender");
        if( target==me ) {
            msg_postfix = "_self";
            target = 0;
        } else msg_postfix = "_target";
    } else msg_postfix = "";

    my_gender = me->query("gender");

    if( stringp(str = emote[verb]["myself" + msg_postfix]) ) {
        str = replace_string(str, "$N", me->name(channel_emote));
        str = replace_string(str, "$P", gender_self(my_gender));
        if( objectp(target) ) {
            str = replace_string(str, "$n", target->name());
            str = replace_string(str, "$p", gender_pronoun(target_gender));
        }
        if( !channel_emote ) message("emote:vision", CYN + str + NOR, me);
    }

    if( objectp(target) && stringp(str = emote[verb]["target"]) ) {
        str = replace_string(str, "$N", me->name(channel_emote));
        str = replace_string(str, "$P", gender_pronoun(my_gender));
        str = replace_string(str, "$n", target->name());
        str = replace_string(str, "$p", gender_self(target_gender));
        if( !channel_emote ) message("emote:vision", CYN + str + NOR, target);
    }

    if( stringp(str = emote[verb]["others"+msg_postfix]) ) {
        str = replace_string(str, "$N", me->name(channel_emote));
        str = replace_string(str, "$P", gender_pronoun(my_gender));
        if( objectp(target) ) {
            str = replace_string(str, "$n", target->name());
            str = replace_string(str, "$p", gender_pronoun(target_gender));
        }
        if( !channel_emote ) message("emote:vision", CYN + str + NOR, environment(me), ({me, target}));
        else return str;
    }

    if( channel_emote ) return 0;

    // Let NPC know we are doing emote on him.
    if( objectp(target) ) target->relay_emote(me, verb);

    return 1;
}

int set_emote(string pattern, mapping def)
{
    emote[pattern] = def;
    save();
    return 1;
}

int delete_emote(string pattern)
{
    map_delete(emote, pattern);
    save();
    return 1;
}

mapping query_emote(string pattern)
{
    if( !undefinedp(emote[pattern]) ) return emote[pattern];
    else return ([]);
}

string *query_all_emote()
{
    return keys(emote);
}
