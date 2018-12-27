﻿/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc image editor document

	@module	photoctrl.c | image editor widnows implement file

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

#include "xdcctrl.h"
#include "handler.h"
#include "winnc.h"
#include "xdcfire.h"
#include "xdcbox.h"

#define BITMAP_BYTES(buf)		GET_DWORD(buf, 2)
#define BITMAP_WIDTH(buf)		GET_DWORD(BITMAPINFOHEADERPTR(buf), 4)
#define BITMAP_HEIGHT(buf)		GET_DWORD(BITMAPINFOHEADERPTR(buf), 8)


typedef struct _photo_delta_t{
	long org_x, org_y;
	int index;

	res_bmp_t bmp;

	res_win_t editor;

	bool_t b_drag;
	bool_t b_size;
	bool_t b_lock;

	link_t_ptr anno;
	link_t_ptr spot;

	link_t_ptr stack;
}photo_delta_t;


#define GETPHOTODELTA(ph) 	(photo_delta_t*)widget_get_user_delta(ph)
#define SETPHOTODELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

int noti_photo_owner(res_win_t widget, unsigned long code, link_t_ptr spot, void* data, res_ctx_t rdc);
/*******************************************************************************************************/
static void _photoctrl_done(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	byte_t* buf;
	dword_t len;

	XDL_ASSERT(ptd && ptd->anno);

#ifdef _UNICODE
	len = format_dom_doc_to_bytes(ptd->anno, NULL, MAX_LONG, DEF_UCS);
#else
	len = format_dom_doc_to_bytes(ptd->anno, NULL, MAX_LONG, DEF_MBS);
#endif

	buf = (byte_t*)xmem_alloc(len + sizeof(tchar_t));

#ifdef _UNICODE
	format_dom_doc_to_bytes(ptd->anno, buf, len, DEF_UCS);
#else
	format_dom_doc_to_bytes(ptd->anno, buf, len, DEF_MBS);
#endif

	push_stack_node(ptd->stack, (void*)buf);
}

static void _photoctrl_undo(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	void* p;
	int len;

	XDL_ASSERT(ptd && ptd->anno);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		clear_anno_doc(ptd->anno);

		len = xslen((tchar_t*)p);

#ifdef _UNICODE
		parse_dom_doc_from_bytes(ptd->anno, (byte_t*)p, len * sizeof(tchar_t), DEF_UCS);
#else
		parse_dom_doc_from_bytes(ptd->anno, (byte_t*)p, len * sizeof(tchar_t), DEF_MBS);
#endif

		xmem_free(p);

		photoctrl_redraw(widget);
	}
}

static void _photoctrl_discard(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		xmem_free(p);
	}
}

static void _photoctrl_clean(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	while (p = pop_stack_node(ptd->stack))
	{
		xmem_free(p);
	}
}

static bool_t _photoctrl_copy(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	res_glob_t gb;
	dword_t len;
	byte_t* buf;
	link_t_ptr dom, nlk, ilk;

	XDL_ASSERT(ptd && ptd->anno);

	if (!get_anno_spot_selected_count(ptd->anno))
		return 0;

	if (!clipboard_open())
		return 0;

	dom = create_anno_doc();
	ilk = get_anno_next_spot(ptd->anno, LINK_FIRST);
	while (ilk)
	{
		if (get_anno_spot_selected(ilk))
		{
			nlk = insert_anno_spot(dom, LINK_LAST);
			copy_dom_node(nlk, ilk);
		}

		ilk = get_anno_next_spot(ptd->anno, ilk);
	}

#ifdef _UNICODE
	len = format_dom_doc_to_bytes(dom, NULL, MAX_LONG, DEF_UCS);
#else
	len = format_dom_doc_to_bytes(dom, NULL, MAX_LONG, DEF_MBS);
#endif

	gb = system_alloc(len + sizeof(tchar_t));
	buf = (byte_t*)system_lock(gb);

#ifdef _UNICODE
	format_dom_doc_to_bytes(dom, buf, len, DEF_UCS);
#else
	format_dom_doc_to_bytes(dom, buf, len, DEF_MBS);
#endif

	system_unlock(gb);

	if (!clipboard_put(DEF_CB_FORMAT, gb))
	{
		system_free(gb);
		clipboard_close();

		destroy_anno_doc(dom);

		return 0;
	}

	clipboard_close();

	destroy_anno_doc(dom);
	return 1;
}

static bool_t _photoctrl_cut(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	link_t_ptr nxt, ilk;

	XDL_ASSERT(ptd && ptd->anno);

	if (!_photoctrl_copy(widget))
		return 0;

	ilk = get_anno_next_spot(ptd->anno, LINK_FIRST);
	while (ilk)
	{
		nxt = get_anno_next_spot(ptd->anno, ilk);

		if (get_anno_spot_selected(ilk))
		{
			delete_anno_spot(ilk);
		}
		ilk = nxt;
	}

	photoctrl_redraw(widget);

	return 1;
}

static bool_t _photoctrl_paste(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk;

	XDL_ASSERT(ptd && ptd->anno);

	if (!clipboard_open())
		return 0;

	gb = clipboard_get(DEF_CB_FORMAT);
	if (!gb)
	{
		clipboard_close();
		return 0;
	}

	len = system_size(gb);
	buf = (byte_t*)system_lock(gb);

	dom = create_dom_doc();
#ifdef _UNICODE
	if (!parse_dom_doc_from_bytes(dom, buf, len, DEF_UCS))
#else
	if (!parse_dom_doc_from_bytes(dom, buf, len, DEF_MBS))
#endif
	{
		system_unlock(gb);
		clipboard_close();

		destroy_dom_doc(dom);
		return 0;
	}

	if (!is_anno_doc(dom))
	{
		system_unlock(gb);
		clipboard_close();

		destroy_dom_doc(dom);
		return 0;
	}

	while (nlk = get_anno_next_spot(dom, LINK_FIRST))
	{
		nlk = detach_dom_node(get_anno_spotset(dom), nlk);
		attach_dom_node(get_anno_spotset(ptd->anno), LINK_LAST, nlk);
	}

	system_unlock(gb);
	clipboard_clean();
	clipboard_close();

	destroy_dom_doc(dom);
	return 1;
}

static void _photoctrl_spot_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_anno_spot_rect(&cb, ptd->anno, ilk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static res_bmp_t _photoctrl_merge_anno(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	res_ctx_t rdc, memdc;
	res_bmp_t org, membm;
	xcolor_t xc = { 0 };
	xbrush_t xb = { 0 };
	xrect_t xr = { 0 };
	xsize_t xs;
	link_t_ptr ilk;

	if (!ptd->bmp && !ptd->anno)
		return NULL;

	rdc = widget_client_ctx(widget);

	get_bitmap_size(ptd->bmp, &xs.cx, &xs.cy);
	membm = create_compatible_bitmap(rdc, xs.cx, xs.cy);

	memdc = create_compatible_context(rdc);
	org = (res_bmp_t)select_compatible_bitmap(memdc, membm);

	xr.x = 0;
	xr.h = 0;
	xr.w = xs.cx;
	xr.h = xs.cy;

	widget_get_xbrush(widget, &xb);
	draw_rect_raw(memdc, NULL, &xb, &xr);

	draw_bitmap_raw(memdc, ptd->bmp, &xr);

	ilk = get_anno_next_spot(ptd->anno, LINK_FIRST);
	while (ilk)
	{
		_photoctrl_spot_rect(widget, ilk, &xr);

		//noti_photo_owner(widget, NC_PHOTOANNODRAW, ilk, (void*)&xr, memdc);
	}

	membm = (res_bmp_t)select_compatible_bitmap(memdc, org);

	destroy_context(memdc);

	widget_release_ctx(widget, rdc);

	return membm;
}

static void _photoctrl_reset_page(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	long pw, ph, fw, fh;
	xrect_t xr;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	if (ptd->bmp)
	{

		get_bitmap_size(ptd->bmp, &fw, &fh);
	}
	else
	{
		fw = pw;
		fh = ph;
	}

	widget_reset_paging(widget, pw, ph, fw, fh, 20, 20);

	widget_reset_scroll(widget, 1);

	widget_reset_scroll(widget, 0);
}
/*******************************************************************************************************/
int noti_photo_owner(res_win_t widget, unsigned long code, link_t_ptr spot, void* data, res_ctx_t rdc)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	NOTICE_PHOTO nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;

	nf.anno = ptd->anno;
	nf.spot = spot;

	nf.data = data;
	nf.rdc = rdc;
	
	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

void noti_photo_spot_drag(res_win_t widget, long x, long y)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->spot);

	widget_set_cursor(widget, CURSOR_HAND);

	pt.x = x;
	pt.y = y;
	noti_photo_owner(widget, NC_PHOTOANNODRAG, ptd->spot, (void*)&pt, NULL);
}

void noti_photo_spot_drop(res_win_t widget, long x, long y)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	xpoint_t pt;
	xrect_t xr,xr_org;
	xsize_t xs;
	xpoint_t* ppt;
	int i,count;

	XDL_ASSERT(ptd->spot);

	ptd->b_drag = 0;

	widget_set_cursor(widget, CURSOR_ARROW);

	xs.cx = x - ptd->org_x;
	xs.cy = y - ptd->org_y;

	if (!xs.cx && !xs.cy)
		return;

	_photoctrl_done(widget);

	_photoctrl_spot_rect(widget, ptd->spot, &xr_org);

	count = get_anno_spot_xpoint(ptd->spot, NULL, MAX_LONG);
	ppt = (xpoint_t*)xmem_alloc(sizeof(xpoint_t)*count);
	get_anno_spot_xpoint(ptd->spot, ppt, count);

	widget_size_to_tm(widget, &xs);

	for (i = 0; i < count; i++)
	{
		ppt[i].fx += xs.fx;
		ppt[i].fy += xs.fy;
	}
	
	set_anno_spot_xpoint(ptd->spot, ppt, count);
	xmem_free(ppt);

	_photoctrl_spot_rect(widget, ptd->spot, &xr);

	pt_merge_rect(&xr_org, &xr);
	pt_expand_rect(&xr_org, 100, 100);
	widget_invalid(widget, &xr_org, 0);

	pt.x = x;
	pt.y = y;
	noti_photo_owner(widget, NC_PHOTOANNODROP, ptd->spot, (void*)&pt, NULL);
}

void noti_photo_spot_sizing(res_win_t widget, long x, long y)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->spot);

	widget_set_cursor(widget, CURSOR_SIZEALL);

	_photoctrl_spot_rect(widget, ptd->spot, &xr);

	noti_photo_owner(widget, NC_PHOTOANNOSIZING, ptd->spot, (void*)&xr, NULL);
}

void noti_photo_spot_sized(res_win_t widget, long x, long y)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	xrect_t xr_org,xr;
	xsize_t xs;
	int count;
	xpoint_t* ppt;

	XDL_ASSERT(ptd->spot);

	ptd->b_size = 0;

	widget_set_cursor(widget, CURSOR_ARROW);

	xs.cx = x - ptd->org_x;
	xs.cy = y - ptd->org_y;

	if (!xs.cx && !xs.cy)
	{
		ptd->index = -1;
		return;
	}

	_photoctrl_done(widget);

	_photoctrl_spot_rect(widget, ptd->spot, &xr_org);

	count = get_anno_spot_xpoint(ptd->spot, NULL, MAX_LONG);
	ppt = (xpoint_t*)xmem_alloc(sizeof(xpoint_t)*count);
	get_anno_spot_xpoint(ptd->spot, ppt, count);

	widget_size_to_tm(widget, &xs);

	ppt[ptd->index].fx += xs.fx;
	ppt[ptd->index].fy += xs.fy;

	set_anno_spot_xpoint(ptd->spot, ppt, count);
	xmem_free(ppt);

	ptd->index = -1;

	_photoctrl_spot_rect(widget, ptd->spot, &xr);

	pt_merge_rect(&xr_org, &xr);
	pt_expand_rect(&xr_org, 100, 100);
	widget_invalid(widget, &xr_org, 0);

	noti_photo_owner(widget, NC_PHOTOANNOSIZED, ptd->spot, (void*)&xr, NULL);
}

bool_t noti_photo_spot_changing(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->spot);

	if (noti_photo_owner(widget, NC_PHOTOANNOCHANGING, ptd->spot, NULL, NULL))
		return 0;

	_photoctrl_spot_rect(widget, ptd->spot, &xr);

	ptd->spot = NULL;

	pt_expand_rect(&xr, 100, 100);

	widget_invalid(widget, &xr, 0);

	return 1;
}

void noti_photo_spot_changed(res_win_t widget, link_t_ptr ilk)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	xrect_t xr;

	XDL_ASSERT(!ptd->spot);

	ptd->spot = ilk;

	_photoctrl_spot_rect(widget, ptd->spot, &xr);

	pt_expand_rect(&xr, 100, 100);

	widget_invalid(widget, &xr, 0);

	noti_photo_owner(widget, NC_PHOTOANNOCHANGED, ptd->spot, NULL, NULL);
}

void noti_photo_begin_edit(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	xrect_t xr = { 0 };

	clr_mod_t ob = { 0 };

	XDL_ASSERT(ptd->spot);
	XDL_ASSERT(!ptd->editor);

	if (ptd->b_lock)
		return;

	widget_get_color_mode(widget, &ob);

	_photoctrl_spot_rect(widget, ptd->spot, &xr);

	if (noti_photo_owner(widget, NC_PHOTOANNOEDITING, ptd->spot, NULL, NULL))
		return;

	ptd->editor = fireedit_create(widget, &xr);
	XDL_ASSERT(ptd->editor);
	widget_set_user_id(ptd->editor, IDC_FIREEDIT);
	widget_set_owner(ptd->editor, widget);

	widget_set_color_mode(ptd->editor, &ob);
	widget_show(ptd->editor, WD_SHOW_NORMAL);
	widget_set_focus(ptd->editor);

	editbox_set_text(ptd->editor, get_anno_spot_text_ptr(ptd->spot));
	editbox_selectall(ptd->editor);
	editbox_auto_size(ptd->editor, 1);
}

void noti_photo_commit_edit(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	const tchar_t* text;
	res_win_t editctrl;
	xrect_t xr;

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->spot);

	text = (tchar_t*)editbox_get_text_ptr(ptd->editor);

	if (!noti_photo_owner(widget, NC_PHOTOANNOCOMMIT, ptd->spot, NULL, NULL))
	{
		_photoctrl_done(widget);

		set_anno_spot_text(ptd->spot, text);
	}

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);

	_photoctrl_spot_rect(widget, ptd->spot, &xr);

	pt_expand_rect(&xr, 100, 100);

	widget_invalid(widget, &xr, 0);

	widget_set_focus(widget);
}

void noti_photo_rollback_edit(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	res_win_t editctrl;

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->spot);

	noti_photo_owner(widget, NC_PHOTOANNOROLLBACK, ptd->spot, NULL, NULL);

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);
}

void noti_photo_reset_editor(res_win_t widget, bool_t bCommit)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (widget_is_valid(ptd->editor))
	{
		if (bCommit)
			noti_photo_commit_edit(widget);
		else
			noti_photo_rollback_edit(widget);
	}
}
/*****************************************************************************/
int hand_photo_create(res_win_t widget, void* data)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	widget_hand_create(widget);

	ptd = (photo_delta_t*)xmem_alloc(sizeof(photo_delta_t));
	xmem_zero((void*)ptd, sizeof(photo_delta_t));

	SETPHOTODELTA(widget, ptd);

	ptd->bmp = NULL;
	ptd->anno = NULL;
	ptd->stack = create_stack_table();

	return 0;
}

void hand_photo_destroy(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	XDL_ASSERT(ptd != NULL);

	noti_photo_reset_editor(widget, 0);

	if (ptd->bmp)
		destroy_bitmap(ptd->bmp);

	_photoctrl_clean(widget);
	destroy_stack_table(ptd->stack);

	xmem_free(ptd);

	SETPHOTODELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_photo_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	int hint;
	int ind = -1;
	link_t_ptr ilk = NULL;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd)
		return;

	if (ptd->b_drag)
	{
		noti_photo_spot_drag(widget, pxp->x, pxp->y);
	}
	else if (ptd->b_size)
	{
		noti_photo_spot_sizing(widget, pxp->x, pxp->y);
	}
	else
	{
		pt.x = pxp->x;
		pt.y = pxp->y;
		widget_point_to_tm(widget, &pt);

		widget_get_canv_rect(widget, &cb);

		hint = calc_anno_hint(&cb, &pt, ptd->anno, &ilk, &ind);

		if (hint == ANNO_HINT_SIZE && ilk == ptd->spot)
		{
			widget_set_cursor(widget, CURSOR_SIZEALL);
		}
		else if (hint == ANNO_HINT_SPOT && ilk == ptd->spot)
		{
			widget_set_cursor(widget, CURSOR_HAND);
		}
	}
}

void hand_photo_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	int hint;
	int ind = -1;
	link_t_ptr ilk = NULL;
	canvbox_t cb;
	xpoint_t pt;

	if (!ptd)
		return;

	noti_photo_reset_editor(widget, 1);

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	widget_get_canv_rect(widget, &cb);

	hint = calc_anno_hint(&cb, &pt, ptd->anno, &ilk, &ind);

	if (hint == ANNO_HINT_SIZE && ilk == ptd->spot)
	{
		ptd->org_x = pxp->x;
		ptd->org_y = pxp->y;

		ptd->b_size = 1;
		ptd->index = ind;

		widget_set_cursor(widget, CURSOR_SIZEALL);
	}
	else if (hint == ANNO_HINT_SPOT && ilk == ptd->spot)
	{
		ptd->org_x = pxp->x;
		ptd->org_y = pxp->y;

		ptd->b_drag = 1;

		widget_set_cursor(widget, CURSOR_HAND);
	}
}

void hand_photo_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	link_t_ptr ilk = NULL;
	int ind = -1;
	int hint;
	canvbox_t cb;
	xpoint_t pt;
	bool_t bRe;

	if (!ptd)
		return;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}

	if (ptd->b_size)
	{
		noti_photo_spot_sized(widget, pxp->x, pxp->y);
		return;
	}

	if (ptd->b_drag)
	{
		noti_photo_spot_drop(widget, pxp->x, pxp->y);
		return;
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	widget_get_canv_rect(widget, &cb);

	hint = calc_anno_hint(&cb, &pt, ptd->anno, &ilk, &ind);

	bRe = (ilk == ptd->spot) ? 1 : 0;

	if (ptd->spot && !bRe)
	{
		if (!noti_photo_spot_changing(widget))
			bRe = 1;
	}
	
	if (ilk && !bRe)
	{
		noti_photo_spot_changed(widget, ilk);
	}

	noti_photo_owner(widget, NC_PHOTOLBCLK, ptd->spot, (void*)pxp, NULL);
}

void hand_photo_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (ptd->spot && !ptd->b_lock)
	{
		noti_photo_begin_edit(widget);
	}

	noti_photo_owner(widget, NC_PHOTODBCLK, ptd->spot, (void*)pxp, NULL);
}

void hand_photo_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	noti_photo_reset_editor(widget, 1);
}

void hand_photo_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	noti_photo_owner(widget, NC_PHOTORBCLK, ptd->spot, (void*)pxp, NULL);
}

void hand_photo_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd)
		return;

	noti_photo_reset_editor(widget, 1);

	widget_hand_scroll(widget, bHorz, nLine);
}

void hand_photo_keydown(res_win_t widget, int key)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (key == KEY_ENTER)
	{
		noti_photo_begin_edit(widget);
	}
	else if ((key == _T('z') || key == _T('Z')) && widget_key_state(widget, KEY_CONTROL))
	{
		widget_undo(widget);
	}else if ((key == _T('c') || key == _T('C')) && widget_key_state(widget, KEY_CONTROL))
	{
		widget_copy(widget);
	}
	else if ((key == _T('x') || key == _T('X')) && widget_key_state(widget, KEY_CONTROL))
	{
		widget_cut(widget);
	}
	else if ((key == _T('v') || key == _T('V')) && widget_key_state(widget, KEY_CONTROL))
	{
		widget_paste(widget);
	}
}

void hand_photo_set_focus(res_win_t widget, res_win_t wt)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd)
		return;
}

void hand_photo_kill_focus(res_win_t widget, res_win_t wt)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd)
		return;

	if (widget_is_editor(widget))
	{
		if (photoctrl_get_dirty(widget))
			widget_send_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, (var_long)NULL);
		else
			widget_send_command(widget_get_owner(widget), COMMAND_ROLLBACK, IDC_CHILD, (var_long)NULL);
	}
}

void hand_photo_copy(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd->anno)
		return;

	_photoctrl_copy(widget);
}

void hand_photo_cut(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd->anno)
		return;

	_photoctrl_done(widget);

	if (!_photoctrl_cut(widget))
	{
		_photoctrl_discard(widget);
	}
}

void hand_photo_paste(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd->anno)
		return;

	_photoctrl_done(widget);

	if (!_photoctrl_paste(widget))
	{
		_photoctrl_discard(widget);
	}
}

void hand_photo_undo(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	if (!ptd->anno)
		return;

	_photoctrl_undo(widget);
}

void hand_photo_child_command(res_win_t widget, int code, var_long data)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	switch (code)
	{
	case COMMAND_COMMIT:
		noti_photo_commit_edit(widget);
		break;
	case COMMAND_ROLLBACK:
		noti_photo_rollback_edit(widget);
		break;
	}
}

void hand_photo_size(res_win_t widget, int code, const xsize_t* prs)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	_photoctrl_reset_page(widget);

	widget_invalid(widget, NULL, 0);
}

void hand_photo_erase(res_win_t widget, res_ctx_t dc)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
}

void hand_photo_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	res_ctx_t rdc;
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	xpen_t xp = { 0 };
	xbrush_t xb = { 0 };
	xcolor_t xc = { 0 };
	xrect_t xr;

	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb;

	widget_get_xfont(widget, &xf);
	widget_get_xface(widget, &xa);

	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	widget_get_client_rect(widget, &xr);

	canv = widget_get_canvas(widget);
	pif = create_canvas_interface(canv);
	widget_get_canv_rect(widget, &cb);

	rdc = begin_canvas_paint(canv, dc, xr.w, xr.h);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_view_rect(widget, (viewbox_t*)&xr);

	if (ptd->bmp)
	{
		draw_bitmap_raw(rdc, ptd->bmp, &xr);
	}

	noti_photo_owner(widget, NC_PHOTOFACEDRAW, NULL, (void*)&xr, rdc);

	draw_anno(pif, &cb, ptd->anno);

	if (ptd->spot)
	{
		_photoctrl_spot_rect(widget, ptd->spot, &xr);
		pt_expand_rect(&xr, 1, 1);

		parse_xcolor(&xc, DEF_ALARM_COLOR);
		draw_select_raw(rdc, &xc, &xr, ALPHA_SOLID);
	}

	end_canvas_paint(canv, dc, pxr);
	destroy_canvas_interface(pif);
}

/***************************************************************************************/
res_win_t photoctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_photo_create)
		EVENT_ON_DESTROY(hand_photo_destroy)

		EVENT_ON_ERASE(hand_photo_erase)
		EVENT_ON_PAINT(hand_photo_paint)

		EVENT_ON_SIZE(hand_photo_size)

		EVENT_ON_SCROLL(hand_photo_scroll)

		EVENT_ON_KEYDOWN(hand_photo_keydown)

		EVENT_ON_MOUSE_MOVE(hand_photo_mouse_move)
		EVENT_ON_LBUTTON_DBCLICK(hand_photo_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_photo_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_photo_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_photo_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_photo_rbutton_up)

		EVENT_ON_CHILD_COMMAND(hand_photo_child_command)

		EVENT_ON_SET_FOCUS(hand_photo_set_focus)
		EVENT_ON_KILL_FOCUS(hand_photo_kill_focus)

		EVENT_ON_COPY(hand_photo_copy)
		EVENT_ON_CUT(hand_photo_cut)
		EVENT_ON_PASTE(hand_photo_paste)
		EVENT_ON_UNDO(hand_photo_undo)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void photoctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	_photoctrl_clean(widget);

	ptd->anno = ptr;
	ptd->spot = NULL;

	photoctrl_redraw(widget);
}

link_t_ptr photoctrl_detach(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	link_t_ptr ptr;

	_photoctrl_clean(widget);

	ptr = ptd->anno;

	ptd->anno = NULL;
	ptd->spot = NULL;

	photoctrl_redraw(widget);

	return ptr;
}

link_t_ptr photoctrl_fetch(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	return ptd->anno;
}

void photoctrl_set_bitmap(res_win_t widget, const byte_t* data, dword_t size)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	res_ctx_t rdc;

	XDL_ASSERT(ptd != NULL);

	noti_photo_reset_editor(widget, 0);

	rdc = widget_client_ctx(widget);

	if (ptd->bmp)
	{
		destroy_bitmap(ptd->bmp);
		ptd->bmp = NULL;
	}

	ptd->bmp = load_bitmap_from_bytes(rdc, data, size);

	widget_release_ctx(widget, rdc);

	_photoctrl_reset_page(widget);

	widget_invalid(widget, NULL, 0);
}

dword_t photoctrl_get_bitmap(res_win_t widget, byte_t* buf, dword_t max)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	dword_t len_bmp;
	res_ctx_t rdc;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->bmp)
		return 0;

	len_bmp = get_bitmap_bytes(ptd->bmp);
	if (!buf)
		return len_bmp;

	rdc = widget_client_ctx(widget);

	save_bitmap_to_bytes(rdc, ptd->bmp, buf, max);

	widget_release_ctx(widget, rdc);

	return max;
}

void photoctrl_redraw(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	XDL_ASSERT(ptd != NULL);

	noti_photo_reset_editor(widget, 0);

	widget_invalid(widget, NULL, 0);
}

void photoctrl_set_lock(res_win_t widget,bool_t bLock)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_lock = bLock;
}

bool_t photoctrl_get_lock(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->b_lock;
}

link_t_ptr photoctrl_get_focus_spot(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->spot;
}

void photoctrl_commit(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);
	res_bmp_t newbmp;

	XDL_ASSERT(ptd != NULL);

	noti_photo_reset_editor(widget, 1);

	newbmp = _photoctrl_merge_anno(widget);
	destroy_bitmap(ptd->bmp);

	ptd->bmp = newbmp;
	ptd->spot = NULL;

	_photoctrl_clean(widget);

	if (ptd->anno)
		clear_anno_doc(ptd->anno);

	photoctrl_redraw(widget);
}

bool_t photoctrl_get_dirty(res_win_t widget)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return peek_stack_node(ptd->stack, -1) ? 1 : 0;
}

void photoctrl_set_dirty(res_win_t widget, bool_t bDirty)
{
	photo_delta_t* ptd = GETPHOTODELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->anno)
		return;

	if (bDirty)
		_photoctrl_done(widget);
	else
		_photoctrl_clean(widget);
}
