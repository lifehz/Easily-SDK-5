﻿/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc date document

	@module	impdate.h | date interface file

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

#ifndef _IMPDATE_H
#define _IMPDATE_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_DATE

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION get_loc_date: get locale datetime.
@OUTPUT xdate_t* pxd: the datetime struct for returning.
@RETURN void: none.
*/
XDL_API void get_loc_date(xdate_t* pxd);

/*
@FUNCTION mak_loc_date: make a locale datetime according to year, month, day, or and hour, minut, second member.
@INOUTPUT xdate_t* pxd: the datetime struct for returning valid datetime, wday will be filled if member is valid.
@RETURN bool_t: if succeeds retur nonzero, fails return zero.
*/
XDL_API bool_t mak_loc_date(xdate_t* pxd);

/*
@FUNCTION get_utc_date: get UTC datetime.
@OUTPUT xdate_t* pxd: the datetime struct for returning.
@RETURN void: none.
*/
XDL_API void get_utc_date(xdate_t* pxd);

/*
@FUNCTION mak_utc_date: make a UTC datetime according to year, month, day, or and hour, minut, second member.
@INOUTPUT xdate_t* pxd: the datetime struct for returning valid datetime, wday will be filled if member is valid.
@RETURN bool_t: if succeeds retur nonzero, fails return zero.
*/
XDL_API bool_t mak_utc_date(xdate_t* pxd);

/*
@FUNCTION get_times: get seconds elapse since 1970-01-01 00:00:00.
@RETURN dword_t: if succeeds retur seconds, fails return zero.
*/
XDL_API dword_t get_times(void);

/*
@FUNCTION get_ticks: get system ticks elapse since 1970-01-01 00:00:00.
@RETURN lword_t: if succeeds retur ticks, fails return zero.
*/
XDL_API dword_t get_ticks(void);

/*
@FUNCTION get_timestamp: get system timestamp.
@RETURN lword_t: if succeeds retur timestamp, fails return zero.
*/
XDL_API lword_t get_timestamp(void);

/*
@FUNCTION utc_date_from_times: convert time in seconds to UTC datetime.
@OUTPUT xdate_t* pxd: datetime struct.
@INPUT dword_t ts: time second value.
@RETURN void: none.
*/
XDL_API void utc_date_from_times(xdate_t* pxd, dword_t ts);

/*
@FUNCTION utc_date_from_ticks: convert ticks to UTC datetime.
@OUTPUT xdate_t* pxd: datetime struct.
@INPUT dword_t ts: tick value.
@RETURN void: none.
*/
XDL_API void utc_date_from_ticks(xdate_t* pxd, dword_t ts);

/*
@FUNCTION utc_date_from_ticks: convert timestamp to UTC datetime.
@OUTPUT xdate_t* pxd: datetime struct.
@INPUT lword_t ts: timestamp value.
@RETURN void: none.
*/
XDL_API void utc_date_from_timestamp(xdate_t* pxd, lword_t ts);

/*
@FUNCTION fill_calendar: fill clendar struct using datetime struct member.
@OUTPUT calendar_t* pca: calendar struct.
@INPUT const xdate_t* pxd: datetime struct.
@RETURN void: none.
*/
XDL_API void fill_calendar(calendar_t* pca, const xdate_t* pdt);

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_DATE*/

#endif /*_IMPDATE_H*/
