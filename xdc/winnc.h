﻿/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc window nc document

	@module	winnc.h | window nc interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
***********************************************************************/

/**********************************************************************
This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
LICENSE.GPL3 for more details.
***********************************************************************/

#ifndef _WINWNC_H
#define _WINWNC_H

#include "xdcdef.h"

#ifdef	__cplusplus
extern "C" {
#endif

XDC_API void widget_nc_on_paint(res_win_t wt, res_ctx_t dc);

XDC_API void widget_nc_on_calcsize(res_win_t wt, xrect_t* pxr);

XDC_API int widget_nc_on_hittest(res_win_t wt, const xpoint_t* pxp);

XDC_API long widget_nc_on_calcscroll(res_win_t wt, bool_t horz, const xpoint_t* pxp);

XDC_API void widget_redraw_scroll(res_win_t wt, bool_t horz);

#ifdef	__cplusplus
}
#endif

#endif /*XDLWNC_H*/