/*  edit.c - text editor

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/


#include <function.h>

int edit(function callback, string text)
{
    write("結束離開用 '.'﹐取消輸入用 '~q'﹐使用內建列編輯器用 '~e'。\n");
    write("─────────────────────────────\n");
    if( (functionp(callback) & FP_NOT_BINDABLE)==0 )
	callback = bind(callback, this_object());
    
    if( text )      // 增加 text 這個變數, for board's followup
    {
        write( text );
        input_to( "input_line", text, callback );
        return 1;
    }
    input_to("input_line", "", callback );
    return 1;
}

private void input_line(string line, string text, function callback)
{
    if( line=="." ) {
        if( functionp(callback) & FP_OWNER_DESTED ) 
            write("啟動編輯模式的物件已經消失了﹐輸入取消。\n");
        else
            evaluate(callback, text);
        return;
    } else if( line=="~q" ) {
        write("輸入取消。\n");
        return;
    } else if( line=="~e" ) {
    } else
        text += line + "\n";
    input_to("input_line", text, callback);
}
