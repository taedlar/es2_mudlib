/*---
description: Online "help" command
author: Annihilator <taedlar@gmail.com>
---*/

#include <help.h>

inherit F_CLEAN_UP;

private string *player_doc_paths = PLAYER_HELP_DOC_PATHS;
private string* wizard_doc_paths = PLAYER_HELP_DOC_PATHS + WIZ_HELP_DOC_PATHS;

int main (object me, string arg) {
    string file, *search;

    seteuid (getuid());
    if (!arg) {
        string topics = HELP_DIR + "topics.txt";
        if (file_size (topics) < 0)
            return notify_fail ("沒有說明文件的主題列表。\n");
        me->start_more (read_file (topics));
        return 1;
    }

    // try if a command name is specified.
    arg = replace_string (arg, " ", "_");
    if (stringp (file = me->find_command (arg))) {
        notify_fail ("有這個指令存在﹐但是並沒有詳細的說明文件。\n");
        return file->help (me);
    }

    // support efun/lfun help with same name as other topics such as
    // ed() and ed command.
    sscanf (arg, "%s()", arg);

    search = wizardp(me) ? wizard_doc_paths : player_doc_paths;
    foreach (string path in search) {
        string file_path = path + arg + ".txt";
        if (file_size (file_path) < 0)
            continue;
        if (wizardp (me))
            write ("說明文件：" + file_path
                + "\n---------------------------------------------------------------------\n");
        me->start_more (read_file (file_path)); 
        return 1;
    }

    // allow wizard to specify a path in the HELP_DIR, e.g. "help wiz/alias" to read "/docs/help/wiz/alias.txt".
    if (wizardp (me)) {
        string help_file = HELP_DIR + arg + ".txt";
        if (file_size (help_file) < 0)
            return notify_fail ("沒有針對這項主題的說明文件。\n");
        write ("說明文件：" + help_file
            + "\n---------------------------------------------------------------------\n");
        me->start_more (read_file (help_file));
        return 1;
    }

    return notify_fail ("沒有針對這項主題的說明文件。\n");
}

int help (object me) {
    write (@HELP
指令格式﹕help <主題>

這個指令提供你針對某一主題的詳細說明文件﹐若是不指定主題﹐則提供你有關主題
的文件﹐所有的主題與函數名稱皆為英文。
HELP
    );
    return 1;
}

