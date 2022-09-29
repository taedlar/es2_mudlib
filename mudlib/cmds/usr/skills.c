/*  skills.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <ansi.h>
#include <skill.h>

inherit F_CLEAN_UP;

string *skill_level_desc = ({
    "初學乍練",
    "粗通皮毛",
    "半生不熟",
    "馬馬虎虎",
    "駕輕就熟",
    "出類拔萃",
    "神乎其技",
    "爐火純青",
    "出神入化",
    "登峰造極",
    "深不可測"
});

string *knowledge_level_desc = ({
    "新學乍用",
    "初窺門徑",
    "略知一二",
    "馬馬虎虎",
    "已有小成",
    "得心應手",
    "卓然有成",
    "心領神會",
    "名滿天下",
    "一代宗師",
    "深不可測"
});

string skill_level(string, int);

mapping _skill;

int main(object me, string arg)
{
    object ob;
    mapping skl, lrn, map, flag;
    string *sname, *mapped, s, old_s, line, option;
    int show_all = 0;

    seteuid(getuid());

    ob = me;

    if( arg && arg!="" )
        foreach(option in explode(arg, " "))
            switch(option) {
            case "-a":
                show_all = 1;
                break;
            default:
                ob = present(option, environment(me));
                if (!ob) ob = find_player(option);
                if (!ob) ob = find_living(option);
                if (!ob) return notify_fail("你要察看誰的技能﹖\n");
                if( !wizardp(me)
                &&    !ob->is_apprentice_of(me)
                &&    !me->is_apprentice_of(ob)
                &&    !ob->accept_info(me, "skills") )
                    return notify_fail("只有巫師或有師徒關係的人能察看他人的技能。\n");
            }

    flag = ob->query_skill_flags();
    if( !mapp(flag) ) flag = ([]);

    skl = ob->query_skills();
    if( !mapp(skl) ) skl = ([]);

    lrn = ob->query_learned();
    if( !mapp(lrn) ) lrn = ([]);

    if(!sizeof(skl) && !sizeof(lrn)) {
        write( (ob==me ? "你" : ob->name()) + "目前並沒有學會任何技能。\n");
        return 1;
    }

    line = (ob==me ? "你" : ob->name()) +"目前的技能狀態﹕\n\n";

    // This is a very tricky code that setup a mapping of effective skill
    // level and an array of skill names (>0).
    _skill = ([]);
    sname = filter_array(keys(skl) + keys(lrn),
        (: undefinedp($3[$1]) ? $3[$1] = $2->query_skill($1) : 0 :), ob, _skill );

    map = ob->query_skill_map();
    if( mapp(map) ) mapped = values(map);
    else mapped = ({});

    sname = sort_array( show_all ? keys(_skill) : sname,
        (: _skill[$2] - _skill[$1] :));

    foreach(s in sname )
    {
        if( flag[s] & SKILL_FLAG_ABANDONED ) {
            if( !show_all ) continue;
            line += CYN;
        }

        if( !show_all ) {
            if( !undefinedp(map[s]) ) {
                line += sprintf(HIY "  %-50s" NOR " - %-10s %3d",
                    to_chinese(s) + " - " + to_chinese(map[s]) + " (" + map[s] + ")",
                    skill_level(SKILL_D(s)->type(), skl[s]),
                    _skill[s]);
                if( _skill[s] != skl[s] )
                    line += sprintf( HIW " (%+d)" NOR, _skill[s] - skl[s]);
                line += "\n";
                continue;
            }
            if( member_array(s, mapped) >= 0 ) continue;
        }

        line += sprintf("%s%-50s" NOR " - %-10s %3d", 
            (member_array(s, mapped)==-1? "  ": HIY "ˇ"),
            to_chinese(s) + " (" + s + ")",
            skill_level(SKILL_D(s)->type(), skl[s]),
            _skill[s]
        );
        if( _skill[s] != skl[s] )
            line += sprintf( HIW " (%+d)" NOR, _skill[s] - skl[s]);
        line += "\n";
    }

    this_player()->start_more(line);
    return 1;
}

string skill_level(string type, int level)
{
    int grade;

    grade = sqrt(level/3);

    switch(type) {
        case "knowledge":
            if( grade >= sizeof(knowledge_level_desc) )
                grade = sizeof(knowledge_level_desc)-1;
            return knowledge_level_desc[grade];
        default:
            if( grade >= sizeof(skill_level_desc) )
                grade = sizeof(skill_level_desc)-1;
            return skill_level_desc[grade];
    }
}

int help(object me)
{
    write(@HELP
指令格式 : skills [-a][<某人>]

這個指令可以讓你查詢學過的技能, 亦可查知和你有師徒關係的對象之技能狀況。
skill -a  可以列出所有學過的技能, 無論是否練到 1 重以上。

當你在基本 skill 上有 enable 武術時, 真正有效的是平均後的值, 所以你若看到
內功 (force) ... 145 (-5), 則表示你 (基本內功 + 內功武術)/2 == 145,
而實際的基本內功為 150, 內功武術為 140。 ( 若有小數點進位, 則會差 1 重 )

巫師可以查詢任何人或 NPC 的技能狀況。

HELP
    );
    return 1;
}
