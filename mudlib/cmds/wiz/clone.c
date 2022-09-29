/*  clone.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <command.h>
#include "/doc/help.h"

inherit F_CLEAN_UP;

int main(object me, string file)
{
    int amount;
    object obj;
    string err, msg;

    SECURED_WIZARD_COMMAND;

    /* 使用下命令者的權限複製物件 */
    seteuid(geteuid(this_player(1)));

    if (!file) file = me->query("cwf");
    if (!file) return notify_fail("你要複製什麼物件﹖\n");

    if(sscanf(file, "%d %s", amount, file)==2 && amount < 1 )
	return notify_fail("複合物品的數量至少必須是 1。\n");

    file = resolve_path(me->query("cwd"), file);
    if( sscanf(file, "%*s.c") != 1 ) file += ".c";

    if( file_size(file) < 0 )
	return notify_fail("沒有這個檔案(" + file + ")。\n");

    if( !amount ) me->set("cwf", file);
    else me->set("cwf", amount + " " + file);

    if( !find_object(file) ) {
	err = catch(load_object(file));
	if (err) {
	    printf("載入失敗：%O\n", err);
	    return 1;
	}
    }

    err = catch(obj = new(file));
    if( err || !obj ) {
	printf("複製失敗：%O\n", err);
	return 1;
    }

    if( amount ) {
	if( function_exists("set_amount", obj) )
	    obj->set_amount(amount);
	else if( function_exists("set_volume", obj) )
	    obj->set_volume(amount);
	else
	    write("( " + obj->name() + "不是複合物件。 )\n");
    }

    if( !stringp(msg = me->query("env/msg_clone")) )
	msg = "只見$N伸手凌空一指﹐變出了$n。\n";

    // re-set by dragoon
    log_file("static/CLONE", 
	sprintf("[%s] %s(%s) 複製 %s at %s\n",
	ctime(time()), me->name(1), geteuid(me), file, 
	file_name(environment(me)) ) );

    notify_fail("無法複製不能移動的物件(" + file + ")。\n");

    if( !obj->query("no_get")
    && !obj->is_character()
    && !living(obj)
    && obj->move(me) ) {
	write(obj->query("name") + "複製成功\﹐放在你的物品欄。\n");
	message_vision(msg + "\n", me, obj);
	return 1;
    }
    if( obj->move(environment(me)) ) {
	write(obj->query("name") + "複製成功\﹐放在這個房間。\n");
	message_vision(msg + "\n", me, obj);
	return 1;
    }
        

    if( function_exists("set_volume", obj) ) {
	object bottle;
	if( !obj->query("default_container") )
	    bottle = new("/obj/bottle");
	else
	    bottle = new(obj->query("default_container"));
	if( obj->move(bottle) ) {
	    write(obj->query("name") + "複製成功\﹐放個一個" + bottle->name() + "中。\n");
	    bottle->move(me);
	    message_vision(msg + "\n", me, obj);
	    return 1;
	}
	destruct(bottle);
    }

    destruct(obj);
    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : clone <檔名>

利用此指令可複製任何能移動之物件(含怪物)。
HELP
    );
    return 1;
}
