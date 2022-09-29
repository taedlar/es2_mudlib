/*  condition.c - character condition interface

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <condition.h>

mapping conditions = ([]);

/* update_condition()
 *
 * 這個函數會被人物的 heart_beat 在每次 tick 時呼叫一次，負責更新人物
 * 所有的長效狀態。
 */

nomask void
update_condition()
{
    string cnd;
    mixed cnd_data;

    if( !mapp(conditions) || !sizeof(conditions) ) return;

    foreach(cnd, cnd_data in conditions)
	CONDITION_D(cnd)->condition_update(this_object(), cnd, cnd_data);
}

/* restore_condition()
 *
 * 這個函數用來在一個人物從檔案取回長效狀態的資料之後，恢復長效狀態的
 * 影響。原則上這個函數應該只有人物自己才會需要呼叫，所以宣告成 static。
 */

static nomask void
restore_condition()
{
    string cnd;
    mixed cnd_data;

    if( !mapp(conditions) || !sizeof(conditions) ) return;

    foreach(cnd, cnd_data in conditions)
	CONDITION_D(cnd)->condition_apply(this_object(), cnd, cnd_data);
}

nomask void
clear_condition()
{
    string cnd;
    mixed cnd_data;

    if( !mapp(conditions) || !sizeof(conditions) ) return;

    foreach(cnd, cnd_data in conditions)
	CONDITION_D(cnd)->condition_unapply(this_object(), cnd);

    conditions = ([]);
}

nomask void
set_condition(string cnd, mixed info)
{
    if( !mapp(conditions) ) conditions = ([]);

    /* 呼叫長效狀態 daemon 解除(unapply) 原來的同一長效狀態 */
    if( conditions[cnd] )
	CONDITION_D(cnd)->condition_unapply(this_object(), cnd,
		conditions[cnd]);

    conditions[cnd] = info;
    CONDITION_D(cnd)->condition_apply(this_object(), cnd, info);
}

nomask mixed
query_condition(string cnd)
{
    return mapp(conditions) ? conditions[cnd] : 0;
}

nomask void
delete_condition(string cnd)
{
    if( !mapp(conditions) || undefinedp(conditions[cnd]) ) return;

    CONDITION_D(cnd)->condition_unapply(this_object(), cnd);
    map_delete(conditions, cnd);
}

 