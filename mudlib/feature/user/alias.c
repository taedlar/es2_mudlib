/*  alias.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#define MAX_ALIASES 40

mapping alias;

string
process_alias(string str)
{
    if( mapp(alias) )
    {
	string cmd, *part;
	int i, n_args;

	if( !undefinedp(alias[str]) )
	    return replace_string( alias[str], "$*", "" );

	i = strsrch(str, ' ');
	if( i < 0 || undefinedp(alias[str[0..i-1]]) )
	    return (string)ALIAS_D->process_global_alias(str);

	part = explode(str, " ") - ({ "" });
	n_args = sizeof(part);

	cmd = replace_string( alias[part[0]], "$*", str[i+1..] );
	for(i=1; i < n_args; i++)
	    cmd = replace_string( cmd, "$" + i, part[i] );

	return cmd;
    }

    return (string)ALIAS_D->process_global_alias(str);
}

int
set_alias(string verb, string replace)
{
    if( geteuid(previous_object()) != ROOT_UID ) return 0;

    if( !replace ) {
	if( mapp(alias) ) map_delete(alias, verb);
	return 1;
    }

    if( !mapp(alias) ) {
	alias = allocate_mapping(MAX_ALIASES);
	alias[verb] = replace;
	return 1;
    }

    if( sizeof(alias) > MAX_ALIASES )
	return notify_fail("您設定的 alias 太多了，請先刪掉一些不常用的。\n");

    alias[verb] = replace;
    return 1;
}

mapping
query_all_alias()
{
    if( geteuid(previous_object()) != ROOT_UID ) return 0;

    return alias;
}

void
set_all_alias(mapping a)
{
    if( geteuid(previous_object()) != ROOT_UID ) return;

    alias = a;
}
