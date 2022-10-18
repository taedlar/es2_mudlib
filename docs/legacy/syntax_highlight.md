LPC Syntax Highlight
====================

ES2 Mudlib uses vim's modeline to support LPC syntax hightlight.

You can download the syntax highlight settings for vim from [LPC Syntax Highlight](https://www.vim.org/scripts/script.php?script_id=761).
This download page also gives you instructions about how to install the settings.

Remember to enable modeline for vim in your .vimrc:
```
set modeline
```

In ES2's LPC programs, you may see the first line containing:
```
// vim: syntax=lpc
```

This line tells vim to syntax highlight the file with LPC syntax instead of regular C language.
