/*  more.c - the builtin pager

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

int i, matched, match_line, tmp_line;
string pattern;

void more(string cmd, string *text, int line)
{
    switch(cmd[0]) {
    case '\n':
	break;
    case '\r':
#if 0
	write( "\r" + text[++line]);
	if( line >= sizeof(text)-1 ) { write("\n"); return; }
	printf("\n(More " HIY "%d%%" NOR " / " HIY "%3d" NOR ") :",
		(line*100/sizeof(text)), line );
	break;
#endif
    case ' ':
	if( sizeof(text) - line >= 22 ) write(CLR HOME);
	/* fall through */
    case 'f':
	write( "\r" + implode(text[line..line+22], "\n"));
	line += 23;
	if( line >= sizeof(text)-1 ) { write("\n"); return; }
	printf("\n(More " HIY "%d%%" NOR " / " HIY "%3d" NOR ") :",
		(line*100/sizeof(text)), line );
        break;
    case 'b':
        line = (line >= 46) ? (line - 46) : 0;
	write( "\r" + implode(text[line..line+22], "\n"));
	line += 23;
	printf("\n(More " HIY "%d%%" NOR " / " HIY "%3d" NOR ") :",
		(line*100/sizeof(text)), line );
        break;
    case '/':
        matched=0;
        // update pattern if it was changed, else use last pattern
        if( strlen(cmd)>1 ) {
            if( userp(this_object()) ) {
                if( pattern != cmd[1..strlen(cmd)-1] ) {
                    pattern = cmd[1..strlen(cmd)-1];
                    this_object()->set_temp("more_pattern", pattern);
                    match_line = 0;
                }
            }
            else break;
        }

        if( !pattern ) {
            write("你要找甚麼字串? \n");
            break;
        }

        // if we are in page 1 and it's a new search, start at line -1
        if(line==22 && !match_line)
            line=-1;

        tmp_line=line;
        // start search at next line
        line++;
        // keep on searching until matched
        while( line<sizeof(text) && !matched ) {
            // find pattern in text[line]
            if( strsrch(text[line], pattern) > -1 ) {
                matched = 1;        // boolean
                match_line = line;
                // show the pattern at the last line of screen
                for(line -= 22 ; line <= match_line ; ) {
                    if( line<0 )    line=0;
                    write(text[line] + "\n");
                    if( strlen(text[line]) > 80 )    i--;
                    if( (++line) >= sizeof(text) ) return;
                }
                printf("== More " HIY "%d%%" NOR ", line:" HIY "%3d" NOR ", <" HIW "%s" NOR "> is founded in line: " HIY "%3d" NOR " == >", (line*100/sizeof(text)), line, pattern, match_line+1);
            }
            else if( (++line) >= sizeof(text) )    break;
        }
        // no pattern matched, set line to the starting line of search
        if(!matched) {
            line=tmp_line;
            printf("<" HIW "%s" NOR "> is not founded from line %d to %d == >", pattern, tmp_line+1, sizeof(text) );
        }
        break;
    case 'q':
        write("\n");
        return;
    case '?':
    case 'h':
        write("(SPACE,f,ENTER:下頁  b:上頁  n:下半頁  q:離開) >");
        break;
    default:
        write("\n錯誤指令﹕'" + cmd[0] + "', " + strlen(cmd) + "。(SPACE,f,ENTER:下頁  b:上頁  n:next half page  q:離開) >\n");
    }
//    input_to("more", text, line);
    get_char("more", text, line, 1);
}

void start_more(string msg)
{
    more("f", explode(msg, "\n"), 0);
}
