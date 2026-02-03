/*  chinese.c - some routines for Chinese language support

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

string
chinese_number(int i)
{
    return CHINESE_D->chinese_number(i);
}

string
chinese_period(int i)
{
    return CHINESE_D->chinese_period(i);
}

string
to_chinese(string str)
{
    return CHINESE_D->chinese(str);
}

string break_chinese_string (string str, int len, mixed indent) {
    string* wch;
    string indent_str, out, line;
    int cntr, w;

    if (!stringp (str))
        return 0;

    if (intp (indent))
        indent_str = sprintf ("%*s", indent, " ");
    else if (stringp (indent))
        indent_str = indent;
    else
        indent_str = "";

    cntr = 0;

    // strip all newlines.
    str = replace_string (str, "\n", "");

    // [NEOLITH] break up the string into array of unicode characters.
    wch = explode (str, "");
    out = "";
    line = indent_str;
    w = strlen(indent_str);
    foreach (string ch in wch) {
        line += ch;
        if (strlen(ch) > 1) {
            w += 2; // double width for multi-byte Chinese chars
        }
        else if (ch == "\t") {
            w += 8 - w % 8; // set width to next tab stop
        }
        else {
            w += 1;
            if (ch != " " && ch != "\t")
                continue; // only break on spaces for single byte chars
        }
        if (w >= len) {
            out += line + "\n";
            line = indent_str;
            w = strlen(indent_str);
        }
    }
    return out + line;
}

string
currency_string(int v)
{
    if( v >= 100 ) {
        return chinese_number(v/100) + "兩銀子"
            + ((v%100) ? "又" + chinese_number(v%100) + "文錢" : "");
    }
    return chinese_number(v) + "文錢";
}

