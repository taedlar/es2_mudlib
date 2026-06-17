/*---
description: text pager, used to display long text in pages.
author: Annihilator <taedlar@gmail.com>
---*/
#include <ansi.h>

private int i, matched, match_line, tmp_line;
private string pattern;

private int justify_next_line (int line, int total, int page_size) {
    if (line < 0)
        line = 0; // first page
    else if (line > total - page_size)
        line = total - page_size; // last page
    return line;
}

/* [NEOLITH-EXTENSION]
 *
 * The `input_prompt` apply will recognize the pager by callback function name "more"
 * and write the pager prompt to the user.
 */
void more (string cmd, string *text, int line) {
    int page_size = this_object()->query("page_size") || 22;
    switch (cmd) {
    case "q": // quit pager
        write ("\r" CLR "\n");
        return;
    case "g": case KEY_HOME: // first page
        more (" ", text, 0);
        return;
    case "G": case KEY_END: // last page
        more (" ", text, justify_next_line (sizeof(text) - page_size, sizeof(text), page_size));
        return;
    case "k": case KEY_UP: // one line backward
        more (" ", text, justify_next_line (line - page_size - 1, sizeof(text), page_size));
        return;
    case "b": case KEY_PAGE_UP: // one page backward
        more (" ", text, justify_next_line (line - 2*page_size, sizeof(text), page_size));
        return;
    case "j": case "\n": case "\r": case KEY_DOWN: // one line forward
        more (" ", text, justify_next_line (line - page_size + 1, sizeof(text), page_size));
        return;
    case " ": case KEY_PAGE_DOWN: // render and forward one page, re-arm pager
        line = justify_next_line (line, sizeof(text), page_size);
        write (HOME CLR + implode (text[line .. line + page_size - 1], "\n"));
        line += page_size;
        break;
    default:
        write (CUU(1));
        break;
    }
    get_char ("more", 1, text, line);
}

/*
 * Start the pager with the given text.
 */
varargs void start_more (mixed msg, int start_line) {
    if (stringp (msg))
        msg = explode (msg, "\n");
    else if (!arrayp (msg))
        error ("Invalid argument to start_more: expected string or array of strings.");
    more (" ", msg, start_line ? start_line : 0);
}

void start_more_if_needed (mixed msg) {
    if (stringp (msg))
        msg = explode (msg, "\n");
    else if (!arrayp (msg))
        error ("Invalid argument to start_more_if_needed: expected string or array of strings.");
    int page_size = this_object()->query("page_size") || 22;
    if (sizeof (msg) > page_size)
        start_more (msg);
    else
        write (implode (msg, "\n") + "\n");
}
