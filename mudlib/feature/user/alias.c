/*---
description: Process user defined aliases.
author: Annihilator <taedlar@gmail.com>
---*/

#define MAX_ALIASES 40

mapping alias;

string process_alias (string str) {
    if (mapp (alias)) {
        string verb = query_verb();
        if (!undefinedp (alias[str]))
            return replace_string (alias[str], "$*", ""); // short cut for simple alias

        if (!undefinedp (alias[verb])) {
            string* parts = explode (str, " ") - ({ "" });
            int n_args = sizeof (parts);

            string cmd = replace_string (alias[verb], "$*", str[verb.len() + 1 ..]);
            for (int i = 1; i < n_args; i++)
                cmd = replace_string (cmd, "$" + i, parts[i]);

            return cmd;
        }
    }

    return (string)ALIAS_D->process_global_alias(str);
}

int set_alias (string verb, string replace) {
    if (geteuid(previous_object()) != ROOT_UID)
        return 0;

    if (!replace) {
        if (mapp(alias))
            map_delete (alias, verb);
        return 1;
    }

    if (!mapp(alias))
        alias = allocate_mapping (MAX_ALIASES);

    if (sizeof(alias) > MAX_ALIASES)
        return notify_fail ("您設定的 alias 太多了，請先刪掉一些不常用的。\n");

    alias[verb] = replace;
    return 1;
}

mapping query_all_alias() {
    if (geteuid(previous_object()) != ROOT_UID)
        return 0;
    return alias;
}

void set_all_alias(mapping a) {
    if (geteuid(previous_object()) != ROOT_UID)
        return;
    alias = a;
}

