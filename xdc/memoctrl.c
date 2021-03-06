﻿/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc text control document

	@module	memoctrl.c | text control widnows implement file

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
#include "xdcmenu.h"
#include "textor.h"

typedef struct _memoctrl_delta_t{
	textor_t textor;
	link_t_ptr line;

	bool_t b_lock;

	int chs;
	tchar_t pch[CHS_LEN + 1];
}memoctrl_delta_t;

#define GETMEMOCTRLDELTA(ph) 	(memoctrl_delta_t*)widget_get_user_delta(ph)
#define SETMEMOCTRLDELTA(ph,ptd) widget_set_user_delta(ph,(var_long)ptd)

static int _memoctrl_get_text(void* data, tchar_t* buf, int max)
{
	link_t_ptr text = (link_t_ptr)data;

	return format_memo_doc(text, buf, max);
}

static void _memoctrl_set_text(void* data, const tchar_t* buf, int len)
{
	link_t_ptr text = (link_t_ptr)data;

	parse_memo_doc(text, buf, len);
}

static bool_t _memoctrl_get_paging(res_win_t widget, xsize_t* pse)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);
	xrect_t xr;

	XDL_ASSERT(ptd && ptd->textor.data);

	if (widget_get_style(widget) & WD_STYLE_PAGING)
	{
		pse->fx = get_memo_width((link_t_ptr)(ptd->textor.data));
		pse->fy = get_memo_height((link_t_ptr)(ptd->textor.data));

		widget_size_to_pt(widget, pse);

		return 1;
	}else
	{
		widget_get_client_rect(widget, &xr);

		pse->cx = xr.w;
		pse->cy = xr.h;

		return 0;
	}
}

/********************************************************************************************/
int noti_memoctrl_owner(res_win_t widget, unsigned long code, link_t_ptr ptr, link_t_ptr nlk, void* data)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);
	NOTICE_MEMO nf = { 0 };

	nf.widget = widget;
	nf.id = widget_get_user_id(widget);
	nf.code = code;
	nf.data = data;
	nf.ret = 0;

	nf.memo = ptr;
	nf.line = nlk;

	widget_send_notice(widget_get_owner(widget), (LPNOTICE)&nf);

	return nf.ret;
}

/********************************************************************************************/
int hand_memoctrl_create(res_win_t widget, void* data)
{
	memoctrl_delta_t* ptd;

	widget_hand_create(widget);

	ptd = (memoctrl_delta_t*)xmem_alloc(sizeof(memoctrl_delta_t));
	xmem_zero((void*)ptd, sizeof(memoctrl_delta_t));

	SETMEMOCTRLDELTA(widget, ptd);

	ptd->textor.widget = widget;
	ptd->textor.dc = widget_client_ctx(widget);
	ptd->textor.data = NULL;
	ptd->textor.pf_scan_text = (PF_SCAN_TEXT)scan_memo_text;
	ptd->textor.pf_get_text = _memoctrl_get_text;
	ptd->textor.pf_set_text = _memoctrl_set_text;
	ptd->textor.pf_get_paging = _memoctrl_get_paging;
	ptd->textor.max_undo = 1024;

	ptd->b_lock = 1;

	return 0;
}

void hand_memoctrl_destroy(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	hand_textor_clean(&ptd->textor);

	widget_release_ctx(widget, ptd->textor.dc);

	xmem_free(ptd);

	SETMEMOCTRLDELTA(widget, 0);

	widget_hand_destroy(widget);
}

void hand_memoctrl_set_focus(res_win_t widget, res_win_t wt)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_set_focus(&ptd->textor, wt);
}

void hand_memoctrl_kill_focus(res_win_t widget, res_win_t wt)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_kill_focus(&ptd->textor, wt);

	if (widget_is_editor(widget))
	{
		if (memoctrl_get_dirty(widget))
			widget_send_command(widget_get_owner(widget), COMMAND_COMMIT, IDC_CHILD, (var_long)NULL);
		else
			widget_send_command(widget_get_owner(widget), COMMAND_ROLLBACK, IDC_CHILD, (var_long)NULL);
	}
}

void hand_memoctrl_keydown(res_win_t widget, int key)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);
	xface_t xa;

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	widget_get_xface(widget, &xa);

	switch (key)
	{
	case KEY_BACK:
		if (ptd->b_lock)
			break;

		if (_TEXTOR_PRESS_ACCEPT == hand_textor_back(&ptd->textor))
		{
			widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, (var_long)NULL);

			if (ptd->line != (link_t_ptr)ptd->textor.object)
			{
				ptd->line = (link_t_ptr)ptd->textor.object;
				widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
			}
		}
		break;
	case KEY_DELETE:
		if (ptd->b_lock)
			break;

		if (_TEXTOR_PRESS_ACCEPT == hand_textor_delete(&ptd->textor))
		{
			widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, (var_long)NULL);

			if (ptd->line != (link_t_ptr)ptd->textor.object)
			{
				ptd->line = (link_t_ptr)ptd->textor.object;
				widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
			}
		}
		break;
	case KEY_TAB:
		break;
	case KEY_ENTER:
		break;
	case KEY_ESC:
		if (ptd->b_lock)
			break;

		hand_textor_escape(&ptd->textor);
		break;
	case KEY_LEFT:
		hand_textor_left(&ptd->textor);

		if (ptd->line != (link_t_ptr)ptd->textor.object)
		{
			ptd->line = (link_t_ptr)ptd->textor.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
		}
		break;
	case KEY_RIGHT:
		hand_textor_right(&ptd->textor);

		if (ptd->line != (link_t_ptr)ptd->textor.object)
		{
			ptd->line = (link_t_ptr)ptd->textor.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
		}
		break;
	case KEY_UP:
		hand_textor_up(&ptd->textor);

		if (ptd->line != (link_t_ptr)ptd->textor.object)
		{
			ptd->line = (link_t_ptr)ptd->textor.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
		}
		break;
	case KEY_DOWN:
		hand_textor_down(&ptd->textor);

		if (ptd->line != (link_t_ptr)ptd->textor.object)
		{
			ptd->line = (link_t_ptr)ptd->textor.object;
			widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
		}
		break;
	case KEY_PAGEDOWN:
		hand_textor_move_next_page(&ptd->textor);
		break;
	case KEY_PAGEUP:
		hand_textor_move_prev_page(&ptd->textor);
		break;
	case KEY_HOME:
		hand_textor_move_first_page(&ptd->textor);
		break;
	case KEY_END:
		hand_textor_move_last_page(&ptd->textor);
		break;
	case _T('c'):
	case _T('C'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			widget_copy(widget);
		}
		break;
	case _T('x'):
	case _T('X'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			widget_cut(widget);
		}
		break;
	case _T('v'):
	case _T('V'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			widget_paste(widget);
		}
		break;
	case _T('z'):
	case _T('Z'):
		if (widget_key_state(widget, KEY_CONTROL))
		{
			widget_undo(widget);
		}
		break;
	}
}

void hand_memoctrl_char(res_win_t widget, tchar_t ch)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	if (!ptd)
		return;

	if (ptd->b_lock)
		return;

	if (!ptd->textor.data)
		return;

	if (ch == KEY_BACK)
		return;

	if (ch != KEY_ENTER && ch != KEY_TAB && ch > 0 && ch < 32)
		return;

	if (!ptd->chs)
	{
		ptd->chs = xschs(&ch);

		xsncpy(ptd->pch, &ch, 1);
		ptd->chs--;

		if (ptd->chs)
			return;
	}
	else
	{
		xsncat(ptd->pch, &ch, 1);
		ptd->chs--;

		if (ptd->chs)
			return;
	}

	if (_TEXTOR_PRESS_ACCEPT == hand_textor_word(&ptd->textor, ptd->pch))
	{
		widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, (var_long)NULL);
	}
}

void hand_memoctrl_copy(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_copy(&ptd->textor);
}

void hand_memoctrl_cut(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	if (!ptd)
		return;

	if (ptd->b_lock)
		return;

	if (!ptd->textor.data)
		return;

	if (hand_textor_cut(&ptd->textor) != _TEXTOR_PRESS_ACCEPT)
		return;

	widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, (var_long)NULL);

	if (ptd->line != (link_t_ptr)ptd->textor.object)
	{
		ptd->line = (link_t_ptr)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
	}
}

void hand_memoctrl_paste(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	if (!ptd)
		return;

	if (ptd->b_lock)
		return;

	if (!ptd->textor.data)
		return;

	if (hand_textor_paste(&ptd->textor) != _TEXTOR_PRESS_ACCEPT)
		return;

	widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, (var_long)NULL);

	if (ptd->line != (link_t_ptr)ptd->textor.object)
	{
		ptd->line = (link_t_ptr)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
	}
}

void hand_memoctrl_undo(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	if (!ptd)
		return;

	if (ptd->b_lock)
		return;

	if (!ptd->textor.data)
		return;

	if (hand_textor_undo(&ptd->textor) != _TEXTOR_PRESS_ACCEPT)
		return;

	widget_post_command(widget, COMMAND_UPDATE, IDC_SELF, (var_long)NULL);

	if (ptd->line != (link_t_ptr)ptd->textor.object)
	{
		ptd->line = (link_t_ptr)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
	}
}

void hand_memoctrl_lbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);
	
	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_lbutton_down(&ptd->textor, pxp);
}

void hand_memoctrl_lbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_lbutton_up(&ptd->textor, pxp);

	if (ptd->line != (link_t_ptr)ptd->textor.object)
	{
		ptd->line = (link_t_ptr)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
	}
}

void hand_memoctrl_lbutton_dbclick(res_win_t widget, const xpoint_t* pxp)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	if (ptd->textor.object)
	{
		hand_textor_selectobj(&ptd->textor);
	}
}

void hand_memoctrl_rbutton_down(res_win_t widget, const xpoint_t* pxp)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

}

void hand_memoctrl_rbutton_up(res_win_t widget, const xpoint_t* pxp)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);
	xpoint_t xp;

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	xp.x = pxp->x;
	xp.y = pxp->y;
	widget_client_to_screen(widget, &xp);

	textor_menu(widget, &xp, WD_LAYOUT_LEFTBOTTOM);
}

void hand_memoctrl_mousemove(res_win_t widget, dword_t mk, const xpoint_t* ppt)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_mousemove(&ptd->textor, mk, ppt);
}

void hand_memoctrl_size(res_win_t widget, int code, const xsize_t* prs)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_size(&ptd->textor, code, prs);
}

void hand_memoctrl_scroll(res_win_t widget, bool_t bHorz, long nLine)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_scroll(&ptd->textor, bHorz, nLine);
}

void hand_memoctrl_wheel(res_win_t widget, bool_t bHorz, long nDelta)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);
	scroll_t scr = { 0 };
	long nLine;
	res_win_t win;

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	widget_get_scroll(widget, bHorz, &scr);

	if (bHorz)
		nLine = (nDelta > 0) ? scr.min : -scr.min;
	else
		nLine = (nDelta < 0) ? scr.min : -scr.min;

	if (hand_textor_scroll(&ptd->textor, bHorz, nLine))
		return;

	win = widget_get_parent(widget);

	if (widget_is_valid(win))
	{
		widget_scroll(win, bHorz, nLine);
	}
}

void hand_memoctrl_self_command(res_win_t widget, int code, var_long data)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	if (code == COMMAND_UPDATE)
	{
		noti_memoctrl_owner(widget, NC_MEMOLINEUPDATE, (link_t_ptr)ptd->textor.data, ptd->line, NULL);
	}
	else if (code == COMMAND_CHANGE)
	{
		noti_memoctrl_owner(widget, NC_MEMOLINECHANGED, (link_t_ptr)ptd->textor.data, ptd->line, NULL);
	}
}

void hand_memoctrl_menu_command(res_win_t widget, int code, int cid, var_long data)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	if (cid == IDC_EDITMENU)
	{
		switch (code)
		{
		case COMMAND_COPY:
			widget_copy(widget);
			break;
		case COMMAND_CUT:
			widget_cut(widget);
			break;
		case COMMAND_PASTE:
			widget_paste(widget);
			break;
		case COMMAND_UNDO:
			widget_undo(widget);
			break;
		}

		widget_close((res_win_t)data, 1);
	}
}

void hand_memoctrl_erase(res_win_t widget, res_ctx_t dc)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;
}

void hand_memoctrl_paint(res_win_t widget, res_ctx_t dc, const xrect_t* pxr)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	if (!ptd)
		return;

	if (!ptd->textor.data)
		return;

	hand_textor_paint(&ptd->textor, dc, pxr);
}

/************************************************************************************************/
res_win_t memoctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent)
{
	if_event_t ev = { 0 };
	res_win_t wt;
	xface_t xa;

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(hand_memoctrl_create)
		EVENT_ON_DESTROY(hand_memoctrl_destroy)

		EVENT_ON_ERASE(hand_memoctrl_erase)
		EVENT_ON_PAINT(hand_memoctrl_paint)

		EVENT_ON_SIZE(hand_memoctrl_size)
		EVENT_ON_SCROLL(hand_memoctrl_scroll)
		EVENT_ON_WHEEL(hand_memoctrl_wheel)

		EVENT_ON_KEYDOWN(hand_memoctrl_keydown)
		EVENT_ON_CHAR(hand_memoctrl_char)

		EVENT_ON_MOUSE_MOVE(hand_memoctrl_mousemove)
		EVENT_ON_LBUTTON_DBCLICK(hand_memoctrl_lbutton_dbclick)
		EVENT_ON_LBUTTON_DOWN(hand_memoctrl_lbutton_down)
		EVENT_ON_LBUTTON_UP(hand_memoctrl_lbutton_up)
		EVENT_ON_RBUTTON_DOWN(hand_memoctrl_rbutton_down)
		EVENT_ON_RBUTTON_UP(hand_memoctrl_rbutton_up)

		EVENT_ON_SELF_COMMAND(hand_memoctrl_self_command)
		EVENT_ON_MENU_COMMAND(hand_memoctrl_menu_command)

		EVENT_ON_SET_FOCUS(hand_memoctrl_set_focus)
		EVENT_ON_KILL_FOCUS(hand_memoctrl_kill_focus)

		EVENT_ON_COPY(hand_memoctrl_copy)
		EVENT_ON_CUT(hand_memoctrl_cut)
		EVENT_ON_PASTE(hand_memoctrl_paste)
		EVENT_ON_UNDO(hand_memoctrl_undo)

		EVENT_ON_NC_IMPLEMENT

	EVENT_END_DISPATH

	wt = widget_create(wname, wstyle, pxr, wparent, &ev);
	if (!wt)
		return NULL;

	widget_get_xface(wt, &xa);
	xscpy(xa.text_wrap, GDI_ATTR_TEXT_WRAP_WORDBREAK);
	widget_set_xface(wt, &xa);

	return wt;
}

void memoctrl_attach(res_win_t widget, link_t_ptr data)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	XDL_ASSERT(data && is_memo_doc(data));

	ptd->textor.data = (void*)data;
	ptd->textor.object = NULL;
	ptd->textor.page = 1;

	ptd->line = NULL;

	memoctrl_redraw(widget);

	widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
}

link_t_ptr memoctrl_fetch(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return (link_t_ptr)ptd->textor.data;
}

link_t_ptr memoctrl_detach(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);
	link_t_ptr ptr;

	XDL_ASSERT(ptd != NULL);

	ptr = (link_t_ptr)ptd->textor.data;
	ptd->textor.data = NULL;
	ptd->textor.object = NULL;
	ptd->textor.page = 0;

	ptd->line = NULL;

	return ptr;
}

link_t_ptr memoctrl_get_focus_line(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);
	link_t_ptr nlk = NULL;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return NULL;

	return ptd->line;
}

void memoctrl_set_focus_line(res_win_t widget, link_t_ptr nlk)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_findobj(&ptd->textor, nlk);

	if (ptd->line != (link_t_ptr)ptd->textor.object)
	{
		ptd->line = (link_t_ptr)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
	}
}

void memoctrl_delete_line(res_win_t widget, link_t_ptr nlk)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	if (ptd->b_lock)
		return;

	hand_textor_done(&ptd->textor);

	delete_memo_line(nlk);

	memoctrl_redraw(widget);
}

link_t_ptr memoctrl_insert_line(res_win_t widget, link_t_ptr pos)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);
	link_t_ptr nlk;

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return NULL;

	if (ptd->b_lock)
		return NULL;

	hand_textor_done(&ptd->textor);

	nlk = insert_memo_line((link_t_ptr)ptd->textor.data, pos);

	memoctrl_redraw(widget);

	return nlk;
}

void memoctrl_set_line_text(res_win_t widget, link_t_ptr nlk, const tchar_t* token, int len)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	if (ptd->b_lock)
		return;

	hand_textor_done(&ptd->textor);

	set_memo_line_text(nlk, token, len);

	memoctrl_redraw(widget);
}

void memoctrl_set_text(res_win_t widget, const tchar_t* token, int len)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	if (ptd->b_lock)
		return;

	hand_textor_done(&ptd->textor);

	clear_memo_doc(ptd->textor.data);

	parse_memo_doc(ptd->textor.data, token, len);

	memoctrl_redraw(widget);
}

int memoctrl_get_text(res_win_t widget, tchar_t* buf, int max)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return 0;

	return format_memo_doc(ptd->textor.data, buf, max);
}

void memoctrl_get_line_rect(res_win_t widget, link_t_ptr nlk, xrect_t* pxr)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	textor_object_rect(&ptd->textor, nlk, pxr);
}

void memoctrl_redraw(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_redraw(&ptd->textor);

	if (ptd->line != (link_t_ptr)ptd->textor.object)
	{
		ptd->line = (link_t_ptr)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
	}
}

void memoctrl_select_cur(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_selectobj(&ptd->textor);

	if (ptd->line != (link_t_ptr)ptd->textor.object)
	{
		ptd->line = (link_t_ptr)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
	}
}

void memoctrl_select_all(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_selectall(&ptd->textor);

	if (ptd->line != (link_t_ptr)ptd->textor.object)
	{
		ptd->line = (link_t_ptr)ptd->textor.object;
		widget_post_command(widget, COMMAND_CHANGE, IDC_SELF, (var_long)NULL);
	}
}

int memoctrl_get_selected_text(res_win_t widget, tchar_t* buf, int max)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return 0;

	return hand_textor_selected_text(&ptd->textor, buf, max);
}

bool_t memoctrl_get_dirty(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return 0;

	return (ptd->textor.ptu != NULL) ? 1 : 0;
}

void memoctrl_set_dirty(res_win_t widget, bool_t bDirty)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return ;

	if (bDirty)
		hand_textor_done(&ptd->textor);
	else
		hand_textor_clean(&ptd->textor);
}

void memoctrl_move_to_page(res_win_t widget, int page)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_move_to_page(&ptd->textor, page);
}

void memoctrl_move_first_page(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_move_first_page(&ptd->textor);
}

void memoctrl_move_last_page(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_move_last_page(&ptd->textor);
}

void memoctrl_move_next_page(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_move_next_page(&ptd->textor);
}

void memoctrl_move_prev_page(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return;

	hand_textor_move_prev_page(&ptd->textor);
}

int memoctrl_get_cur_page(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return 0;

	return textor_cur_page(&ptd->textor);
}

int memoctrl_get_max_page(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	if (!ptd->textor.data)
		return 0;

	return textor_max_page(&ptd->textor);
}

bool_t memoctrl_get_lock(res_win_t widget)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	return ptd->b_lock;
}

void memoctrl_set_lock(res_win_t widget, bool_t bLock)
{
	memoctrl_delta_t* ptd = GETMEMOCTRLDELTA(widget);

	XDL_ASSERT(ptd != NULL);

	ptd->b_lock = bLock;
}
