/*  action.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <dbase.h>
#include <function.h>

static mixed busy, intr;

varargs void
start_busy(mixed new_busy, mixed new_intr)
{
    if( !new_busy ) return;

    if( !intp(new_busy) && !functionp(new_busy) )
        error("action: Invalid busy action type.\n");

    busy = new_busy;
    if( !intp(new_intr) && !functionp(new_intr) )
        error("action: Invalid busy action interrupt handler type.\n");

    intr = new_intr;
    set_heart_beat(1);
}

nomask mixed query_busy() { return busy; }

nomask int
is_busy()
{
    return busy!=0;
}

// This is called by heart_beat() instead of attack() when a ppl is busy
// doing something else.
static void
continue_action()
{
    if( intp(busy) && (busy > 0) ) {
        busy--;
        return;
    }
    else if( functionp(busy) ) {
        if( functionp(busy) & FP_OWNER_DESTED ) {
            busy = 0;
            intr = 0;
            tell_object(this_object(), "因為某種原因，你正在做的事被停止了。\n");
        }
        else if(    !evaluate(busy, this_object()) ) {
            busy = 0;
            intr = 0;
        }
    }
    else {
        busy = 0;
        intr = 0;
    }
}

void
interrupt(object who, string how)
{
    if( !busy ) return;

    if( intp(busy) && intp(intr) )
    {
	if( busy < intr || how[0]=='*' ) {
	    busy = 0;
	    intr = 0;
	}
    }
    else if( functionp(intr) ) {
	/*  函數型態的 interrupt：
	 *  evaluate 之後，若傳回值 != 0 或者 how 是強制 interrupt，則
	 *  中斷動作。
	 */
	if( evaluate(intr, this_object(), who, how)
	||  how[0]=='*' ) {
	    busy = 0;
	    intr = 0;
	}
    }
}
