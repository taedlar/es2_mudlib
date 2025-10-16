#ifndef ANSI_ESCAPE_H
#define ANSI_ESCAPE_H

/* ANSI - The American National Standards Institute
 * ANSI escape sequences for terminal colors, screen clearing, cursor positioning, etc.
 * See http://en.wikipedia.org/wiki/ANSI_escape_code
 */
 
#define CSI     "\x1b["         /* Control Sequence Introducer */

#define HOME    CSI "H"         /* (CUP n=1; m=1) Move cursor left-top of scren */
#define CLR     CSI "J"         /* (ED n=2) Clear from cursor to end of screen */
#define CLREOL  CSI "K"         /* (EL n=0) Clear from cursor to end of line */

#define SAVEC   CSI "s"         /* Save cursor */
#define RESTC   CSI "u"         /* Restore cursor */

/* Select Graphic Rendition (CSI n m) */

#define NOR     CSI "0m"        /* All attributes become turned off */
#define BOLD    CSI "1m"        /* Bold mode */
#define FAINT   CSI "2m"        /* Faint mode */
#define ITALIC  CSI "3m"        /* Italic mode */
#define UNDL    CSI "4m"        /* Underline mode */
#define BLINK   CSI "5m"        /* Blink mode */

/* Foreground colors */
 
#define BLK     CSI "30m"       /* Black    */
#define RED     CSI "31m"       /* Red      */
#define GRN     CSI "32m"       /* Green    */
#define YEL     CSI "33m"       /* Yellow   */
#define BLU     CSI "34m"       /* Blue     */
#define MAG     CSI "35m"       /* Magenta  */
#define CYN     CSI "36m"       /* Cyan     */
#define WHT     CSI "37m"       /* White    */
#define SFC     CSI "38;5;%dm"  /* Special foreground color from 0-255 */
 
/* Background colors */
 
#define BBLK    CSI "40m"       /* Black    */
#define BRED    CSI "41m"       /* Red      */
#define BGRN    CSI "42m"       /* Green    */
#define BYEL    CSI "43m"       /* Yellow   */
#define BBLU    CSI "44m"       /* Blue     */
#define BMAG    CSI "45m"       /* Magenta  */
#define BCYN    CSI "46m"       /* Cyan     */
#define BWHT    CSI "47m"       /* White    */
#define SBC     CSI "48;5;%dm"  /* Special background color from 0-255.  */

/* Bold foreground colors */

#define HIK     CSI "1;30m"     /* Black    */
#define HIR     CSI "1;31m"     /* Red      */
#define HIG     CSI "1;32m"     /* Green    */
#define HIY     CSI "1;33m"     /* Yellow   */
#define HIB     CSI "1;34m"     /* Blue     */
#define HIM     CSI "1;35m"     /* Magenta  */
#define HIC     CSI "1;36m"     /* Cyan     */
#define HIW     CSI "1;37m"     /* White    */

/* Bold background colors */

#define HBBLK   CSI "1;40m"     /* Black    */
#define HBRED   CSI "1;41m"     /* Red      */
#define HBGRN   CSI "1;42m"     /* Green    */
#define HBYEL   CSI "1;43m"     /* Yellow   */
#define HBBLU   CSI "1;44m"     /* Blue     */
#define HBMAG   CSI "1;45m"     /* Magenta  */
#define HBCYN   CSI "1;46m"     /* Cyan     */
#define HBWHT   CSI "1;47m"     /* White    */

/* VT100 */

#define DECSC   "\x1b7"         /* Saves the cursor position, encoding shift state and formatting attributes. */
#define DECRC   "\x1b8"         /* Restores states saved by a previous DECSC, or reset all to their defaults. */

#endif  /* ANSI_ESCAPE_H */
