/*  call.c

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

#define USAGE	"指令格式﹕call <物件>-><函數>( <參數>, ... )\n"

inherit F_CLEAN_UP;

int
main(object me, string arg)
{
    string objname, func, param, euid;
    object obj;
    mixed *args, result;
    int i, cost;

    SECURED_WIZARD_COMMAND;

    if( !arg || arg=="" ) return notify_fail(USAGE);

    if( sscanf(arg, "-%s %s", euid, arg)==2 ) {
	/* 只有 (admin) 能設定自己的 euid 為任意值，包括 ROOT_EUID */
	switch(euid)
	{
	case DOMAIN_UID:
	case MUDLIB_UID:
	    if( wiz_level(me) < wiz_level("(arch)") )
		return notify_fail("你的權限不夠。\n");
	    seteuid(euid);
	    break;
	default:
	    if( wiz_level(me) < wiz_level("(admin)") )
		return notify_fail("你的權限不夠。\n");
	    seteuid(euid);
	    break;
        }
    }
    else
	seteuid(geteuid(me));

    if( sscanf(arg, "%s->%s(%s)", objname, func, param)!=3 )
	return notify_fail(USAGE);

    obj = present(objname, environment(me));
    if(!obj) obj = present(objname, me);
    if(!obj) obj = find_player(objname);
    if(!obj || !obj->visible(me))
	obj = find_object(resolve_path(me->query("cwd"), objname));

    if(objname=="me") obj = me;
    if(objname=="my_link") obj = me->link();
    if(objname=="here") obj = environment(me);

    if(!obj) return notify_fail("找不到指定的物件。\n");

    if( userp(obj) ){
        if( obj != me ) log_file("static/CALL_PLAYER",
            sprintf("%s(%s) call %s(%s)->%s(%s) on %s\n",
                me->name(1), geteuid(me), obj->name(1), geteuid(obj), func, param,
                ctime(time()) ) );
    }

    if( !function_exists(func, obj) ) {
	write( sprintf("%O並沒有定義 %s 這個函數。\n", obj, func));
	return 1;
    }

    args = explode(param, ",");
    for(i=0; i<sizeof(args); i++)
    {
        object ob_arg;

        // This removes preceeding blanks and trailing blanks.
        //parse_command(args[i], environment(me), "%s", args[i]);
        //if( sscanf(args[i], "%d", args[i]) ) continue;
        //if( sscanf(args[i], "\"%s\"", args[i]) ) continue;
        if( args[i]=="me") { args[i] = me; continue; }
        if( (ob_arg = present(args[i], me))
        ||  (ob_arg = present(args[i], environment(me))) )
	    { args[i] = ob_arg; continue; }
	args[i] = restore_variable(args[i]);
    }

    args = ({ func }) + args;

    cost = eval_cost();
    result = call_other(obj, args);
    cost -= eval_cost();;
    for(i=1; i<sizeof(args); i++)
        args[i] = sprintf("%O",args[i]);
    printf("%O->%s(%s) = %O\n執行指令數﹕%d\n", obj, func, 
        implode(args[1..sizeof(args)-1], ", "), result, cost);
    return 1;
}

int help(object me)
{
write(@HELP
指令格式 : call <物件>-><函數>(<參數>, ...... )
 
呼叫<物件>裡的<函數>並傳入相關<參數>.
 
HELP
    );
    return 1;
}
