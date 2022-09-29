/*  ansi.h

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#ifndef	ANSI_ESCAPE_H
#define ANSI_ESCAPE_H
 
#define ESC	"\x1B"
 
#define NOR ESC "[0;2m"		/* Normal white forground */
 
/* Foreground colors */
 
#define BLK	ESC "[30m"	/* Black    */
#define RED	ESC "[31m"	/* Red      */
#define GRN	ESC "[32m"	/* Green    */
#define YEL	ESC "[33m"	/* Yellow   */
#define BLU	ESC "[34m"	/* Blue     */
#define MAG	ESC "[35m"	/* Magenta  */
#define CYN	ESC "[36m"	/* Cyan     */
#define WHT	ESC "[37m"	/* White    */
 
/* Background colors */
 
#define BBLK	ESC "[40m"	/* Black    */
#define BRED	ESC "[41m"	/* Red      */
#define BGRN	ESC "[42m"	/* Green    */
#define BYEL	ESC "[43m"	/* Yellow   */
#define BBLU	ESC "[44m"	/* Blue     */
#define BMAG	ESC "[45m"	/* Magenta  */
#define BCYN	ESC "[46m"	/* Cyan     */
#define BWHT	ESC "[47m"	/* White    */

/* Bold foreground colors */

#define HIK	ESC "[1;30m"	/* Black    */
#define HIR	ESC "[1;31m"	/* Red      */
#define HIG	ESC "[1;32m"	/* Green    */
#define HIY	ESC "[1;33m"	/* Yellow   */
#define HIB	ESC "[1;34m"	/* Blue     */
#define HIM	ESC "[1;35m"	/* Magenta  */
#define HIC	ESC "[1;36m"	/* Cyan     */
#define HIW	ESC "[1;37m"	/* White    */

/* Bold background colors */

#define HBBLK	ESC "[1;40m"	/* Black    */
#define HBRED	ESC "[1;41m"	/* Red      */
#define HBGRN	ESC "[1;42m"	/* Green    */
#define HBYEL	ESC "[1;43m"	/* Yellow   */
#define HBBLU	ESC "[1;44m"	/* Blue     */
#define HBMAG	ESC "[1;45m"	/* Magenta  */
#define HBCYN	ESC "[1;46m"	/* Cyan     */
#define HBWHT	ESC "[1;47m"	/* White    */
 
#define SAVEC	ESC "[s"	/* Save cursor */
#define RESTC	ESC "[u"	/* Restore cursor */
#define SAVECA	ESC "7"		/* Save cursor & attribute */
#define RESTCA	ESC "8"		/* Restore cursor & attribute */

#define CLR	ESC "[2J"	/* Clear entire screen  */
#define HOME	ESC "[H"	/* Move cursor left-top of scren */

#define BOLD	ESC "[1m"	/* Bold mode */
#define REF	CLR HOME	/* Clear screen, move cursor to left-top */
 
#endif	/* ANSI_ESCAPE_H */
