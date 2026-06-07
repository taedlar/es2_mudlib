/*---
description: UTF-8 utilities
author: Annihilator <taedlar@gmail.com>
---*/

varargs string cjk_wrap (string str, int width, int indent, int first_line_indent) {
    string* mbch;
    string result = "";
    int line_length = first_line_indent;
    int prev_char_width = 0;

    if (!width || width < 1)
        return str;
    mbch = explode (str, ""); // [NEOLITH-EXTENSION] explode to array of utf-8 characters
    foreach (string ch in mbch) {
        int char_width = (strlen (ch) > 1) ? 2 : 1; // CJK multi-byte chars are double width
        if (ch == "\n" || ch == "\r") {
            if (prev_char_width > 1)
                continue; // skip newlines immediately following multi-byte characters to avoid breaking them
            ch = " "; // treat newlines as spaces for wrapping purposes
        }
        prev_char_width = char_width;
        if (char_width > 1 || ch == " ") { // break on spaces and multi-byte characters
            if (line_length + char_width > width) {
                result += "\n";
                if (indent > 0)
                    result += repeat_string (" ", indent);
                line_length = indent;
            }
        }
        if (ch == " " && line_length == 0)
            continue; // skip leading spaces
        result += ch;
        if (ch == "\t")
            line_length += 8 - (line_length % 8); // tab stops every 8 characters
        else
            line_length += char_width;
    }

    return result;
}
