/*---
description: 定義 origin() efun 返回的代碼。
author: Annihilator <taedlar@gmail.com>
---*/
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
