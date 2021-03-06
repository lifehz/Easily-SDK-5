﻿/***********************************************************************
Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc thread document

	@module	impthr.h | thread interface file

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

#ifndef _IMPTHR_H
#define _IMPTHR_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_THREAD

#define THREAD_BASE_TMO		500

#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xthread_begin: create and run a thread. 
@INPUT res_hand_t* ph_hand: used to return thread system resource handle.
@INPUT PF_THREADFUNC pf_worker: the working function will running when thread created.
@INPUT void* param: the paramter send into worker function.
@RETURN void: none.
*/
XDL_API void xthread_begin(res_hand_t* ph_hand, PF_THREADFUNC pf_worker, void* param);

/*
@FUNCTION xthread_end: terminate thread, this functoin must be called at end of the PF_THREADFUNC worker function body.
@RETURN void: none.
*/
XDL_API void xthread_end(void);

/*
@FUNCTION xthread_sleep: let thread sleeping some millisecond, this function may be called in PF_THREADFUNC worker function.
@INPUT int ms: millisecond, it can be negative, if so thread will sleep infinity.
@RETURN void: none.
*/
XDL_API void xthread_sleep(int ms);

/*
@FUNCTION xthread_get_id: get the thread identifier, this function may be called in PF_THREADFUNC worker function.
@RETURN dword_t: identifier.
*/
XDL_API dword_t xthread_get_id(void);

/*
@FUNCTION xthread_join: join the thread into process thread list, then the process will wait for thread terminated befor it exit.
@INPUT res_hand_t th: the thread handle.
@RETURN void: none.
*/
XDL_API void xthread_join(res_hand_t th);

#ifdef XDK_SUPPORT_THREAD_EVENT

/*
@FUNCTION xevent_create: create system event object.
@RETURN res_even_t: if succeeds return system resource handle, fails return NULL.
*/
XDL_API res_even_t xevent_create(void);

/*
@FUNCTION xevent_destroy: destroy system event object.
@INPUT res_even_t eh: event system resource handle.
@RETURN void: none.
*/
XDL_API void xevent_destroy(res_even_t eh);

/*
@FUNCTION xevent_wait: pause thread some time for until event singed, or timeout.
@INPUT res_even_t eh: event system resource handle.
@INPUT int ms: millisecond. it can be negative value, if so, the thread will wait infinity until event signed.
@RETURN wait_t: if succeeds return WAIT_RET, if fails return WAIT_ERR, otherwise timeout with WAIT_TMO returned.
*/
XDL_API wait_t xevent_wait(res_even_t eh, int ms);

/*
@FUNCTION xevent_sign: sign or unsign the event.
@INPUT res_even_t eh: event system resource handle.
@INPUT bool_t b: if nonzero sign the event, if zero unsign the event.
@RETURN void: none.
*/
XDL_API void xevent_sign(res_even_t eh, bool_t b);

#endif

#ifdef XDK_SUPPORT_THREAD_CRITI

/*
@FUNCTION xcriti_create: create critical section system object.
@RETURN res_crit_t: system resource handle.
*/
XDL_API res_crit_t xcriti_create(void);

/*
@FUNCTION xcriti_destroy: destroy critical section system object.
@INPUT res_crit_t cr: the critical section system resource handle.
@RETURN void: none.
*/
XDL_API void xcriti_destroy(res_crit_t cr);

/*
@FUNCTION xcriti_enter: try enter critical section, if other thread occupy the section, then the calling thread must wait.
@INPUT res_crit_t cr: the critical section system resource handle.
@RETURN void: none.
*/
XDL_API void xcriti_enter(res_crit_t cr);

/*
@FUNCTION xcriti_leave: leave critical section.
@INPUT res_crit_t cr: the critical section system resource handle.
@RETURN void: none.
*/
XDL_API void xcriti_leave(res_crit_t cr);

/*
@FUNCTION xcriti_query: query critical section can be entering.
@INPUT res_crit_t cr: the critical section system resource handle.
@RETURN bool_t: if can enter section return nonzero, otherwise return zero.
*/
XDL_API bool_t xcriti_query(res_crit_t cr);

#endif

#ifdef XDK_SUPPORT_THREAD_MUTEX

/*
@FUNCTION xmutex_create: create meutex system object.
@INPUT const tchar_t* mname: the mutex name.
@RETURN res_mutx_t: if succeeds return mutex system resource handle, fails return NULL.
*/
XDL_API res_mutx_t	xmutex_create(const tchar_t* mname);

/*
@FUNCTION xmutex_destroy: destroy meutex system object.
@INPUT const tchar_t* mname: the mutex name.
@INPUT res_mutx_t mx: the mutex system resource handle.
@RETURN void: none.
*/
XDL_API void xmutex_destroy(const tchar_t* mname, res_mutx_t mx);

/*
@FUNCTION xmutex_open: open a meutex created by other process.
@INPUT const tchar_t* mname: the mutex name.
@RETURN res_mutx_t: if the mutex exists then return sytem resource handle, otherwise return NULL.
*/
XDL_API res_mutx_t	xmutex_open(const tchar_t* mname);

/*
@FUNCTION xmutex_lock: occupy a mutex object.
@INPUT res_mutx_t mx: the mutex system resource handle.
@INPUT int ms: millisecond for wait,it can be negative, if so, waiting will be infinity untill mutex object occupied.
@RETURN wait_t: if succeeds return WAIT_RET, if fails return WAIT_ERR, otherwise timeout with WAIT_TMO returned.
*/
XDL_API wait_t xmutex_lock(res_mutx_t mx, int ms);

/*
@FUNCTION xmutex_unlock: release the mutex object.
@INPUT res_mutx_t mx: the mutex system resource handle.
@RETURN void: none.
*/
XDL_API void xmutex_unlock(res_mutx_t mx);

#endif

#ifdef XDK_SUPPORT_THREAD_SEMAP

/*
@FUNCTION xsemap_create: create semaphone system object.
@INPUT const tchar_t* mname: the semaphone name.
@INPUT int max: the max semaphone count.
@RETURN res_sema_t: if succeeds return semaphone system resource handle, fails return NULL.
*/
XDL_API res_sema_t	xsemap_create(const tchar_t* mname, int max);

/*
@FUNCTION xsemap_destroy: destroy semaphone system object.
@INPUT const tchar_t*  mname: the semaphone name.
@INPUT res_sema_t mx: the semaphone system resource handle.
@RETURN void: none.
*/
XDL_API void xsemap_destroy(const tchar_t* mname, res_sema_t mx);

/*
@FUNCTION xsemap_open: open a semaphone created by other process.
@INPUT const tchar_t* mname: the semaphone name.
@RETURN res_sema_t: if the semaphone exists then return sytem resource handle, otherwise return NULL.
*/
XDL_API res_sema_t	xsemap_open(const tchar_t* mname);

/*
@FUNCTION xsemap_lock: the semaphone P operation.
@INPUT res_sema_t mx: the mutex system resource handle.
@INPUT int ms: millisecond for wait,it can be negative, if so, waiting will be infinity until P operation succeed.
@RETURN wait_t: if succeeds return WAIT_RET, if fails return WAIT_ERR, otherwise timeout with WAIT_TMO returned.
*/
XDL_API wait_t xsemap_lock(res_sema_t mx, int milsec);

/*
@FUNCTION xsemap_unlock: the semaphone V operation.
@INPUT res_sema_t mx: the mutex system resource handle.
@RETURN void: none.
*/
XDL_API void xsemap_unlock(res_sema_t mx);

#endif

#ifdef XDK_SUPPORT_THREAD_QUEUE
/*
@FUNCTION xqueue_create: create asychronous io queue.
@INPUT res_queue_t ep: the master queue system handle.
@INPUT res_file_t fd: the io system handle.
@INPUT int max: the max threads can monitor the queue. 
@RETURN res_queue_t: if succeeds return queue system resource handle, fails return NULL.
*/
XDL_API res_queue_t xqueue_create(res_queue_t ep, res_file_t fd, int max);

/*
@FUNCTION xqueue_destroy: destroy queue system object.
@INPUT res_queue_t mx: the queue system resource handle.
@RETURN void: none.
*/
XDL_API void xqueue_destroy(res_queue_t ep);

/*
@FUNCTION xqueue_wait: wait io coming.
@INPUT res_queue_t ep: the queue system resource handle.
@INPUT int ms: millisecond for wait,it can be negative, if so, waiting will be infinity until io coming.
@RETURN wait_t: if succeeds return WAIT_RET, if fails return WAIT_ERR, otherwise timeout with WAIT_TMO returned.
*/
XDL_API wait_t xqueue_wait(res_queue_t ep, int ms);
#endif

#ifdef	__cplusplus
}
#endif

#endif /*XDK_SUPPORT_THREAD*/

#endif /*IMPTHR_H*/