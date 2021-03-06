﻿/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc form control document

	@module	formctrl.c | form control implement file

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

#define FORM_LINE_FEED		(float)50
#define FIELD_MIN_WIDTH		(float)10
#define FIELD_MIN_HEIGHT	(float)10

typedef struct _form_delta_t{
	link_t_ptr form;
	link_t_ptr field;
	link_t_ptr hover;

	int org_hint;
	long org_x, org_y;
	long cur_x, cur_y;
	short cur_page;
	short max_page;

	res_win_t editor;
	tchar_t pch[CHS_LEN + 1];

	bool_t b_drag;
	bool_t b_size;
	bool_t b_group;
	bool_t b_alarm;
	bool_t b_lock;

	link_t_ptr stack;
}form_delta_t;

#define GETFORMDELTA(ph) 	(form_delta_t*)widget_get_user_delta(ph)
#define SETFORMDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

/******************************************form event********************************************************/
static void _formctrl_done(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	byte_t* buf;
	dword_t len;

	XDL_ASSERT(ptd && ptd->form);

#ifdef _UNICODE
	len = format_dom_doc_to_bytes(ptd->form, NULL, MAX_LONG, DEF_UCS);
#else
	len = format_dom_doc_to_bytes(ptd->form, NULL, MAX_LONG, DEF_MBS);
#endif

	buf = (byte_t*)xmem_alloc(len + sizeof(tchar_t));

#ifdef _UNICODE
	format_dom_doc_to_bytes(ptd->form, buf, len, DEF_UCS);
#else
	format_dom_doc_to_bytes(ptd->form, buf, len, DEF_MBS);
#endif

	push_stack_node(ptd->stack, (void*)buf);
}

static void _formctrl_undo(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	void* p;
	int len;

	XDL_ASSERT(ptd && ptd->form);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		clear_form_doc(ptd->form);

		len = xslen((tchar_t*)p);

#ifdef _UNICODE
		parse_dom_doc_from_bytes(ptd->form, (byte_t*)p, len * sizeof(tchar_t), DEF_UCS);
#else
		parse_dom_doc_from_bytes(ptd->form, (byte_t*)p, len * sizeof(tchar_t), DEF_MBS);
#endif

		xmem_free(p);

		formctrl_redraw(widget, 1);
	}
}

static void _formctrl_discard(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	p = pop_stack_node(ptd->stack);
	if (p)
	{
		xmem_free(p);
	}
}

static void _formctrl_clean(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	void* p;

	XDL_ASSERT(ptd && ptd->stack);

	while (p = pop_stack_node(ptd->stack))
	{
		xmem_free(p);
	}
}

static bool_t _formctrl_copy(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk, ilk;

	XDL_ASSERT(ptd && ptd->form);

	if (!form_is_design(ptd->form))
		return 0;

	if (!get_field_selected_count(ptd->form) && !ptd->field)
		return 0;

	if (!clipboard_open())
		return 0;

	dom = create_form_doc();
	ilk = get_next_field(ptd->form, LINK_FIRST);
	while (ilk)
	{
		if (get_field_selected(ilk) || ilk == ptd->field)
		{
			nlk = insert_field(dom, get_field_class_ptr(ilk));
			copy_dom_node(nlk, ilk);
		}

		ilk = get_next_field(ptd->form, ilk);
	}

#ifdef _UNICODE
	len = format_dom_doc_to_bytes(dom, NULL, MAX_LONG, DEF_UCS);
#else
	len = format_dom_doc_to_bytes(dom, NULL, MAX_LONG, DEF_MBS);
#endif

	gb = gmem_alloc(len + sizeof(tchar_t));
	buf = (byte_t*)gmem_lock(gb);

#ifdef _UNICODE
	format_dom_doc_to_bytes(dom, buf, len, DEF_UCS);
#else
	format_dom_doc_to_bytes(dom, buf, len, DEF_MBS);
#endif

	gmem_unlock(gb);

	if (!clipboard_put(DEF_CB_FORMAT, gb))
	{
		gmem_free(gb);
		clipboard_close();

		destroy_form_doc(dom);
		return 0;
	}

	clipboard_close();

	destroy_form_doc(dom);
	return 1;
}

static bool_t _formctrl_cut(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	link_t_ptr nxt, ilk;

	XDL_ASSERT(ptd && ptd->form);

	if (!_formctrl_copy(widget))
		return 0;

	ilk = get_next_field(ptd->form, LINK_FIRST);
	while (ilk)
	{
		nxt = get_next_field(ptd->form, ilk);

		if (get_field_selected(ilk) || ilk == ptd->field)
		{
			delete_field(ilk);
		}
		ilk = nxt;
	}

	formctrl_redraw(widget, 1);

	return 1;
}

static bool_t _formctrl_paste(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	res_glob_t gb;
	int len;
	byte_t* buf;
	link_t_ptr dom, nlk;

	float y;
	tchar_t sz_name[RES_LEN + 1] = { 0 };

	XDL_ASSERT(ptd && ptd->form);

	if (!form_is_design(ptd->form))
		return 0;

	if (!clipboard_open())
		return 0;

	gb = clipboard_get(DEF_CB_FORMAT);
	if (!gb)
	{
		clipboard_close();
		return 0;
	}

	len = gmem_size(gb);
	buf = (byte_t*)gmem_lock(gb);

	dom = create_dom_doc();
#ifdef _UNICODE
	if (!parse_dom_doc_from_bytes(dom, buf, len, DEF_UCS))
#else
	if (!parse_dom_doc_from_bytes(dom, buf, len, DEF_MBS))
#endif
	{
		gmem_unlock(gb);
		clipboard_close();

		destroy_dom_doc(dom);
		return 0;
	}

	if (!is_form_doc(dom))
	{
		gmem_unlock(gb);
		clipboard_close();

		destroy_dom_doc(dom);
		return 0;
	}

	while (nlk = get_next_field(dom, LINK_FIRST))
	{
		nlk = detach_dom_node(get_form_fieldset(dom), nlk);
		attach_dom_node(get_form_fieldset(ptd->form), LINK_LAST, nlk);

		y = get_field_y(nlk);
		y += get_field_height(nlk);
		set_field_y(nlk, y);

		xsprintf(sz_name, _T("%s%d"), get_field_class_ptr(nlk), get_field_count_by_class(ptd->form, get_field_class_ptr(nlk)));
		set_field_name(nlk, sz_name);
	}

	gmem_unlock(gb);
	clipboard_clean();
	clipboard_close();

	destroy_dom_doc(dom);
	formctrl_redraw(widget, 1);

	return 1;
}

static void _formctrl_field_rect(res_win_t widget, link_t_ptr flk, xrect_t* pxr)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_form_field_rect(&cb, ptd->form, flk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _formctrl_group_rect(res_win_t widget, link_t_ptr flk, xrect_t* pxr)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	canvbox_t cb;

	widget_get_canv_rect(widget, &cb);

	calc_form_group_rect(&cb, ptd->form, flk, pxr);

	widget_rect_to_pt(widget, pxr);
}

static void _formctrl_reset_page(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	long pw, ph, fw, fh, lw, lh;
	xrect_t xr;
	xsize_t xs;

	widget_get_client_rect(widget, &xr);
	pw = xr.w;
	ph = xr.h;

	xs.fx = get_form_width(ptd->form);
	xs.fy = get_form_height(ptd->form);

	widget_size_to_pt(widget, &xs);
	fw = xs.cx;
	fh = xs.cy;

	xs.fx = (float)10;
	xs.fy = (float)10;
	widget_size_to_pt(widget, &xs);
	lw = xs.cx;
	lh = xs.cy;

	widget_reset_paging(widget, pw, ph, fw, fh, lw, lh);

	widget_reset_scroll(widget, 1);

	widget_reset_scroll(widget, 0);
}

static void _formctrl_ensure_visible(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	xrect_t xr = { 0 };

	if (!ptd->field)
		return;

	_formctrl_field_rect(widget, ptd->field, &xr);

	widget_ensure_visible(widget, &xr, 1);
}

static void _formctrl_reset_group(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	LINKPTR flk;
	long gid;

	XDL_ASSERT(ptd->field);

	gid = get_field_group(ptd->field);
	if (!gid)
		return;

	if (is_null(get_field_text_ptr(ptd->field)))
		return;

	flk = get_next_visible_field(ptd->form, LINK_FIRST);
	while (flk)
	{
		if (flk == ptd->field)
		{
			flk = get_next_visible_field(ptd->form, flk);
			continue;
		}

		if (compare_text(get_field_class_ptr(flk), -1, DOC_FORM_CHECK, -1, 0) != 0)
		{
			flk = get_next_visible_field(ptd->form, flk);
			continue;
		}

		if (get_field_group(flk) != gid)
		{
			flk = get_next_visible_field(ptd->form, flk);
			continue;
		}

		if (!is_null(get_field_text_ptr(flk)))
		{
			formctrl_set_field_text(widget, flk, NULL);
		}

		flk = get_next_visible_field(ptd->form, flk);
	}
}
/*********************************************************************************************************/
int noti_form_owner(res_win_t widget, unsigned long code, link_t_ptr form, link_t_ptr flk, void* data)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	NOTICE_FORM nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;

	nf.data = data;	
	nf.ret = 0;

	nf.form = form;
	nf.field = flk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);
	return nf.ret;
}

void noti_form_reset_select(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	link_t_ptr flk;
	int count = 0;

	flk = get_next_field(ptd->form, LINK_FIRST);
	while (flk)
	{
		if (get_field_selected(flk))
		{
			set_field_selected(flk, 0);
			noti_form_owner(widget, NC_FIELDSELECTED, ptd->form, flk, NULL);

			count++;
		}

		flk = get_next_field(ptd->form, flk);
	}

	if (count)
	{
		widget_update(widget, NULL, 0);
	}
}

void noti_form_field_selected(res_win_t widget, link_t_ptr flk)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	xrect_t xr;
	bool_t b_check;

	b_check = get_field_selected(flk);

	if (b_check)
		set_field_selected(flk, 0);
	else
		set_field_selected(flk, 1);

	noti_form_owner(widget, NC_FIELDSELECTED, ptd->form, flk, NULL);

	_formctrl_field_rect(widget, flk, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);
}

bool_t noti_form_field_changing(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd->field);

	if (noti_form_owner(widget, NC_FIELDCHANGING, ptd->form, ptd->field, NULL))
		return (bool_t)0;

	ptd->b_alarm = (bool_t)0;

	if (form_is_design(ptd->form) && get_field_group(ptd->field))
		_formctrl_group_rect(widget, ptd->field, &xr);
	else
		_formctrl_field_rect(widget, ptd->field, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	ptd->field = NULL;

	widget_update(widget, &xr, 0);

	return (bool_t)1;
}

void noti_form_field_changed(res_win_t widget, link_t_ptr flk)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(flk);
	XDL_ASSERT(!ptd->field);

	ptd->field = flk;

	if (form_is_design(ptd->form) && get_field_group(ptd->field))
		_formctrl_group_rect(widget, ptd->field, &xr);
	else
		_formctrl_field_rect(widget, ptd->field, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);

	noti_form_owner(widget, NC_FIELDCHANGED, ptd->form, flk, NULL);
}

void noti_form_field_enter(res_win_t widget, link_t_ptr flk)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(flk);
	XDL_ASSERT(!ptd->hover);

	ptd->hover = flk;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_LEAVE);
	}
}

void noti_form_field_leave(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd->hover != NULL);

	ptd->hover = NULL;

	if (widget_is_hotvoer(widget))
	{
		widget_track_mouse(widget, MS_TRACK_HOVER | MS_TRACK_CANCEL);
	}
}

void noti_form_field_hover(res_win_t widget, long x, long y)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	xpoint_t xp;

	XDL_ASSERT(ptd->hover != NULL);

	xp.x = x;
	xp.y = y;
	noti_form_owner(widget, NC_FIELDHOVER, ptd->form, ptd->hover, (void*)&xp);
}

void noti_form_field_drag(res_win_t widget, long x, long y)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	xpoint_t pt;

	XDL_ASSERT(ptd->field);

	ptd->b_drag = (bool_t)1;
	ptd->org_x = x;
	ptd->org_y = y;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}
	widget_set_cursor(widget,CURSOR_HAND);

	pt.x = x;
	pt.y = y;
	noti_form_owner(widget, NC_FIELDDRAG, ptd->form, ptd->field, (void*)&pt);
}

void noti_form_field_drop(res_win_t widget, long x, long y)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	
	xpoint_t pt;
	xrect_t xr;
	link_t_ptr flk;
	int gid;
	long cx, cy;
	canvbox_t cb;

	XDL_ASSERT(ptd->field);

	ptd->cur_x = x;
	ptd->cur_y = y;

	ptd->b_drag = (bool_t)0;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	cx = x - ptd->org_x;
	cy = y - ptd->org_y;

	if (!cx && !cy)
		return;

	_formctrl_done(widget);

	widget_get_canv_rect(widget, &cb);

	calc_form_group_rect(&cb, ptd->form, ptd->field, &xr);

	widget_rect_to_pt(widget, &xr);

	if (xr.x + cx < 0 || xr.y + cy < 0)
		return;

	gid = get_field_group(ptd->field);

	flk = get_next_field(ptd->form, LINK_FIRST);
	while (flk)
	{
		if (flk != ptd->field)
		{
			if ((!gid && !get_field_selected(flk)) || (gid && get_field_group(flk) != gid))
			{
				flk = get_next_field(ptd->form, flk);
				continue;
			}
		}

		calc_form_field_rect(&cb, ptd->form, flk, &xr);

		widget_rect_to_pt(widget, &xr);

		pt.x = xr.x + cx;
		pt.y = xr.y + cy;
	
		widget_point_to_tm(widget, &pt);

		pt.fx = (float)((long)(pt.fx));
		pt.fy = (float)((long)(pt.fy));

		set_field_x(flk, pt.fx);
		set_field_y(flk, pt.fy);

		flk = get_next_field(ptd->form, flk);
	}

	widget_update(widget, NULL, 0);

	pt.x = x;
	pt.y = y;
	noti_form_owner(widget, NC_FIELDDROP, ptd->form, ptd->field, (void*)&pt);
}

void noti_form_field_sizing(res_win_t widget, int hint, long x, long y)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd->field);

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}

	if (hint == FORM_HINT_HORZ_SPLIT)
	{
		widget_set_cursor(widget,CURSOR_SIZENS);
	}
	else if (hint == FORM_HINT_VERT_SPLIT)
	{
		widget_set_cursor(widget,CURSOR_SIZEWE);
	}
	else
	{
		widget_set_cursor(widget,CURSOR_SIZEALL);
	}

	ptd->org_hint = hint;
	ptd->org_x = x;
	ptd->org_y = y;

	ptd->b_size = (bool_t)1;

	noti_form_owner(widget, NC_FIELDSIZING, ptd->form, ptd->field, NULL);
}

void noti_form_field_sized(res_win_t widget, long x, long y)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	float minw, minh, fw, fh;
	int hint;
	xrect_t xr;
	xsize_t xs;

	XDL_ASSERT(ptd->field);

	ptd->cur_x = x;
	ptd->cur_y = y;

	ptd->b_size = (bool_t)0;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	hint = ptd->org_hint;

	minw = FIELD_MIN_WIDTH;
	minh = FIELD_MIN_HEIGHT;

	xs.cx = ptd->cur_x - ptd->org_x;
	xs.cy = ptd->cur_y - ptd->org_y;

	if (!xs.cx && !xs.cy)
		return;

	_formctrl_done(widget);

	widget_size_to_tm(widget, &xs);

	_formctrl_field_rect(widget, ptd->field, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);

	fw = get_field_width(ptd->field);
	fh = get_field_height(ptd->field);

	fw += xs.fx;
	fh += xs.fy;

	if (fw < minw)
		fw = minw;

	if (fh < minh)
		fh = minh;

	fw = (float)((long)fw);
	fh = (float)((long)fh);

	if (hint == FORM_HINT_HORZ_SPLIT)
	{
		set_field_height(ptd->field, fh);
	}
	else if (hint == FORM_HINT_VERT_SPLIT)
	{
		set_field_width(ptd->field, fw);
	}
	else
	{
		set_field_width(ptd->field, fw);
		set_field_height(ptd->field, fh);
	}

	_formctrl_field_rect(widget, ptd->field, &xr);

	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);

	noti_form_owner(widget, NC_FIELDSIZED, ptd->form, ptd->field, NULL);
}

void noti_form_begin_group(res_win_t widget, long x, long y)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 1);
	}
	widget_set_cursor(widget,CURSOR_HAND);

	ptd->b_group = (bool_t)1;
	ptd->org_x = x;
	ptd->org_y = y;
}

void noti_form_end_group(res_win_t widget, long x, long y)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	link_t_ptr flk;
	xrect_t xr_group, xr;

	ptd->b_group = (bool_t)0;

	if (widget_can_focus(widget))
	{
		widget_set_capture(widget, 0);
	}
	widget_set_cursor(widget, CURSOR_ARROW);

	ptd->cur_x = x;
	ptd->cur_y = y;

	xr_group.x = (ptd->org_x < ptd->cur_x) ? ptd->org_x : ptd->cur_x;
	xr_group.y = (ptd->org_y < ptd->cur_y) ? ptd->org_y : ptd->cur_y;
	xr_group.w = (ptd->org_x > ptd->cur_x) ? ptd->org_x : ptd->cur_x - xr_group.x;
	xr_group.h = (ptd->org_y > ptd->cur_y) ? ptd->org_y : ptd->cur_y - xr_group.y;

	flk = get_next_field(ptd->form, LINK_FIRST);
	while (flk)
	{
		_formctrl_field_rect(widget, flk, &xr);

		if (pt_inside(xr.x, xr.y, xr_group.x, xr_group.y, xr_group.x + xr_group.w, xr_group.y + xr_group.h))
		{
			set_field_selected(flk, 1);
		}

		flk = get_next_field(ptd->form, flk);
	}

	widget_update(widget, NULL, 0);
}

void noti_form_begin_edit(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	const tchar_t* fclass;
	const tchar_t* editor;
	const tchar_t* text;
	link_t_ptr data;

	xrect_t xr;
	xsize_t xs;

	EDITDELTA fd = { 0 };

	clr_mod_t ob = { 0 };
	xfont_t xf = { 0 };
	xface_t xa = { 0 };

	XDL_ASSERT(ptd->field);

	if (widget_is_valid(ptd->editor))
		return;

	if (form_is_design(ptd->form) || !get_field_focusable(ptd->field))
	{
		return;
	}

	fclass = get_field_class_ptr(ptd->field);

	if (compare_text(fclass, -1, DOC_FORM_TEXT, -1, 0) == 0)
	{
		editor = get_field_editor_ptr(ptd->field);
	}
	else if (compare_text(fclass, -1, DOC_FORM_CHECK, -1, 0) == 0)
	{
		editor = ATTR_EDITOR_FIRECHECK;
	}
	else if (compare_text(fclass, -1, DOC_FORM_RICH, -1, 0) == 0)
	{
		editor = ATTR_EDITOR_RICHBOX;
	}
	else if (compare_text(fclass, -1, DOC_FORM_TAG, -1, 0) == 0)
	{
		editor = ATTR_EDITOR_TAGBOX;
	}
	else if (compare_text(fclass, -1, DOC_FORM_TABLE, -1, 0) == 0)
	{
		editor = ATTR_EDITOR_TABLEBOX;
	}
	else if (compare_text(fclass, -1, DOC_FORM_MEMO, -1, 0) == 0)
	{
		editor = ATTR_EDITOR_MEMOBOX;
	}
	else if (compare_text(fclass, -1, DOC_FORM_GRID, -1, 0) == 0)
	{
		editor = ATTR_EDITOR_GRIDBOX;
	}
	else if (compare_text(fclass, -1, DOC_FORM_GRAPH, -1, 0) == 0)
	{
		editor = ATTR_EDITOR_GRAPHBOX;
	}
	else if (compare_text(fclass, -1, DOC_FORM_FORM, -1, 0) == 0)
	{
		editor = ATTR_EDITOR_FORMBOX;
	}
	else
	{
		return;
	}

	widget_get_xfont(widget, &xf);
	parse_xfont_from_style(&xf, get_field_style_ptr(ptd->field));
	widget_get_xface(widget, &xa);
	parse_xface_from_style(&xa, get_field_style_ptr(ptd->field));
	widget_get_color_mode(widget, &ob);

	_formctrl_field_rect(widget, ptd->field, &xr);
	pt_expand_rect(&xr, DEF_INNER_FEED, DEF_INNER_FEED);

	xs.fx = WIDGET_SCROLL_SPAN;
	xs.fy = WIDGET_SCROLL_SPAN;

	widget_size_to_pt(widget, &xs);

	if (compare_text(editor, -1, ATTR_EDITOR_FIREEDIT, -1, 0) == 0)
	{
		if (noti_form_owner(widget, NC_FIELDEDITING, ptd->form, ptd->field, NULL))
			return;

		ptd->editor = fireedit_create(widget, &xr);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIREEDIT);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_color_mode(ptd->editor, &ob);

		if (ptd->pch[0])
		{
			editbox_set_text(ptd->editor, ptd->pch);
		}
		else
		{
			text = get_field_text_ptr(ptd->field);
			editbox_set_text(ptd->editor, text);
		}

		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);

		if (ptd->pch[0])
		{
			ptd->pch[0] = _T('\0');
		}
		else
		{
			editbox_selectall(ptd->editor);
		}
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIRECHECK, -1, 0) == 0)
	{
		if (noti_form_owner(widget, NC_FIELDEDITING, ptd->form, ptd->field, NULL))
			return;

		ptd->editor = firecheck_create(widget, &xr);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIRECHECK);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_color_mode(ptd->editor, &ob);

		text = get_field_text_ptr(ptd->field);
		if (is_null(text))
			checkbox_set_state(ptd->editor, 1);
		else
			checkbox_set_state(ptd->editor, 0);

		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIRENUM, -1, 0) == 0)
	{
		if (noti_form_owner(widget, NC_FIELDEDITING, ptd->form, ptd->field, NULL))
			return;

		ptd->editor = firenum_create(widget, &xr);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIRENUM);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_color_mode(ptd->editor, &ob);

		if (ptd->pch[0])
		{
			editbox_set_text(ptd->editor, ptd->pch);
		}
		else
		{
			text = get_field_text_ptr(ptd->field);
			editbox_set_text(ptd->editor, text);
		}

		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);

		if (ptd->pch[0])
		{
			ptd->pch[0] = _T('\0');
		}
		else
		{
			editbox_selectall(ptd->editor);
		}
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIREDATE, -1, 0) == 0)
	{
		if (noti_form_owner(widget, NC_FIELDEDITING, ptd->form, ptd->field, NULL))
			return;

		ptd->editor = firedate_create(widget, &xr);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIREDATE);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_color_mode(ptd->editor, &ob);

		text = get_field_text_ptr(ptd->field);
		editbox_set_text(ptd->editor, text);

		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);

		editbox_selectall(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIRETIME, -1, 0) == 0)
	{
		if (noti_form_owner(widget, NC_FIELDEDITING, ptd->form, ptd->field, NULL))
			return;

		ptd->editor = firetime_create(widget, &xr);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIRETIME);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_color_mode(ptd->editor, &ob);

		text = get_field_text_ptr(ptd->field);
		editbox_set_text(ptd->editor, text);

		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);

		editbox_selectall(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIRELIST, -1, 0) == 0)
	{
		if (noti_form_owner(widget, NC_FIELDEDITING, ptd->form, ptd->field, NULL))
			return;
		
		data = get_field_options_table(ptd->field);
		if (!data)
			return;

		ptd->editor = firelist_create(widget, &xr, data);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIRELIST);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_color_mode(ptd->editor, &ob);

		text = get_field_text_ptr(ptd->field);
		editbox_set_text(ptd->editor, text);

		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);

		editbox_selectall(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIREWORDS, -1, 0) == 0)
	{
		if (noti_form_owner(widget, NC_FIELDEDITING, ptd->form, ptd->field, (void*)&fd))
			return;

		data = (link_t_ptr)fd.data;
		ptd->editor = firewords_create(widget, &xr, data);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIREWORDS);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_color_mode(ptd->editor, &ob);

		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);

		text = get_field_text_ptr(ptd->field);
		editbox_set_text(ptd->editor, text);
		editbox_selectall(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FIREGRID, -1, 0) == 0)
	{
		if (noti_form_owner(widget, NC_FIELDEDITING, ptd->form, ptd->field, (void*)&fd))
			return;

		data = (link_t_ptr)fd.data;
		if (!data)
			return;

		ptd->editor = firegrid_create(widget, &xr, data);
		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FIREGRID);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_color_mode(ptd->editor, &ob);

		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_TABLEBOX, -1, 0) == 0)
	{
		if (noti_form_owner(widget, NC_FIELDEDITING, ptd->form, ptd->field, &fd))
			return;

		pt_expand_rect(&xr, -DEF_INNER_FEED, -DEF_INNER_FEED);
		xr.h += xs.cy;

		if (fd.menu)
		{
			ptd->editor = tablectrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_VSCROLL | WD_STYLE_MENUBAR, &xr, widget);
		}
		else
		{
			ptd->editor = tablectrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_VSCROLL, &xr, widget);
		}

		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_TABLEBOX);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_xface(ptd->editor, &xa);
		widget_set_color_mode(ptd->editor, &ob);
		widget_update_window(ptd->editor);

		if (fd.menu)
		{
			widget_attach_menu(ptd->editor, fd.menu);
			widget_update_window(ptd->editor);
		}

		tablectrl_set_lock(ptd->editor, 0);
		tablectrl_auto_insert(ptd->editor, 1);

		text = get_field_text_ptr(ptd->field);
		data = create_string_table(0);
		string_table_parse_options(data, text, -1, OPT_ITEMFEED, OPT_LINEFEED);

		tablectrl_attach(ptd->editor, data);

		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_GRIDBOX, -1, 0) == 0)
	{
		data = get_field_embed_grid(ptd->field);
		if (!data)
			return;

		if (noti_form_owner(widget, NC_FIELDEDITING, ptd->form, ptd->field, &fd))
			return;

		pt_expand_rect(&xr, -DEF_INNER_FEED, -DEF_INNER_FEED);
		xr.w += xs.cx;
		xr.h += xs.cy;

		if (fd.menu)
		{
			ptd->editor = gridctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL | WD_STYLE_MENUBAR, &xr, widget);
		}
		else
		{
			ptd->editor = gridctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, widget);
		}

		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_GRIDBOX);
		widget_set_owner(ptd->editor, widget);

		widget_set_color_mode(ptd->editor, &ob);
		widget_update_window(ptd->editor);

		if (fd.menu)
		{
			widget_attach_menu(ptd->editor, fd.menu);
			widget_update_window(ptd->editor);
		}

		set_grid_width(data, get_field_width(ptd->field));
		set_grid_height(data, get_field_height(ptd->field));

		gridctrl_set_lock(ptd->editor, 0);
		gridctrl_auto_insert(ptd->editor, 1);

		gridctrl_attach(ptd->editor, data);

		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_GRAPHBOX, -1, 0) == 0)
	{
		data = get_field_embed_graph(ptd->field);
		if (!data)
			return;

		if (noti_form_owner(widget, NC_FIELDEDITING, ptd->form, ptd->field, &fd))
			return;

		pt_expand_rect(&xr, -DEF_INNER_FEED, -DEF_INNER_FEED);
		xr.w += xs.cx;
		xr.h += xs.cy;

		if (fd.menu)
			ptd->editor = graphctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL | WD_STYLE_MENUBAR, &xr, widget);
		else
			ptd->editor = graphctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, widget);

		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_GRAPHBOX);
		widget_set_owner(ptd->editor, widget);

		widget_set_color_mode(ptd->editor, &ob);
		widget_update_window(ptd->editor);

		if (fd.menu)
		{
			widget_attach_menu(ptd->editor, fd.menu);
			widget_update_window(ptd->editor);
		}

		graphctrl_set_lock(ptd->editor, 0);
		graphctrl_auto_insert(ptd->editor, 1);

		set_graph_width(data, get_field_width(ptd->field));
		set_graph_height(data, get_field_height(ptd->field));

		graphctrl_attach(ptd->editor, data);

		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_FORMBOX, -1, 0) == 0)
	{
		data = get_field_embed_form(ptd->field);
		if (!data)
			return;

		if (noti_form_owner(widget, NC_FIELDEDITING, ptd->form, ptd->field, &fd))
			return;

		pt_expand_rect(&xr, -DEF_INNER_FEED, -DEF_INNER_FEED);
		xr.w += xs.cx;
		xr.h += xs.cy;

		if (fd.menu)
			ptd->editor = formctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL | WD_STYLE_MENUBAR, &xr, widget);
		else
			ptd->editor = formctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, widget);

		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_FORMBOX);
		widget_set_owner(ptd->editor, widget);

		widget_set_color_mode(ptd->editor, &ob);
		widget_update_window(ptd->editor);

		if (fd.menu)
		{
			widget_attach_menu(ptd->editor, fd.menu);
			widget_update_window(ptd->editor);
		}

		formctrl_set_lock(ptd->editor, 0);

		set_form_width(data, get_field_width(ptd->field));
		set_form_height(data, get_field_height(ptd->field));

		formctrl_attach(ptd->editor, data);

		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_TAGBOX, -1, 0) == 0)
	{
		if (noti_form_owner(widget, NC_FIELDEDITING, ptd->form, ptd->field, &fd))
			return;

		pt_expand_rect(&xr, -DEF_INNER_FEED, -DEF_INNER_FEED);
		xr.w += xs.cx;
		xr.h += xs.cy;

		if (fd.menu)
			ptd->editor = tagctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL | WD_STYLE_MENUBAR, &xr, widget);
		else
			ptd->editor = tagctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, widget);

		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_TAGBOX);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_xface(ptd->editor, &xa);
		widget_set_color_mode(ptd->editor, &ob);
		widget_update_window(ptd->editor);

		if (fd.menu)
		{
			widget_attach_menu(ptd->editor, fd.menu);
			widget_update_window(ptd->editor);
		}

		tagctrl_set_lock(ptd->editor, 0);

		text = get_field_text_ptr(ptd->field);
		data = create_tag_doc();
		parse_tag_doc(data, text, -1);

		tagctrl_attach(ptd->editor, data);

		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_MEMOBOX, -1, 0) == 0)
	{
		if (noti_form_owner(widget, NC_FIELDEDITING, ptd->form, ptd->field, &fd))
			return;

		pt_expand_rect(&xr, -DEF_INNER_FEED, -DEF_INNER_FEED);
		xr.w += xs.cx;
		xr.h += xs.cy;

		if (fd.menu)
			ptd->editor = memoctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL | WD_STYLE_MENUBAR, &xr, widget);
		else
			ptd->editor = memoctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, widget);

		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_MEMOBOX);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_xface(ptd->editor, &xa);
		widget_set_color_mode(ptd->editor, &ob);
		widget_update_window(ptd->editor);

		if (fd.menu)
		{
			widget_attach_menu(ptd->editor, fd.menu);
			widget_update_window(ptd->editor);
		}

		memoctrl_set_lock(ptd->editor, 0);

		text = get_field_text_ptr(ptd->field);
		data = create_memo_doc();
		parse_memo_doc(data, text, -1);

		set_memo_width(data, get_field_width(ptd->field));
		set_memo_height(data, get_field_height(ptd->field));
		memoctrl_attach(ptd->editor, data);

		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);
	}
	else if (compare_text(editor, -1, ATTR_EDITOR_RICHBOX, -1, 0) == 0)
	{
		data = get_field_embed_rich(ptd->field);
		if (!data)
			return;

		if (noti_form_owner(widget, NC_FIELDEDITING, ptd->form, ptd->field, &fd))
			return;

		pt_expand_rect(&xr, -DEF_INNER_FEED, -DEF_INNER_FEED);
		xr.w += xs.cx;
		xr.h += xs.cy;

		if (fd.menu)
			ptd->editor = richctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL | WD_STYLE_MENUBAR, &xr, widget);
		else
			ptd->editor = richctrl_create(NULL, WD_STYLE_CONTROL | WD_STYLE_HSCROLL | WD_STYLE_VSCROLL, &xr, widget);

		XDL_ASSERT(ptd->editor);
		widget_set_user_id(ptd->editor, IDC_RICHBOX);
		widget_set_owner(ptd->editor, widget);

		widget_set_xfont(ptd->editor, &xf);
		widget_set_xface(ptd->editor, &xa);
		widget_set_color_mode(ptd->editor, &ob);
		widget_update_window(ptd->editor);

		if (fd.menu)
		{
			widget_attach_menu(ptd->editor, fd.menu);
			widget_update_window(ptd->editor);
		}

		richctrl_set_lock(ptd->editor, 0);

		set_rich_width(data, get_field_width(ptd->field));
		set_rich_height(data, get_field_height(ptd->field));
		richctrl_attach(ptd->editor, data);

		widget_show(ptd->editor, WD_SHOW_NORMAL);
		widget_set_focus(ptd->editor);
	}

	if (!widget_is_valid(ptd->editor))
		return;

	set_field_visible(ptd->field, 0);
}

void noti_form_commit_edit(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	res_win_t editctrl;
	dword_t uid;
	tchar_t* text;
	int len;
	link_t_ptr item, data;
	bool_t dirty;
	bool_t b_accept = 0;

	EDITDELTA fd = { 0 };

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->field);

	set_field_visible(ptd->field, 1);

	uid = widget_get_user_id(ptd->editor);

	if (uid == IDC_FIREEDIT)
	{
		text = (tchar_t*)editbox_get_text_ptr(ptd->editor);
		b_accept = (noti_form_owner(widget, NC_FIELDCOMMIT, ptd->form, ptd->field, (void*)text) == 0) ? 1 : 0;
		if (b_accept)
		{
			formctrl_set_field_text(widget, ptd->field, text);
		}
	}
	else if (uid == IDC_FIRECHECK)
	{
		if (checkbox_get_state(ptd->editor))
			text = get_field_value_ptr(ptd->field);
		else
			text = NULL;
		b_accept = (noti_form_owner(widget, NC_FIELDCOMMIT, ptd->form, ptd->field, (void*)text) == 0) ? 1 : 0;
		if (b_accept)
		{
			formctrl_set_field_text(widget, ptd->field, text);

			_formctrl_reset_group(widget);
		}
	}
	else if (uid == IDC_FIRELIST)
	{
		text = (tchar_t*)editbox_get_text_ptr(ptd->editor);
		b_accept = (noti_form_owner(widget, NC_FIELDCOMMIT, ptd->form, ptd->field, (void*)text) == 0) ? 1 : 0;
		if (b_accept)
		{
			formctrl_set_field_text(widget, ptd->field, text);
		}
	}
	else if (uid == IDC_FIRENUM)
	{
		text = (tchar_t*)editbox_get_text_ptr(ptd->editor);
		b_accept = (noti_form_owner(widget, NC_FIELDCOMMIT, ptd->form, ptd->field, (void*)text) == 0) ? 1 : 0;
		if (b_accept)
		{
			formctrl_set_field_text(widget, ptd->field, text);
		}
	}
	else if (uid == IDC_FIREDATE)
	{
		text = (tchar_t*)editbox_get_text_ptr(ptd->editor);
		b_accept = (noti_form_owner(widget, NC_FIELDCOMMIT, ptd->form, ptd->field, (void*)text) == 0) ? 1 : 0;
		if (b_accept)
		{
			formctrl_set_field_text(widget, ptd->field, text);
		}
	}
	else if (uid == IDC_FIRETIME)
	{
		text = (tchar_t*)editbox_get_text_ptr(ptd->editor);
		b_accept = (noti_form_owner(widget, NC_FIELDCOMMIT, ptd->form, ptd->field, (void*)text) == 0) ? 1 : 0;
		if (b_accept)
		{
			formctrl_set_field_text(widget, ptd->field, text);
		}
	}
	else if (uid == IDC_FIREWORDS)
	{
		fd.data = firewords_get_data(ptd->editor);
		item = firewords_get_item(ptd->editor);
		if (item)
		{
			editbox_set_text(ptd->editor, get_words_item_text_ptr(item));
		}

		fd.text = editbox_get_text_ptr(ptd->editor);
		b_accept = (noti_form_owner(widget, NC_FIELDCOMMIT, ptd->form, ptd->field, (void*)&fd) == 0) ? 1 : 0;
		if (b_accept)
		{
			formctrl_set_field_text(widget, ptd->field, fd.text);
		}
	}
	else if (uid == IDC_FIREGRID)
	{
		fd.data = firegrid_get_data(ptd->editor);
		fd.item = firegrid_get_item(ptd->editor);

		b_accept = (noti_form_owner(widget, NC_FIELDCOMMIT, ptd->form, ptd->field, (void*)&fd) == 0) ? 1 : 0;
	}
	else if (uid == IDC_TABLEBOX)
	{
		fd.menu = widget_detach_menu(ptd->editor);

		tablectrl_accept(ptd->editor, 1);
		dirty = (get_field_editable(ptd->field)) ? tablectrl_is_update(ptd->editor) : 0;

		data = tablectrl_detach(ptd->editor);
		b_accept = (noti_form_owner(widget, NC_FIELDCOMMIT, ptd->form, ptd->field, (void*)&fd) == 0) ? 1 : 0;
		if (b_accept)
		{
			if (dirty)
			{
				len = string_table_format_options(data, NULL, MAX_LONG, OPT_ITEMFEED, OPT_LINEFEED);
				text = xsalloc(len + 1);
				string_table_format_options(data, text, len, OPT_ITEMFEED, OPT_LINEFEED);

				set_field_text(ptd->field, text, len);
				xsfree(text);

				set_field_dirty(ptd->field, 1);

				noti_form_owner(widget, NC_FIELDUPDATE, ptd->form, ptd->field, NULL);
			}
		}

		destroy_string_table(data);
	}
	else if (uid == IDC_GRIDBOX)
	{
		fd.menu = widget_detach_menu(ptd->editor);

		gridctrl_accept(ptd->editor, 1);
		dirty = (get_field_editable(ptd->field)) ? gridctrl_is_update(ptd->editor) : 0;

		ptd->cur_page = gridctrl_get_cur_page(ptd->editor);

		data = gridctrl_detach(ptd->editor);
		b_accept = (noti_form_owner(widget, NC_FIELDCOMMIT, ptd->form, ptd->field, (void*)&fd) == 0) ? 1 : 0;
		if (b_accept)
		{
			if (dirty)
			{
				set_field_dirty(ptd->field, 1);
				noti_form_owner(widget, NC_FIELDUPDATE, ptd->form, ptd->field, NULL);
			}
		}
	}
	else if (uid == IDC_GRAPHBOX)
	{
		fd.menu = widget_detach_menu(ptd->editor);

		graphctrl_accept(ptd->editor, 1);
		dirty = (get_field_editable(ptd->field)) ? graphctrl_is_update(ptd->editor) : 0;

		ptd->cur_page = graphctrl_get_cur_page(ptd->editor);

		data = graphctrl_detach(ptd->editor);
		b_accept = (noti_form_owner(widget, NC_FIELDCOMMIT, ptd->form, ptd->field, (void*)&fd) == 0) ? 1 : 0;
		if (b_accept)
		{
			if (dirty)
			{
				set_field_dirty(ptd->field, 1);
				noti_form_owner(widget, NC_FIELDUPDATE, ptd->form, ptd->field, NULL);
			}
		}
	}
	else if (uid == IDC_FORMBOX)
	{
		fd.menu = widget_detach_menu(ptd->editor);

		formctrl_accept(ptd->editor, 1);
		dirty = (get_field_editable(ptd->field)) ? formctrl_is_update(ptd->editor) : 0;

		data = formctrl_detach(ptd->editor);
		b_accept = (noti_form_owner(widget, NC_FIELDCOMMIT, ptd->form, ptd->field, (void*)&fd) == 0) ? 1 : 0;
		if (b_accept)
		{
			if (dirty)
			{
				set_field_dirty(ptd->field, 1);
				noti_form_owner(widget, NC_FIELDUPDATE, ptd->form, ptd->field, NULL);
			}
		}
	}
	else if (uid == IDC_TAGBOX)
	{
		fd.menu = widget_detach_menu(ptd->editor);

		dirty = (get_field_editable(ptd->field)) ? tagctrl_get_dirty(ptd->editor) : 0;

		data = tagctrl_detach(ptd->editor);
		b_accept = (noti_form_owner(widget, NC_FIELDCOMMIT, ptd->form, ptd->field, (void*)&fd) == 0) ? 1 : 0;
		if (b_accept)
		{
			if (dirty)
			{
				len = format_tag_doc(data, NULL, MAX_LONG);
				text = xsalloc(len + 1);
				format_tag_doc(data, text, len);

				set_field_text(ptd->field, text, len);
				xsfree(text);

				set_field_dirty(ptd->field, 1);

				noti_form_owner(widget, NC_FIELDUPDATE, ptd->form, ptd->field, NULL);
			}
		}

		destroy_tag_doc(data);
	}
	else if (uid == IDC_MEMOBOX)
	{
		fd.menu = widget_detach_menu(ptd->editor);

		dirty = (get_field_editable(ptd->field)) ? memoctrl_get_dirty(ptd->editor) : 0;

		ptd->cur_page = memoctrl_get_cur_page(ptd->editor);

		data = memoctrl_detach(ptd->editor);
		b_accept = (noti_form_owner(widget, NC_FIELDCOMMIT, ptd->form, ptd->field, (void*)&fd) == 0) ? 1 : 0;
		if (b_accept)
		{
			if (dirty)
			{
				len = format_memo_doc(data, NULL, MAX_LONG);
				text = xsalloc(len + 1);
				format_memo_doc(data, text, len);

				set_field_text(ptd->field, text, len);
				xsfree(text);

				set_field_dirty(ptd->field, 1);

				noti_form_owner(widget, NC_FIELDUPDATE, ptd->form, ptd->field, NULL);
			}
		}

		destroy_memo_doc(data);
	}
	else if (uid == IDC_RICHBOX)
	{
		fd.menu = widget_detach_menu(ptd->editor);

		dirty = (get_field_editable(ptd->field)) ? richctrl_get_dirty(ptd->editor) : 0;

		ptd->cur_page = richctrl_get_cur_page(ptd->editor);

		data = richctrl_detach(ptd->editor);
		b_accept = (noti_form_owner(widget, NC_FIELDCOMMIT, ptd->form, ptd->field, (void*)&fd) == 0) ? 1 : 0;
		if (b_accept)
		{
			if (dirty)
			{
				set_field_dirty(ptd->field, 1);

				noti_form_owner(widget, NC_FIELDUPDATE, ptd->form, ptd->field, NULL);
			}
		}
	}

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);

	if (!b_accept)
		return;

	if (IS_DATA_FIELD(get_field_class_ptr(ptd->field)))
	{
		widget_post_key(widget, KEY_TAB);
	}
}

void noti_form_rollback_edit(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	dword_t uid;
	link_t_ptr data;
	res_win_t editctrl;

	EDITDELTA fd = { 0 };

	if (!widget_is_valid(ptd->editor))
		return;

	XDL_ASSERT(ptd->field);

	set_field_visible(ptd->field, 1);

	uid = widget_get_user_id(ptd->editor);

	if (uid == IDC_FIREWORDS)
	{
		fd.data = firewords_get_data(ptd->editor);
		fd.text = NULL;
		noti_form_owner(widget, NC_FIELDROLLBACK, ptd->form, ptd->field, (void*)&fd);
	}
	else if (uid == IDC_FIREGRID)
	{
		fd.data = firegrid_get_data(ptd->editor);
		fd.item = NULL;
		noti_form_owner(widget, NC_FIELDROLLBACK, ptd->form, ptd->field, (void*)&fd);
	}
	else if (uid == IDC_TABLEBOX)
	{
		fd.menu = widget_detach_menu(ptd->editor);

		tablectrl_accept(ptd->editor, 0);

		data = tablectrl_detach(ptd->editor);

		noti_form_owner(widget, NC_FIELDROLLBACK, ptd->form, ptd->field, (void*)&fd);

		destroy_string_table(data);
	}
	else if (uid == IDC_GRIDBOX)
	{
		fd.menu = widget_detach_menu(ptd->editor);

		gridctrl_accept(ptd->editor, 0);
		ptd->cur_page = gridctrl_get_cur_page(ptd->editor);
		noti_form_owner(widget, NC_FIELDROLLBACK, ptd->form, ptd->field, (void*)&fd);
	}
	else if (uid == IDC_GRAPHBOX)
	{
		fd.menu = widget_detach_menu(ptd->editor);

		graphctrl_accept(ptd->editor, 0);
		ptd->cur_page = graphctrl_get_cur_page(ptd->editor);
		noti_form_owner(widget, NC_FIELDROLLBACK, ptd->form, ptd->field, (void*)&fd);
	}
	else if (uid == IDC_FORMBOX)
	{
		fd.menu = widget_detach_menu(ptd->editor);

		formctrl_accept(ptd->editor, 0);
		noti_form_owner(widget, NC_FIELDROLLBACK, ptd->form, ptd->field, (void*)&fd);
	}
	else if (uid == IDC_MEMOBOX)
	{
		fd.menu = widget_detach_menu(ptd->editor);

		ptd->cur_page = memoctrl_get_cur_page(ptd->editor);

		data = memoctrl_detach(ptd->editor);

		noti_form_owner(widget, NC_FIELDROLLBACK, ptd->form, ptd->field, (void*)&fd);

		destroy_memo_doc(data);
	}
	else if (uid == IDC_TAGBOX)
	{
		fd.menu = widget_detach_menu(ptd->editor);

		data = tagctrl_detach(ptd->editor);

		noti_form_owner(widget, NC_FIELDROLLBACK, ptd->form, ptd->field, (void*)&fd);

		destroy_tag_doc(data);
	}
	else if (uid == IDC_RICHBOX)
	{
		fd.menu = widget_detach_menu(ptd->editor);

		ptd->cur_page = richctrl_get_cur_page(ptd->editor);
		noti_form_owner(widget, NC_FIELDROLLBACK, ptd->form, ptd->field, (void*)&fd);
	}
	else
	{
		noti_form_owner(widget, NC_FIELDROLLBACK, ptd->form, ptd->field, NULL);
	}

	editctrl = ptd->editor;
	ptd->editor = NULL;

	widget_destroy(editctrl);
	widget_set_focus(widget);
}

void noti_form_reset_editor(res_win_t widget, bool_t bCommit)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	if (!widget_is_valid(ptd->editor))
		return;

	if (bCommit)
		noti_form_commit_edit(widget);
	else
		noti_form_rollback_edit(widget);
}

/*******************************************************************************/
int hand_form_create(res_win_t widget, void* data)
{
	form_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (form_delta_t*)xmem_alloc(sizeof(form_delta_t));
	xmem_zero((void*)ptd, sizeof(form_delta_t));

	SETFORMDELTA(widget, ptd);

	ptd->b_lock = 1;
	ptd->stack = create_stack_table();

	return 0;
}

void hand_form_destroy(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	_formctrl_clean(widget);
	destroy_stack_table(ptd->stack);

	xmem_free(ptd);

	SETFORMDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_form_size(res_win_t widget, int code, const xsize_t* prs)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	if (!ptd->form)
		return;

	formctrl_redraw(widget, 1);
}

void hand_form_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	xrect_t xr;

	if (!ptd->form)
		return;

	if (widget_is_valid(ptd->editor))
	{
		if (!IS_AUTO_FIELD(get_field_class_ptr(ptd->field)))
		{
			noti_form_reset_editor(widget, 1);
		}
	}

	if (!widget_hand_scroll(widget, bHorz, nLine))
		return;

	if (widget_is_valid(ptd->editor))
	{
		_formctrl_field_rect(widget, ptd->field, &xr);
		widget_move(ptd->editor, RECTPOINT(&xr));
		widget_update_window(ptd->editor);
		widget_update_client(ptd->editor);
	}
}

void hand_form_mouse_move(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	int nHint;
	link_t_ptr flk;
	bool_t b_design;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->form)
		return;

	b_design = form_is_design(ptd->form);

	if (b_design)
	{
		if (ptd->b_size || ptd->b_drag || ptd->b_group)
		{
			ptd->cur_x = pxp->x;
			ptd->cur_y = pxp->y;

			widget_update(widget, NULL, 0);
			return;
		}
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	widget_get_canv_rect(widget, &cb);

	flk = NULL;
	nHint = calc_form_hint(&cb, &pt, ptd->form, &flk);

	if (b_design)
	{
		if (nHint == FORM_HINT_HORZ_SPLIT && flk == ptd->field && !(dw & MS_WITH_CONTROL))
		{
			if (dw & MS_WITH_LBUTTON)
			{
				noti_form_field_sizing(widget, nHint, pxp->x, pxp->y);
				return;
			}
			else
				widget_set_cursor(widget, CURSOR_SIZENS);
		}
		else if (nHint == FORM_HINT_VERT_SPLIT && flk == ptd->field && !(dw & MS_WITH_CONTROL))
		{
			if (dw & MS_WITH_LBUTTON)
			{
				noti_form_field_sizing(widget, nHint, pxp->x, pxp->y);
				return;
			}
			else
				widget_set_cursor(widget, CURSOR_SIZEWE);
		}
		else if (nHint == FORM_HINT_CROSS_SPLIT && flk == ptd->field && !(dw & MS_WITH_CONTROL))
		{
			if (dw & MS_WITH_LBUTTON)
			{
				noti_form_field_sizing(widget, nHint, pxp->x, pxp->y);
				return;
			}
			else
				widget_set_cursor(widget, CURSOR_SIZEALL);
		}
		else if (nHint == FORM_HINT_FIELD && flk == ptd->field && !(dw & MS_WITH_CONTROL))
		{
			if (dw & MS_WITH_LBUTTON)
			{
				noti_form_field_drag(widget, pxp->x, pxp->y);
				return;
			}
		}
		else if (nHint == FORM_HINT_NONE)
		{
			if (dw & MS_WITH_LBUTTON)
			{
				noti_form_begin_group(widget, pxp->x, pxp->y);
				return;
			}
		}
	}
}

void hand_form_mouse_hover(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	if (!ptd->form)
		return;

	if (ptd->hover)
		noti_form_field_hover(widget, pxp->x, pxp->y);
}

void hand_form_mouse_leave(res_win_t widget, dword_t dw, const xpoint_t* pxp)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	if (!ptd->form)
		return;

	if (ptd->hover)
		noti_form_field_leave(widget);
}

void hand_form_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	int nHint;
	link_t_ptr flk;
	bool_t bRe;
	xpoint_t pt;
	canvbox_t cb;

	bool_t b_design;

	if (!ptd->form)
		return;

	noti_form_reset_editor(widget, 1);

	if (widget_can_focus(widget))
	{
		widget_set_focus(widget);
	}

	b_design = form_is_design(ptd->form);
	
	if (!b_design)
		return;

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	widget_get_canv_rect(widget, &cb);

	flk = NULL;
	nHint = calc_form_hint(&cb, &pt, ptd->form, &flk);
	bRe = (flk == ptd->field) ? 1 : 0;

	switch (nHint)
	{
	case FORM_HINT_FIELD:
		if (widget_key_state(widget, KEY_CONTROL))
		{
			noti_form_field_selected(widget, flk);
		}
		break;
	case FORM_HINT_NONE:
		if (!widget_key_state(widget, KEY_CONTROL))
		{		
			noti_form_reset_select(widget);
		}
		break;
	}
}

void hand_form_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	int nHint;
	link_t_ptr flk;
	bool_t b_design, bRe, bAuto = 0;
	xpoint_t pt;
	canvbox_t cb;

	if (!ptd->form)
		return;

	b_design = form_is_design(ptd->form);

	if (b_design)
	{
		if (ptd->b_size)
		{
			noti_form_field_sized(widget, pxp->x, pxp->y);
			return;
		}

		if (ptd->b_drag)
		{
			noti_form_field_drop(widget, pxp->x, pxp->y);
			return;
		}

		if (ptd->b_group)
		{
			noti_form_end_group(widget, pxp->x, pxp->y);
			return;
		}
	}

	pt.x = pxp->x;
	pt.y = pxp->y;
	widget_point_to_tm(widget, &pt);

	widget_get_canv_rect(widget, &cb);

	flk = NULL;
	nHint = calc_form_hint(&cb, &pt, ptd->form, &flk);

	bRe = (flk == ptd->field) ? 1 : 0;

	if (!b_design && !ptd->b_lock && bRe && flk && get_field_editable(flk))
	{
		widget_post_key(widget, KEY_ENTER);
		return;
	}

	if (ptd->field && !bRe)
	{
		if (!noti_form_field_changing(widget))
			bRe = 1;
	}

	if (!b_design && flk && !get_field_focusable(flk))
		flk = NULL;

	if (flk && !bRe)
	{
		noti_form_field_changed(widget, flk);

		if (IS_AUTO_FIELD(get_field_class_ptr(flk)))
			bAuto = 1;
	}

	noti_form_owner(widget, NC_FORMLBCLK, ptd->form, ptd->field, (void*)pxp);

	if (!b_design && !ptd->b_lock && bAuto && flk && get_field_editable(flk))
	{
		widget_post_key(widget, KEY_ENTER);
	}
}

void hand_form_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	if (!ptd->form)
		return;

	noti_form_reset_editor(widget, 1);

	noti_form_owner(widget, NC_FORMDBCLK, ptd->form, ptd->field, (void*)pxp);
}

void hand_form_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	if (!ptd->form)
		return;

	noti_form_reset_editor(widget, 1);
}

void hand_form_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	if (!ptd->form)
		return;

	noti_form_owner(widget, NC_FORMRBCLK, ptd->form, ptd->field, (void*)pxp);
}

void hand_form_keydown(res_win_t widget, int nKey)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	float x, y, w, h, m;
	bool_t b_design;
	bool_t ks;
	link_t_ptr flk;

	if (!ptd->form)
		return;

	b_design = form_is_design(ptd->form);

	if (b_design)
	{
		if (ptd->field && (nKey == KEY_UP || nKey == KEY_DOWN || nKey == KEY_LEFT || nKey == KEY_RIGHT))
		{
			ks = widget_key_state(widget, KEY_SHIFT);
			m = 1;

			if (ks)
				noti_form_owner(widget, NC_FIELDSIZING, ptd->form, ptd->field, NULL);
			else
				noti_form_owner(widget, NC_FIELDDRAG, ptd->form, ptd->field, NULL);

			_formctrl_done(widget);

			flk = get_next_field(ptd->form, LINK_FIRST);
			while (flk)
			{
				if (flk != ptd->field && !get_field_selected(flk))
				{
					flk = get_next_field(ptd->form, flk);
					continue;
				}

				x = get_field_x(flk);
				y = get_field_y(flk);
				w = get_field_width(flk);
				h = get_field_height(flk);

				switch (nKey)
				{
				case KEY_DOWN:
					if (ks)
						h += m;
					else
						y += m;
					break;
				case KEY_UP:
					if (ks)
						h = (h - m < 0) ? h : h - m;
					else
						y = (y - m < 0) ? y : y - m;
					break;
				case KEY_LEFT:
					if (ks)
						w = (w - m < 0) ? w : w - m;
					else
						x = (x - m < 0) ? x : x - m;
					break;
				case KEY_RIGHT:
					if (ks)
						w += m;
					else
						x += m;
					break;
				}

				set_field_x(flk, x);
				set_field_y(flk, y);
				set_field_width(flk, w);
				set_field_height(flk, h);

				flk = get_next_field(ptd->form, flk);
			}

			widget_update(widget, NULL, 0);

			if (ks)
				noti_form_owner(widget, NC_FIELDSIZED, ptd->form, ptd->field, NULL);
			else
				noti_form_owner(widget, NC_FIELDDROP, ptd->form, ptd->field, NULL);
		}
		else if ((nKey == _T('z') || nKey == _T('Z')) && widget_key_state(widget, KEY_CONTROL))
		{
			widget_undo(widget);
		}
		else if ((nKey == _T('c') || nKey == _T('C')) && widget_key_state(widget, KEY_CONTROL))
		{
			widget_copy(widget);
		}
		else if ((nKey == _T('x') || nKey == _T('X')) && widget_key_state(widget, KEY_CONTROL))
		{
			widget_cut(widget);
		}
		else if ((nKey == _T('v') || nKey == _T('V')) && widget_key_state(widget, KEY_CONTROL))
		{
			widget_paste(widget);
		}
	}
	else
	{
		if (nKey == KEY_ENTER && ptd->field)
		{
			if (widget_can_focus(widget) && !ptd->b_lock)
			{
				noti_form_begin_edit(widget);
			}
		}
		else if (nKey == KEY_TAB)
		{
			formctrl_tabskip(widget,WD_TAB_RIGHT);
		}
		else if (nKey == KEY_LEFT || nKey == KEY_UP) //VK_LEFT VK_UP
		{
			formctrl_tabskip(widget,WD_TAB_LEFT);
		}
		else if (nKey == KEY_RIGHT || nKey == KEY_DOWN) //VK_RIGHT VK_DOWN
		{
			formctrl_tabskip(widget,WD_TAB_RIGHT);
		}
		else if (nKey == KEY_END) //vk_end
		{
			formctrl_move_last_page(widget);
		}
		else if (nKey == KEY_HOME) //vk_home
		{
			formctrl_move_first_page(widget);
		}
		else if (nKey == KEY_PAGEUP) //PAGEUP
		{
			formctrl_move_prev_page(widget);
		}
		else if (nKey == KEY_PAGEDOWN)	//PAGEDOWN
		{
			formctrl_move_next_page(widget);
		}
	}
}

void hand_form_char(res_win_t widget, tchar_t nChar)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	if (!ptd->form)
		return;

	if (form_is_design(ptd->form))
		return;

	if (IS_VISIBLE_CHAR(nChar) && !widget_is_valid(ptd->editor))
	{
		ptd->pch[0] = nChar;
		hand_form_keydown(widget, KEY_ENTER);
	}

	if (IS_VISIBLE_CHAR(nChar) && widget_is_valid(ptd->editor))
	{
		//widget_post_char(NULL, nChar);
	}
}

void hand_form_copy(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	if (!ptd->form)
		return;

	_formctrl_copy(widget);
}

void hand_form_cut(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	if (!ptd->form)
		return;

	_formctrl_done(widget);

	if (!_formctrl_cut(widget))
	{
		_formctrl_discard(widget);
	}
}

void hand_form_paste(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	if (!ptd->form)
		return;

	_formctrl_done(widget);

	if (!_formctrl_paste(widget))
	{
		_formctrl_discard(widget);
	}
}

void hand_form_undo(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	if (!ptd->form)
		return;

	_formctrl_undo(widget);
}

void hand_form_child_command(res_win_t widget, int code, var_long data)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	switch (code)
	{
	case COMMAND_COMMIT:
		noti_form_commit_edit(widget);
		break;
	case COMMAND_ROLLBACK:
		noti_form_rollback_edit(widget);
		break;
	}
}

void hand_form_menu_command(res_win_t widget, int code, int cid, var_long data)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	if (!widget_is_valid(ptd->editor))
		return;

	if (widget_get_user_id(ptd->editor) == cid && code)
	{
		widget_post_command(widget_get_owner(widget), code, IDC_CHILD, (var_long)ptd->editor);
	}
}

void hand_form_notice(res_win_t widget, NOTICE* pnt)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	if (!ptd->form)
		return;

	if (pnt->id == IDC_GRIDBOX)
	{
		noti_form_owner(widget, NC_FIELDGRID, ptd->form, ptd->field, (void*)pnt);
	}
	else if (pnt->id == IDC_GRAPHBOX)
	{
		noti_form_owner(widget, NC_FIELDGRAPH, ptd->form, ptd->field, (void*)pnt);
	}
	else if (pnt->id == IDC_IMAGESBOX)
	{
		noti_form_owner(widget, NC_FIELDIMAGES, ptd->form, ptd->field, (void*)pnt);
	}
	else if (pnt->id == IDC_FORMBOX)
	{
		noti_form_owner(widget, NC_FIELDFORM, ptd->form, ptd->field, (void*)pnt);
	}
	else if (pnt->id == IDC_PHOTOBOX)
	{
		noti_form_owner(widget, NC_FIELDPHOTO, ptd->form, ptd->field, (void*)pnt);
	}
	else if (pnt->id == IDC_RICHBOX)
	{
		noti_form_owner(widget, NC_FIELDRICH, ptd->form, ptd->field, (void*)pnt);
	}else if (pnt->id == IDC_RICHBOX)
	{
		noti_form_owner(widget, NC_FIELDRICH, ptd->form, ptd->field, (void*)pnt);
	}
	else if (pnt->id == IDC_TAGBOX)
	{
		noti_form_owner(widget, NC_FIELDTAG, ptd->form, ptd->field, (void*)pnt);
	}
	else if (pnt->id == IDC_MEMOBOX)
	{
		noti_form_owner(widget, NC_FIELDMEMO, ptd->form, ptd->field, (void*)pnt);
	}
}


void hand_form_erase(res_win_t widget, res_ctx_t dc)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	if (!ptd->form)
		return;
}

void hand_form_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	bool_t b_design;
	xrect_t xr = { 0 };
	xfont_t xf = { 0 };
	xbrush_t xb = { 0 };
	xpen_t xp = { 0 };
	xcolor_t xc = { 0 };
	res_ctx_t rdc;
	link_t_ptr flk;

	canvas_t canv;
	if_canvas_t* pif;
	canvbox_t cb;

	if (!ptd->form)
		return;

	b_design = form_is_design(ptd->form);

	widget_get_xfont(widget, &xf);
	widget_get_xbrush(widget, &xb);
	widget_get_xpen(widget, &xp);

	canv = widget_get_canvas(widget);
	pif = create_canvas_interface(canv);

	parse_xcolor(&pif->clr_bkg, xb.color);
	parse_xcolor(&pif->clr_frg, xp.color);
	parse_xcolor(&pif->clr_txt, xf.color);
	widget_get_xcolor(widget, &pif->clr_msk);

	widget_get_client_rect(widget, &xr);

	rdc = begin_canvas_paint(pif->canvas, dc, xr.w, xr.h);

	widget_get_xbrush(widget, &xb);

	widget_get_xpen(widget, &xp);

	draw_rect_raw(rdc, NULL, &xb, &xr);

	widget_get_canv_rect(widget, &cb);

	if (widget_can_paging(widget))
	{
		if (!b_design)
		{
			parse_xcolor(&xc, xb.color);
			lighten_xcolor(&xc, DEF_SOFT_DARKEN);
			draw_shadow(pif->canvas, &xc, (const xrect_t*)&cb);
		}

		parse_xcolor(&xc, xb.color);
		lighten_xcolor(&xc, DEF_SOFT_DARKEN);
		draw_corner(canv, &xc, (const xrect_t*)&cb);

		if (b_design)
		{
			parse_xcolor(&xc, xp.color);
			lighten_xcolor(&xc, DEF_SOFT_DARKEN);
			draw_ruler(pif->canvas, &xc, (const xrect_t*)&cb);
		}
	}

	draw_form_page(pif, &cb, ptd->form, ptd->cur_page);

	//draw focus
	if (ptd->field)
	{
		_formctrl_field_rect(widget, ptd->field, &xr);

		if (ptd->b_lock)
		{
			parse_xcolor(&xc, DEF_DISABLE_COLOR);
		}
		else
		{
			if (ptd->b_alarm)
			{
				parse_xcolor(&xc, DEF_ALARM_COLOR);
			}
			else
			{
				if (get_field_editable(ptd->field))
					parse_xcolor(&xc, DEF_ENABLE_COLOR);
				else
					parse_xcolor(&xc, DEF_DISABLE_COLOR);
			}
		}
		draw_feed_raw(rdc, &xc, &xr, ALPHA_SOLID);
	}

	if (b_design)
	{
		//draw check
		parse_xcolor(&xc, DEF_ALPHA_COLOR);

		flk = get_next_field(ptd->form, LINK_FIRST);
		while (flk)
		{
			if (get_field_selected(flk))
			{
				_formctrl_field_rect(widget, flk, &xr);
				pt_expand_rect(&xr, DEF_INNER_FEED, DEF_INNER_FEED);

				alphablend_rect_raw(rdc, &xc, &xr, ALPHA_TRANS);
			}
			flk = get_next_field(ptd->form, flk);
		}

		if (ptd->b_drag)
		{
			xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

			_formctrl_group_rect(widget, ptd->field, &xr);

			xr.x += (ptd->cur_x - ptd->org_x);
			xr.y += (ptd->cur_y - ptd->org_y);

			draw_rect_raw(rdc, &xp, NULL, &xr);
		}
		else if (ptd->b_size)
		{
			xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

			_formctrl_field_rect(widget, ptd->field, &xr);

			if (ptd->org_hint == FORM_HINT_VERT_SPLIT)
			{
				xr.w = (ptd->cur_x - xr.x);
			}
			else if (ptd->org_hint == FORM_HINT_HORZ_SPLIT)
			{
				xr.h = (ptd->cur_y - xr.y);
			}
			else
			{
				xr.w = (ptd->cur_x - xr.x);
				xr.h = (ptd->cur_y - xr.y);
			}

			draw_rect_raw(rdc, &xp, NULL, &xr);
		}
		else if (ptd->b_group)
		{
			xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

			xr.x = ptd->org_x;
			xr.w = ptd->cur_x - ptd->org_x;
			xr.y = ptd->org_y;
			xr.h = ptd->cur_y - ptd->org_y;

			draw_rect_raw(rdc, &xp, NULL, &xr);
		}

		if (ptd->field && get_field_group(ptd->field))
		{
			xscpy(xp.style, GDI_ATTR_STROKE_STYLE_DASHED);

			_formctrl_group_rect(widget, ptd->field, &xr);

			draw_rect_raw(rdc, &xp, NULL, &xr);
		}
	}

	end_canvas_paint(pif->canvas, dc, pxr);
	destroy_canvas_interface(pif);
}

/***********************************************function********************************************************/

res_win_t formctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_form_create)
		EVENT_ON_DESTROY(hand_form_destroy)

		EVENT_ON_ERASE(hand_form_erase)
		EVENT_ON_PAINT(hand_form_paint)

		EVENT_ON_SIZE(hand_form_size)

		EVENT_ON_SCROLL(hand_form_scroll)

		EVENT_ON_KEYDOWN(hand_form_keydown)
		EVENT_ON_CHAR(hand_form_char)

		EVENT_ON_MOUSE_MOVE(hand_form_mouse_move)
		EVENT_ON_MOUSE_HOVER(hand_form_mouse_hover)
		EVENT_ON_MOUSE_LEAVE(hand_form_mouse_leave)

		EVENT_ON_LBUTTON_DBCLICK(hand_form_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_form_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_form_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_form_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_form_rbutton_up)

		EVENT_ON_NOTICE(hand_form_notice)
		EVENT_ON_CHILD_COMMAND(hand_form_child_command)
		EVENT_ON_MENU_COMMAND(hand_form_menu_command)

		EVENT_ON_COPY(hand_form_copy)
		EVENT_ON_CUT(hand_form_cut)
		EVENT_ON_PASTE(hand_form_paste)
		EVENT_ON_UNDO(hand_form_undo)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	return widget_create(wname, wstyle, pxr, wparent, &ev);
}

void formctrl_attach(res_win_t widget, link_t_ptr ptr)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(ptr && is_form_doc(ptr));

	noti_form_reset_editor(widget, 0);

	ptd->form = ptr;
	ptd->field = NULL;
	ptd->cur_page = 1;

	formctrl_redraw(widget, 1);
}

link_t_ptr formctrl_detach(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	link_t_ptr data;

	XDL_ASSERT(ptd != NULL);

	noti_form_reset_editor(widget, 0);

	data = ptd->form;
	ptd->form = NULL;
	ptd->field = NULL;
	ptd->cur_page = 0;

	widget_update(widget, NULL, 0);

	return data;
}

link_t_ptr formctrl_fetch(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->form;
}

res_win_t formctrl_get_editor(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	return ptd->editor;
}

bool_t formctrl_verify(res_win_t widget, bool_t bAlarm)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	link_t_ptr flk;
	int code;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->form)
		return 1;

	noti_form_reset_editor(widget, (bool_t)0);

	code = verify_form_doc(ptd->form, &flk);
	if (veValid != code)
	{
		ptd->b_alarm = (bool_t)1;

		formctrl_set_focus_field(widget, flk);
		
		return (bool_t)0;
	}
	else
		return (bool_t)1;
}

void formctrl_accept(res_win_t widget, bool_t bAccept)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->form)
		return;

	noti_form_reset_editor(widget, bAccept);
}

void formctrl_redraw(res_win_t widget, bool_t bCalc)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	link_t_ptr flk;
	bool_t b_valid;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->form)
		return;

	noti_form_reset_editor(widget, 0);

	if (bCalc)
	{
		calc_form_doc(ptd->form);
	}

	b_valid = 0;
	flk = get_next_field(ptd->form, LINK_FIRST);
	while (flk)
	{
		if (flk == ptd->field)
			b_valid = 1;

		noti_form_owner(widget, NC_FIELDCALCED, ptd->form, flk, NULL);

		flk = get_next_field(ptd->form, flk);
	}
	
	noti_form_owner(widget, NC_FORMCALCED, ptd->form, NULL, NULL);

	if (!b_valid)
	{
		ptd->field = NULL;
	}
	ptd->hover = NULL;

	_formctrl_reset_page(widget);

	if (bCalc)
	{
		ptd->max_page = calc_form_pages(widget_get_canvas(widget), ptd->form);
	}

	widget_update_window(widget);

	widget_update(widget, NULL, 0);
}

void formctrl_redraw_field(res_win_t widget, link_t_ptr flk, bool_t bCalc)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->form)
		return;

#ifdef _DEBUG
	if(!is_form_field(ptd->form, flk))
		return;
#endif

	if (bCalc)
	{
		calc_form_field(ptd->form, flk);
	}
	
	noti_form_owner(widget, NC_FIELDCALCED, ptd->form, flk, NULL);

	_formctrl_field_rect(widget, flk, &xr);
	pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);

	widget_update(widget, &xr, 0);
}

void formctrl_tabskip(res_win_t widget, int nSkip)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	link_t_ptr flk = NULL;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->form)
		return;

	noti_form_reset_editor(widget, (bool_t)1);

	switch (nSkip)
	{
	case WD_TAB_LEFT:
	case WD_TAB_UP:
		if (ptd->field == NULL)
			flk = get_prev_focusable_field(ptd->form, LINK_LAST);
		else
			flk = get_prev_focusable_field(ptd->form, ptd->field);
		break;
	case WD_TAB_RIGHT:
	case WD_TAB_DOWN:
		if (ptd->field == NULL)
			flk = get_next_focusable_field(ptd->form, LINK_FIRST);
		else
			flk = get_next_focusable_field(ptd->form, ptd->field);
		break;
	case WD_TAB_HOME:
		flk = get_next_focusable_field(ptd->form, LINK_FIRST);
		break;
	case WD_TAB_END:
		flk = get_prev_focusable_field(ptd->form, LINK_LAST);
		break;
	}

	formctrl_set_focus_field(widget, flk);
}

bool_t formctrl_set_focus_field(res_win_t widget, link_t_ptr flk)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	bool_t bRe;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->form)
		return 0;

	if (flk == LINK_FIRST)
		flk = get_next_focusable_field(ptd->form, LINK_FIRST);
	else if (flk == LINK_LAST)
		flk = get_prev_focusable_field(ptd->form, LINK_LAST);

	bRe = (flk == ptd->field) ? (bool_t)1 : (bool_t)0;
	if (bRe)
		return (bool_t)1;

	if (ptd->field && !bRe)
	{
		if (!noti_form_field_changing(widget))
			return (bool_t)0;
	}

	if (flk && !bRe)
	{
		noti_form_field_changed(widget, flk);

		_formctrl_ensure_visible(widget);
	}

	return (bool_t)1;
}

link_t_ptr formctrl_get_focus_field(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->form)
		return NULL;

	return ptd->field;
}

void formctrl_move_first_page(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->form)
		return;

	if (ptd->cur_page != 1)
	{
		noti_form_reset_editor(widget, (bool_t)1);

		ptd->cur_page = 1;

		widget_update(widget, NULL, 0);
	}
}

void formctrl_move_last_page(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->form)
		return;

	if (ptd->cur_page != ptd->max_page)
	{
		noti_form_reset_editor(widget, (bool_t)1);

		ptd->cur_page = ptd->max_page;
		widget_update(widget, NULL, 0);
	}
}

void formctrl_move_next_page(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->form)
		return;

	if (ptd->cur_page < ptd->max_page)
	{
		noti_form_reset_editor(widget, (bool_t)1);

		ptd->cur_page++;

		widget_update(widget, NULL, 0);
	}
}

void formctrl_move_prev_page(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->form)
		return;

	if (ptd->cur_page > 1)
	{
		noti_form_reset_editor(widget, (bool_t)1);

		ptd->cur_page--;

		widget_update(widget, NULL, 0);
	}
}

int formctrl_get_cur_page(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->form)
		return 0;

	return ptd->cur_page;
}

int formctrl_get_max_page(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->form)
		return 0;

	return calc_form_pages(widget_get_canvas(widget), ptd->form);
}

void formctrl_move_to_page(res_win_t widget, int page)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->form)
		return;

	if (page > ptd->max_page || page < 1)
		return;

	noti_form_reset_editor(widget, (bool_t)1);

	ptd->cur_page = page;

	widget_update(widget, NULL, 0);
}

bool_t formctrl_set_field_text(res_win_t widget, link_t_ptr flk, const tchar_t* szText)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	xrect_t xr;
	const tchar_t* text;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->form)
		return 0;

#ifdef _DEBUG
	XDL_ASSERT(is_form_field(ptd->form, flk));
#endif

	if (!IS_DATA_FIELD(get_field_class_ptr(flk)))
		return 0;

	text = get_field_text_ptr(flk);
	if (compare_data(szText, text, get_field_data_type_ptr(flk)) != 0)
	{
		if (veValid == verify_text(szText, get_field_data_type_ptr(flk), get_field_nullable(flk), get_field_data_len(flk), get_field_data_min_ptr(flk), get_field_data_max_ptr(flk)))
		{
			set_field_text(flk, szText, -1);
			set_field_dirty(flk, 1);

			noti_form_owner(widget, NC_FIELDUPDATE, ptd->form, flk, NULL);

			formctrl_get_field_rect(widget, flk, &xr);
			pt_expand_rect(&xr, DEF_OUTER_FEED, DEF_OUTER_FEED);
			widget_update(widget, &xr, 0);

			if (get_field_fireable(flk))
			{
				if (calc_form_doc(ptd->form))
				{
					widget_update(widget, NULL, 0);
				}
			}

			return 1;
		}
	}

	return 0;
}

bool_t formctrl_is_update(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return get_update_field_count(ptd->form) ? 1 : 0;
}

void formctrl_get_field_rect(res_win_t widget, link_t_ptr flk, xrect_t* pxr)
{
	form_delta_t* ptd = GETFORMDELTA(widget);
	
	XDL_ASSERT(ptd != NULL);

	if (!ptd->form)
		return;

#ifdef _DEBUG
	XDL_ASSERT(is_form_field(ptd->form, flk));
#endif

	_formctrl_field_rect(widget, flk, pxr);
}

bool_t formctrl_get_lock(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->b_lock;
}

void formctrl_set_lock(res_win_t widget, bool_t bLock)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_lock = bLock;
}

bool_t formctrl_get_dirty(res_win_t widget)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->form)
		return 0;

	if (!form_is_design(ptd->form))
		return 0;

	return (peek_stack_node(ptd->stack, -1)) ? 1 : 0;
}

void formctrl_set_dirty(res_win_t widget, bool_t bDirty)
{
	form_delta_t* ptd = GETFORMDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->form)
		return;

	if (!form_is_design(ptd->form))
		return ;

	if (bDirty)
		_formctrl_done(widget);
	else
		_formctrl_clean(widget);
}
