/*  NAME - DESCRIPTION

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#ifndef	ORIGIN_H
#define ORIGIN_H

/* codes returned by the origin() efun */
#define ORIGIN_BACKEND		"driver"	/* backwards compat */
#define ORIGIN_DRIVER		"driver"
#define ORIGIN_LOCAL		"local"
#define ORIGIN_CALL_OTHER	"call_other"
#define ORIGIN_SIMUL_EFUN	"simul"
#define ORIGIN_CALL_OUT		"call_out"
#define ORIGIN_EFUN		"efun"
/* pseudo frames for call_other function pointers and efun pointer */
#define ORIGIN_FUNCTION_POINTER	"function_pointer"
/* anonymous functions */
#define ORIGIN_FUNCTIONAL	"functional"

#endif
