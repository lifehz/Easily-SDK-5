/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc common control document

	@module	xdcctrl.h | control interface file

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

#ifndef _XDCCTRL_H
#define _XDCCTRL_H

#include "xdcdef.h"

typedef struct _EDITDELTA{
	union{
		link_t_ptr data;
		link_t_ptr menu;
	};
	union{
		link_t_ptr item;
		const tchar_t* text;
	};
}EDITDELTA;


#ifdef	__cplusplus
extern "C" {
#endif

/*************************************svg control*******************************************************/
typedef struct _NOTICE_SVG{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr svg;
	void* data;

	int ret;
}NOTICE_SVG;

typedef enum{
	NC_SVGLBCLK,
	NC_SVGRBCLK,
	NC_SVGDBCLK,
}SVG_NOTICE_CODE;

/*
@FUNCTION svgctrl_create: create a svg widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t svgctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t owner);

/*
@FUNCTION svgctrl_attach: attach a svg document to widget.
@INPUT res_win_t widget: the svg widget.
@INPUT link_t_ptr ptr: the svg document.
@RETURN void: none.
*/
XDC_API void	svgctrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION svgctrl_detach: detach the svg document from widget.
@INPUT res_win_t widget: the svg widget.
@RETURN link_t_ptr: the svg link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr svgctrl_detach(res_win_t widget);

/*
@FUNCTION svgctrl_fetch: get the svg document from widget.
@INPUT res_win_t widget: the svg widget.
@RETURN link_t_ptr: the svg link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr svgctrl_fetch(res_win_t widget);

/*
@FUNCTION svgctrl_redraw: redraw the svg widget.
@INPUT res_win_t widget: the svg widget.
@RETURN void: none.
*/
XDC_API void	svgctrl_redraw(res_win_t widget);

/**********************************************form control******************************************************************/

typedef struct _NOTICE_FORM{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr form;
	link_t_ptr field;
	void* data;

	xpoint_t pt;

	int ret;
}NOTICE_FORM;

typedef enum{

	NC_FORMLBCLK,
	NC_FORMRBCLK,
	NC_FORMDBCLK,

	NC_FIELDHOVER,

	NC_FIELDCHANGING,
	NC_FIELDCHANGED,
	NC_FIELDSELECTED,

	NC_FIELDEDITING,
	NC_FIELDUPDATE,
	NC_FIELDCOMMIT,
	NC_FIELDROLLBACK,

	NC_FIELDCALCED,
	NC_FORMCALCED,

	NC_FIELDDRAG,
	NC_FIELDDROP,
	NC_FIELDSIZING,
	NC_FIELDSIZED,

	NC_FIELDGRID,
	NC_FIELDGRAPH,
	NC_FIELDFORM,
	NC_FIELDPHOTO,
	NC_FIELDIMAGES,
	NC_FIELDTAG,
	NC_FIELDRICH,
	NC_FIELDMEMO
}FORM_NOTICE_CODE;

/*
@FUNCTION formctrl_create: create a form widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t formctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t owner);

/*
@FUNCTION formctrl_attach: attach a form document to widget.
@INPUT res_win_t widget: the form widget.
@INPUT link_t_ptr ptr: the form document.
@RETURN void: none.
*/
XDC_API void formctrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION formctrl_detach: detach the form document from widget.
@INPUT res_win_t widget: the form widget.
@RETURN link_t_ptr: the form link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr formctrl_detach(res_win_t widget);

/*
@FUNCTION formctrl_fetch: get the form document from widget.
@INPUT res_win_t widget: the form widget.
@RETURN link_t_ptr: the form link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr formctrl_fetch(res_win_t widget);

/*
@FUNCTION formctrl_get_editor: get the current editor in form widget.
@INPUT res_win_t widget: the form widget.
@RETURN res_win_t: the editor widget if exists, otherwise return NULL.
*/
XDC_API res_win_t formctrl_get_editor(res_win_t widget);

/*
@FUNCTION formctrl_verify: verify field data in form widget.
@INPUT res_win_t widget: the form widget.
@INPUT bool_t b_alarm: nonzero for popup a alarm dialog for being a invalid field data.
@RETURN bool_t: return nonzero for being all of fields valus is valid.
*/
XDC_API bool_t formctrl_verify(res_win_t widget, bool_t b_alarm);

/*
@FUNCTION formctrl_accept: accept or discard current editor value in form widget.
@INPUT res_win_t widget: the form widget.
@INPUT bool_t b_accept: nonzero for accepting the editor value, zero for discarding.
@RETURN void: none.
*/
XDC_API void formctrl_accept(res_win_t widget, bool_t b_accept);

/*
@FUNCTION formctrl_is_update: test form field value is updated.
@INPUT res_win_t widget: the form widget.
@RETURN bool_t: return nonzero if some field content is updated.
*/
XDC_API bool_t formctrl_is_update(res_win_t widget);

/*
@FUNCTION formctrl_redraw: redraw form widget.
@INPUT res_win_t widget: the form widget.
@INPUT bool_t b_calc: nonzero for recalcing field value.
@RETURN void: none.
*/
XDC_API void formctrl_redraw(res_win_t widget, bool_t b_calc);

/*
@FUNCTION formctrl_redraw: redraw form widget.
@INPUT res_win_t widget: the form widget.
@INPUT link_t_ptr flk: the field link component.
@INPUT bool_t b_calc: nonzero for recalcing field value.
@RETURN void: none.
*/
XDC_API void formctrl_redraw_field(res_win_t widget, link_t_ptr flk, bool_t b_calc);

/*
@FUNCTION formctrl_tabskip: tab focus to next field.
@INPUT res_win_t widget: the form widget.
@INPUT int skip: the skip code, it can be WD_TAB_RIGHT, WD_TAB_LEFT, WD_TAB_UP, WD_TAB_DOWN, WD_TAB_PAGEUP, WD_TAB_PAGEDOWN, WD_TAB_HOME, WD_TAB_END.
@RETURN void: none.
*/
XDC_API void formctrl_tabskip(res_win_t widget, int skip);

/*
@FUNCTION formctrl_set_focus_field: set focus to the field.
@INPUT res_win_t widget: the form widget.
@INPUT link_t_ptr flk: the field link component.
@RETURN bool_t: return nonzero for being the field focused, otherwise return zero.
*/
XDC_API bool_t formctrl_set_focus_field(res_win_t widget, link_t_ptr flk);

/*
@FUNCTION formctrl_get_focus_field: get focus field in form widget.
@INPUT res_win_t widget: the form widget.
@RETURN link_t_ptr: return the focused field link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr formctrl_get_focus_field(res_win_t widget);

/*
@FUNCTION formctrl_get_field_rect: get field rect int form widget client.
@INPUT res_win_t widget: the form widget.
@INPUT link_t_ptr flk: the field link component.
@OUTPUT xrect_t* pxr: the rect struct using long member.
@RETURN void: none.
*/
XDC_API void formctrl_get_field_rect(res_win_t widget, link_t_ptr flk, xrect_t* pxr);

/*
@FUNCTION formctrl_get_lock: get the form widget is locked.
@INPUT res_win_t widget: the form widget.
@RETURN bool_t: return nonzero for being form locked, otherwise return zero.
*/
XDC_API bool_t formctrl_get_lock(res_win_t widget);

/*
@FUNCTION formctrl_set_lock: set the form widget is locked.
@INPUT res_win_t widget: the form widget.
@INPUT bool_t b_lock: nonzero for locking the form, zero for unlocking.
@RETURN void: none.
*/
XDC_API void formctrl_set_lock(res_win_t widget, bool_t b_lock);

/*
@FUNCTION formctrl_move_to_page: move to the page.
@INPUT res_win_t widget: the form widget.
@INPUT int page: the 1-based page index.
@RETURN void: none.
*/
XDC_API void formctrl_move_to_page(res_win_t widget, int page);

/*
@FUNCTION formctrl_move_first_page: move to the first page.
@INPUT res_win_t widget: the form widget.
@RETURN void: none.
*/
XDC_API void formctrl_move_first_page(res_win_t widget);

/*
@FUNCTION formctrl_move_last_page: move to the last page.
@INPUT res_win_t widget: the form widget.
@RETURN void: none.
*/
XDC_API void formctrl_move_last_page(res_win_t widget);

/*
@FUNCTION formctrl_move_next_page: move to the next page.
@INPUT res_win_t widget: the form widget.
@RETURN void: none.
*/
XDC_API void formctrl_move_next_page(res_win_t widget);

/*
@FUNCTION formctrl_move_prev_page: move to the previous page.
@INPUT res_win_t widget: the form widget.
@RETURN void: none.
*/
XDC_API void formctrl_move_prev_page(res_win_t widget);

/*
@FUNCTION formctrl_get_cur_page: get current page.
@INPUT res_win_t widget: the form widget.
@RETURN int: return the 1-based page index.
*/
XDC_API int formctrl_get_cur_page(res_win_t widget);

/*
@FUNCTION formctrl_get_max_page: get maximized page.
@INPUT res_win_t widget: the form widget.
@RETURN int: return the total pages.
*/
XDC_API int formctrl_get_max_page(res_win_t widget);

/*
@FUNCTION formctrl_get_dirty: get form is dirty in design mode.
@INPUT res_win_t widget: the form widget.
@RETURN bool_t: return nonzero for beging dirty.
*/
XDC_API bool_t formctrl_get_dirty(res_win_t widget);

/*
@FUNCTION formctrl_set_dirty: set form is dirty in design mode.
@INPUT res_win_t widget: the form widget.
@INPUT bool_t b_dirty: nonzero for setting dirty, zero for cleaning.
@RETURN void: none.
*/
XDC_API void formctrl_set_dirty(res_win_t widget, bool_t b_dirty);

/*
@FUNCTION formctrl_set_field_text: set the field text.
@INPUT res_win_t widget: the form widget.
@INPUT link_t_ptr flk: the field link component.
@INPUT const tchar_t* text: the text token.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDC_API bool_t formctrl_set_field_text(res_win_t widget, link_t_ptr flk, const tchar_t* text);

/***********************************grid control*******************************************************************/
#define GRID_NOTICE_ACCEPT		0
#define GRID_NOTICE_REJECT		1
#define GRID_NOTICE_DELETE		-1

typedef struct _NOTICE_GRID{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr grid;
	link_t_ptr row;
	link_t_ptr col;
	void* data;

	int ret;
}NOTICE_GRID;


typedef enum{
	NC_GRIDLBCLK,
	NC_GRIDRBCLK,
	NC_GRIDDBCLK,

	NC_COLHOVER,
	NC_COLCHANGING,
	NC_COLCHANGED,
	NC_COLSELECTED,

	NC_CELLSETFOCUS,
	NC_CELLKILLFOCUS,

	NC_CELLEDITING,
	NC_CELLUPDATE,
	NC_CELLCOMMIT,
	NC_CELLROLLBACK,

	NC_COLCALCED,
	NC_ROWCALCED,
	NC_GRIDCALCED,

	NC_ROWCHANGING,
	NC_ROWCHANGED,
	NC_ROWCHECKED,
	NC_ROWINSERT,
	NC_ROWDELETE,

	NC_ROWSIZING,
	NC_ROWSIZED,
	NC_ROWDRAG,
	NC_ROWDROP,
	NC_COLSIZING,
	NC_COLSIZED,
	NC_COLDRAG,
	NC_COLDROP
}GRID_NOTICE_CODE;

/*
@FUNCTION gridctrl_create: create a grid widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t gridctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION gridctrl_attach: attach a grid document to widget.
@INPUT res_win_t widget: the grid widget.
@INPUT link_t_ptr ptr: the grid document.
@RETURN void: none.
*/
XDC_API void	gridctrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION gridctrl_detach: detach the grid document from widget.
@INPUT res_win_t widget: the grid widget.
@RETURN link_t_ptr: the grid link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr gridctrl_detach(res_win_t widget);

/*
@FUNCTION gridctrl_fetch: get the grid document from widget.
@INPUT res_win_t widget: the grid widget.
@RETURN link_t_ptr: the grid link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr gridctrl_fetch(res_win_t widget);

/*
@FUNCTION gridctrl_verify: verify cell data in grid widget.
@INPUT res_win_t widget: the grid widget.
@INPUT bool_t b_alarm: nonzero for popup a alarm dialog for being a invalid cell data.
@RETURN bool_t: return nonzero for being all of cells valus is valid.
*/
XDC_API bool_t	gridctrl_verify(res_win_t widget, bool_t b_alarm);

/*
@FUNCTION gridctrl_accept: accept or discard current editor value in grid widget.
@INPUT res_win_t widget: the grid widget.
@INPUT bool_t b_accept: nonzero for accepting the editor value, zero for discarding.
@RETURN void: none.
*/
XDC_API void	gridctrl_accept(res_win_t widget, bool_t b_accept);

/*
@FUNCTION gridctrl_is_update: test grid rows is updated.
@INPUT res_win_t widget: the grid widget.
@RETURN bool_t: return nonzero if some rows content is updated.
*/
XDC_API bool_t	gridctrl_is_update(res_win_t widget);

/*
@FUNCTION gridctrl_redraw: redraw grid widget.
@INPUT res_win_t widget: the grid widget.
@INPUT bool_t b_calc: nonzero for recalcing rows value.
@RETURN void: none.
*/
XDC_API void	gridctrl_redraw(res_win_t widget, bool_t b_calc);

/*
@FUNCTION gridctrl_redraw_row: redraw the row in grid widget.
@INPUT res_win_t widget: the grid widget.
@INPUT link_t_ptr rlk: the row link component.
@INPUT bool_t b_calc: nonzero for recalcing cells value of the row.
@RETURN void: none.
*/
XDC_API void	gridctrl_redraw_row(res_win_t widget, link_t_ptr rlk, bool_t b_calc);

/*
@FUNCTION gridctrl_redraw_col: redraw the col in grid widget.
@INPUT res_win_t widget: the grid widget.
@INPUT link_t_ptr clk: the col link component.
@INPUT bool_t b_calc: nonzero for recalcing cells value of the col.
@RETURN void: none.
*/
XDC_API void	gridctrl_redraw_col(res_win_t widget, link_t_ptr clk, bool_t b_calc);

/*
@FUNCTION gridctrl_tabskip: tab focus to next cell.
@INPUT res_win_t widget: the grid widget.
@INPUT int skip: the skip code, it can be WD_TAB_RIGHT, WD_TAB_LEFT, WD_TAB_UP, WD_TAB_DOWN, WD_TAB_PAGEUP, WD_TAB_PAGEDOWN, WD_TAB_HOME, WD_TAB_END.
@RETURN void: none.
*/
XDC_API void	gridctrl_tabskip(res_win_t widget, int skip);

/*
@FUNCTION gridctrl_set_focus_cell: set focus to the cell.
@INPUT res_win_t widget: the grid widget.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@RETURN bool_t: return nonzero for being the cell focused, otherwise return zero.
*/
XDC_API bool_t	gridctrl_set_focus_cell(res_win_t widget, link_t_ptr rlk, link_t_ptr clk);

/*
@FUNCTION gridctrl_get_focus_row: get focus row in grid widget.
@INPUT res_win_t widget: the grid widget.
@RETURN link_t_ptr: return the focused row link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr gridctrl_get_focus_row(res_win_t widget);

/*
@FUNCTION gridctrl_get_focus_col: get focus col in grid widget.
@INPUT res_win_t widget: the grid widget.
@RETURN link_t_ptr: return the focused col link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr gridctrl_get_focus_col(res_win_t widget);

/*
@FUNCTION gridctrl_get_cell_rect: get cell rect int grid widget client.
@INPUT res_win_t widget: the grid widget.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@OUTPUT xrect_t* pxr: the rect struct using long member.
@RETURN void: none.
*/
XDC_API void	gridctrl_get_cell_rect(res_win_t widget, link_t_ptr rlk, link_t_ptr clk, xrect_t* pxr);

/*
@FUNCTION gridctrl_get_lock: get the grid widget is locked.
@INPUT res_win_t widget: the grid widget.
@RETURN bool_t: return nonzero for being grid locked, otherwise return zero.
*/
XDC_API bool_t	gridctrl_get_lock(res_win_t widget);

/*
@FUNCTION gridctrl_set_lock: set the grid widget is locked.
@INPUT res_win_t widget: the grid widget.
@INPUT bool_t b_lock: nonzero for locking the grid, zero for unlocking.
@RETURN void: none.
*/
XDC_API void	gridctrl_set_lock(res_win_t widget, bool_t b_lock);

/*
@FUNCTION gridctrl_move_to_page: move to the page.
@INPUT res_win_t widget: the grid widget.
@INPUT int page: the 1-based page index.
@RETURN void: none.
*/
XDC_API void	gridctrl_move_to_page(res_win_t widget, int page);

/*
@FUNCTION gridctrl_move_first_page: move to the first page.
@INPUT res_win_t widget: the grid widget.
@RETURN void: none.
*/
XDC_API void	gridctrl_move_first_page(res_win_t widget);

/*
@FUNCTION gridctrl_move_prev_page: move to the previous page.
@INPUT res_win_t widget: the grid widget.
@RETURN void: none.
*/
XDC_API void	gridctrl_move_prev_page(res_win_t widget);

/*
@FUNCTION gridctrl_move_next_page: move to the next page.
@INPUT res_win_t widget: the grid widget.
@RETURN void: none.
*/
XDC_API void	gridctrl_move_next_page(res_win_t widget);

/*
@FUNCTION gridctrl_move_last_page: move to the last page.
@INPUT res_win_t widget: the grid widget.
@RETURN void: none.
*/
XDC_API void	gridctrl_move_last_page(res_win_t widget);

/*
@FUNCTION gridctrl_get_cur_page: get current page.
@INPUT res_win_t widget: the grid widget.
@RETURN int: return the 1-based page index.
*/
XDC_API int		gridctrl_get_cur_page(res_win_t widget);

/*
@FUNCTION gridctrl_get_max_page: get maximized page.
@INPUT res_win_t widget: the grid widget.
@RETURN int: return the total pages.
*/
XDC_API int		gridctrl_get_max_page(res_win_t widget);

/*
@FUNCTION gridctrl_get_dirty: get grid is dirty in design mode.
@INPUT res_win_t widget: the grid widget.
@RETURN bool_t: return nonzero for beging dirty.
*/
XDC_API bool_t	gridctrl_get_dirty(res_win_t widget);

/*
@FUNCTION gridctrl_set_dirty: set grid is dirty in design mode.
@INPUT res_win_t widget: the grid widget.
@INPUT bool_t b_dirty: nonzero for setting dirty, zero for cleaning.
@RETURN void: none.
*/
XDC_API void	gridctrl_set_dirty(res_win_t widget, bool_t b_dirty);

/*
@FUNCTION gridctrl_auto_insert: set insert new row automatic.
@INPUT res_win_t widget: the grid widget.
@INPUT bool_t b_auto: nonzero for automatic.
@RETURN void: none.
*/
XDC_API void	gridctrl_auto_insert(res_win_t widget, bool_t b_auto);

/*
@FUNCTION gridctrl_set_fixed: set fixed navigate col.
@INPUT res_win_t widget: the grid widget.
@INPUT link_t_ptr clk: the col link component.
@RETURN void: none.
*/
XDC_API void	gridctrl_set_fixed(res_win_t widget, link_t_ptr clk);

/*
@FUNCTION gridctrl_get_fixed: get fixed navigate col.
@INPUT res_win_t widget: the grid widget.
@RETURN link_t_ptr: return the col link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr	gridctrl_get_fixed(res_win_t widget);

/*
@FUNCTION gridctrl_set_cell_text: Set cell text.
@INPUT res_win_t widget: the grid widget.
@INPUT link_t_ptr rlk: the row link component.
@INPUT link_t_ptr clk: the col link component.
@INPUT const tchar_t* sz_text: the text token.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDC_API bool_t	gridctrl_set_cell_text(res_win_t widget, link_t_ptr rlk, link_t_ptr clk, const tchar_t* sz_text);

/*
@FUNCTION gridctrl_copy_row: copy source row content into current focus row.
@INPUT res_win_t widget: the grid widget.
@INPUT link_t_ptr srcGrid: the source grid link component.
@INPUT link_t_ptr srcRow: the source row link component.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDC_API bool_t	gridctrl_copy_row(res_win_t widget, link_t_ptr srcGrid, link_t_ptr srcRow);

/*
@FUNCTION gridctrl_delete_row: delete the row in grid widget.
@INPUT res_win_t widget: the grid widget.
@INPUT link_t_ptr rlk: the row link component.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDC_API bool_t	gridctrl_delete_row(res_win_t widget, link_t_ptr rlk);

/*
@FUNCTION gridctrl_insert_row: add a new row into grid.
@INPUT res_win_t widget: the grid widget.
@INPUT link_t_ptr pos: the row link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: if succeeds return the new row link component, fails return NULL.
*/
XDC_API link_t_ptr gridctrl_insert_row(res_win_t widget, link_t_ptr pos);

/*
@FUNCTION gridctrl_find: find a matched row from position.
@INPUT res_win_t widget: the grid widget.
@INPUT const tchar_t* token: the pattern token, eg: "col = val".
@RETURN void: none.
*/
XDC_API void	gridctrl_find(res_win_t widget, const tchar_t* token);

/*
@FUNCTION gridctrl_filter: filter a matched rows.
@INPUT res_win_t widget: the grid widget.
@INPUT const tchar_t* token: the pattern token, eg: "col = val".
@RETURN void: none.
*/
XDC_API void	gridctrl_filter(res_win_t widget, const tchar_t* token);

/*
@FUNCTION gridctrl_popup_size: calcing the grid popup size according to client size.
@INPUT res_win_t widget: the grid widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void	gridctrl_popup_size(res_win_t widget, xsize_t* pse);

/**********************************graph control*******************************************************************/

typedef struct _NOTICE_GRAPH{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr graph;
	link_t_ptr xax;
	link_t_ptr yax;
	link_t_ptr gax;
	void* data;

	int ret;
}NOTICE_GRAPH;

typedef enum{
	NC_GRAPHLBCLK,
	NC_GRAPHRBCLK,
	NC_GRAPHDBCLK,

	NC_GAXCHANGING,
	NC_GAXCHANGED,

	NC_YAXHOVER,
	NC_YAXCHANGING,
	NC_YAXCHANGED,
	NC_YAXSELECTED,

	NC_COORSETFOCUS,
	NC_COORKILLFOCUS,

	NC_COOREDITING,
	NC_COORUPDATE,
	NC_COORCOMMIT,
	NC_COORROLLBACK,

	NC_XAXCHANGING,
	NC_XAXCHANGED,
	NC_XAXCHECKED,
	NC_XAXDELETE,
	NC_XAXINSERT,

	NC_XAXSIZING,
	NC_XAXSIZED,
	NC_YAXSIZING,
	NC_YAXSIZED,

	NC_XAXDRAG,
	NC_XAXDROP,
	NC_YAXDRAG,
	NC_YAXDROP,

	NC_GRAPHCALCED,
	NC_YAXCALCED,
	NC_XAXCALCED
}GRAPH_NOTICE_CODE;

/*
@FUNCTION graphctrl_create: create a graph widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t graphctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION graphctrl_attach: attach a graph document to widget.
@INPUT res_win_t widget: the graph widget.
@INPUT link_t_ptr ptr: the graph document.
@RETURN void: none.
*/
XDC_API void	graphctrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION graphctrl_detach: detach the graph document from widget.
@INPUT res_win_t widget: the graph widget.
@RETURN link_t_ptr: the graph link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr graphctrl_detach(res_win_t widget);

/*
@FUNCTION graphctrl_fetch: get the graph document from widget.
@INPUT res_win_t widget: the graph widget.
@RETURN link_t_ptr: the graph link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr graphctrl_fetch(res_win_t widget);

/*
@FUNCTION graphctrl_accept: accept or discard current editor value in graph widget.
@INPUT res_win_t widget: the graph widget.
@INPUT bool_t b_accept: nonzero for accepting the editor value, zero for discarding.
@RETURN void: none.
*/
XDC_API void	graphctrl_accept(res_win_t widget, bool_t b_accept);

/*
@FUNCTION graphctrl_is_update: test graph xaxs is updated.
@INPUT res_win_t widget: the graph widget.
@RETURN bool_t: return nonzero if some xaxs content is updated.
*/
XDC_API bool_t	graphctrl_is_update(res_win_t widget);

/*
@FUNCTION gridctrl_redraw: redraw grid widget.
@INPUT res_win_t widget: the grid widget.
@INPUT bool_t b_calc: nonzero for recalcing rows value.
@RETURN void: none.
*/
XDC_API void	graphctrl_redraw(res_win_t widget, bool_t b_calc);

/*
@FUNCTION graphctrl_redraw_xax: redraw the xax in graph widget.
@INPUT res_win_t widget: the graph widget.
@INPUT link_t_ptr clk: the xax link component.
@INPUT bool_t b_calc: nonzero for recalcing coors value of the xax.
@RETURN void: none.
*/
XDC_API void	graphctrl_redraw_xax(res_win_t widget, link_t_ptr xlk, bool_t bCalc);

/*
@FUNCTION graphctrl_redraw_yax: redraw the yax in graph widget.
@INPUT res_win_t widget: the graph widget.
@INPUT link_t_ptr clk: the yax link component.
@INPUT bool_t b_calc: nonzero for recalcing coors value of the yax.
@RETURN void: none.
*/
XDC_API void	graphctrl_redraw_yax(res_win_t widget, link_t_ptr ylk, bool_t bCalc);

/*
@FUNCTION gridctrl_tabskip: tab focus to next cell.
@INPUT res_win_t widget: the grid widget.
@INPUT int skip: the skip code, it can be WD_TAB_RIGHT, WD_TAB_LEFT, WD_TAB_UP, WD_TAB_DOWN, WD_TAB_PAGEUP, WD_TAB_PAGEDOWN, WD_TAB_HOME, WD_TAB_END.
@RETURN void: none.
*/
XDC_API void	graphctrl_tabskip(res_win_t widget, int skip);

/*
@FUNCTION graphctrl_set_focus_coor: set focus to the coor.
@INPUT res_win_t widget: the graph widget.
@INPUT link_t_ptr xlk: the xax link component.
@INPUT link_t_ptr ylk: the yax link component.
@RETURN bool_t: return nonzero for being the coor focused, otherwise return zero.
*/
XDC_API bool_t	graphctrl_set_focus_coor(res_win_t widget, link_t_ptr xlk, link_t_ptr ylk);

/*
@FUNCTION graphctrl_get_focus_xax: get focus xax in graph widget.
@INPUT res_win_t widget: the graph widget.
@RETURN link_t_ptr: return the focused xax link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr graphctrl_get_focus_xax(res_win_t widget);

/*
@FUNCTION graphctrl_get_focus_yax: get focus yax in graph widget.
@INPUT res_win_t widget: the graph widget.
@RETURN link_t_ptr: return the focused yax link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr graphctrl_get_focus_yax(res_win_t widget);

/*
@FUNCTION graphctrl_get_focus_gax: get focus gax in graph widget.
@INPUT res_win_t widget: the graph widget.
@RETURN link_t_ptr: return the focused gax link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr graphctrl_get_focus_gax(res_win_t widget);

/*
@FUNCTION graphctrl_get_coor_rect: get coor rect int graph widget client.
@INPUT res_win_t widget: the graph widget.
@INPUT link_t_ptr xlk: the xax link component.
@INPUT link_t_ptr ylk: the yax link component.
@OUTPUT xrect_t* pxr: the rect struct using long member.
@RETURN void: none.
*/
XDC_API void	graphctrl_get_coor_rect(res_win_t widget, link_t_ptr xlk, link_t_ptr ylk, xrect_t* pxr);

/*
@FUNCTION graphctrl_get_lock: get the graph widget is locked.
@INPUT res_win_t widget: the graph widget.
@RETURN bool_t: return nonzero for being graph locked, otherwise return zero.
*/
XDC_API bool_t	graphctrl_get_lock(res_win_t widget);

/*
@FUNCTION graphctrl_set_lock: set the graph widget is locked.
@INPUT res_win_t widget: the graph widget.
@INPUT bool_t b_lock: nonzero for locking the graph, zero for unlocking.
@RETURN void: none.
*/
XDC_API void	graphctrl_set_lock(res_win_t widget, bool_t b_lock);

/*
@FUNCTION graphctrl_move_to_page: move to the page.
@INPUT res_win_t widget: the graph widget.
@INPUT int page: the 1-based page index.
@RETURN void: none.
*/
XDC_API void	graphctrl_move_to_page(res_win_t widget, int page);

/*
@FUNCTION graphctrl_move_first_page: move to the first page.
@INPUT res_win_t widget: the graph widget.
@RETURN void: none.
*/
XDC_API void	graphctrl_move_first_page(res_win_t widget);

/*
@FUNCTION graphctrl_move_prev_page: move to the previous page.
@INPUT res_win_t widget: the graph widget.
@RETURN void: none.
*/
XDC_API void	graphctrl_move_prev_page(res_win_t widget);

/*
@FUNCTION graphctrl_move_next_page: move to the next page.
@INPUT res_win_t widget: the graph widget.
@RETURN void: none.
*/
XDC_API void	graphctrl_move_next_page(res_win_t widget);

/*
@FUNCTION graphctrl_move_last_page: move to the last page.
@INPUT res_win_t widget: the graph widget.
@RETURN void: none.
*/
XDC_API void	graphctrl_move_last_page(res_win_t widget);

/*
@FUNCTION graphctrl_get_cur_page: get current page.
@INPUT res_win_t widget: the graph widget.
@RETURN int: return the 1-based page index.
*/
XDC_API int		graphctrl_get_cur_page(res_win_t widget);

/*
@FUNCTION graphctrl_get_max_page: get maximized page.
@INPUT res_win_t widget: the graph widget.
@RETURN int: return the tital pages.
*/
XDC_API int		graphctrl_get_max_page(res_win_t widget);

/*
@FUNCTION graphctrl_get_dirty: get graph is dirty in design mode.
@INPUT res_win_t widget: the graph widget.
@RETURN bool_t: return nonzero for beging dirty.
*/
XDC_API bool_t	graphctrl_get_dirty(res_win_t widget);

/*
@FUNCTION graphctrl_set_dirty: set graph is dirty in design mode.
@INPUT res_win_t widget: the graph widget.
@INPUT bool_t b_dirty: nonzero for setting dirty, zero for cleaning.
@RETURN void: none.
*/
XDC_API void	graphctrl_set_dirty(res_win_t widget, bool_t b_dirty);

/*
@FUNCTION graphctrl_auto_insert: set insert new xax automatic.
@INPUT res_win_t widget: the graph widget.
@INPUT bool_t b_auto: nonzero for automatic.
@RETURN void: none.
*/
XDC_API void	graphctrl_auto_insert(res_win_t widget, bool_t b_auto);

/*
@FUNCTION graphctrl_delete_xax: delete the xax in graph widget.
@INPUT res_win_t widget: the graph widget.
@INPUT link_t_ptr xlk: the xax link component.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDC_API bool_t	graphctrl_delete_xax(res_win_t widget, link_t_ptr xlk);

/*
@FUNCTION graphctrl_insert_xax: add a new xax into grid.
@INPUT res_win_t widget: the graph widget.
@INPUT link_t_ptr pos: the xax link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: if succeeds return the new xax link component, fails return NULL.
*/
XDC_API link_t_ptr graphctrl_insert_xax(res_win_t widget, link_t_ptr pre);

/*
@FUNCTION graphctrl_set_coor_text: Set coor text.
@INPUT res_win_t widget: the graph widget.
@INPUT link_t_ptr xlk: the xax link component.
@INPUT link_t_ptr ylk: the yax link component.
@INPUT const tchar_t* sz_text: the text token.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDC_API bool_t	graphctrl_set_coor_text(res_win_t widget, link_t_ptr xlk, link_t_ptr ylk, const tchar_t* sz_text);

/***********************************dialog control*******************************************************************/
typedef struct _NOTICE_DIALOG{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr dialog;
	link_t_ptr item;
	void* data;

	int ret;
}NOTICE_DIALOG;

typedef enum{

	NC_DIALOGLBCLK,
	NC_DIALOGRBCLK,
	NC_DIALOGDBCLK,

	NC_DIALOGITEMHOVER,

	NC_DIALOGITEMCHANGING,
	NC_DIALOGITEMCHANGED,
	NC_DIALOGITEMSELECTED,

	NC_DIALOGITEMCALCED,
	NC_DIALOGCALCED,

	NC_DIALOGITEMDRAG,
	NC_DIALOGITEMDROP,
	NC_DIALOGITEMSIZING,
	NC_DIALOGITEMSIZED,
}DIALOG_NOTICE_CODE;

/*
@FUNCTION dialogctrl_create: create a dialog widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t dialogctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION dialogctrl_attach: attach a dialog document to widget.
@INPUT res_win_t widget: the dialog widget.
@INPUT link_t_ptr ptr: the dialog document.
@RETURN void: none.
*/
XDC_API void dialogctrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION dialogctrl_detach: detach the dialog document from widget.
@INPUT res_win_t widget: the dialog widget.
@RETURN link_t_ptr: the dialog link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr dialogctrl_detach(res_win_t widget);

/*
@FUNCTION dialogctrl_fetch: get the dialog document from widget.
@INPUT res_win_t widget: the dialog widget.
@RETURN link_t_ptr: the dialog link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr dialogctrl_fetch(res_win_t widget);

/*
@FUNCTION dialogctrl_redraw: redraw dialog widget.
@INPUT res_win_t widget: the dialog widget.
@RETURN void: none.
*/
XDC_API void dialogctrl_redraw(res_win_t widget);

/*
@FUNCTION dialogctrl_redraw_item: redraw the item in dialog widget.
@INPUT res_win_t widget: the dialog widget.
@INPUT link_t_ptr ilk: the item link component.
@INPUT bool_t b_calc: nonzero for recalcing coors value of the yax.
@RETURN void: none.
*/
XDC_API void dialogctrl_redraw_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION dialogctrl_tabskip: tab focus to next item.
@INPUT res_win_t widget: the dialog widget.
@INPUT int skip: the skip code, it can be WD_TAB_RIGHT, WD_TAB_LEFT, WD_TAB_UP, WD_TAB_DOWN, WD_TAB_PAGEUP, WD_TAB_PAGEDOWN, WD_TAB_HOME, WD_TAB_END.
@RETURN void: none.
*/
XDC_API void dialogctrl_tabskip(res_win_t widget, int skip);

/*
@FUNCTION dialogctrl_set_focus_item: set focus to the item.
@INPUT res_win_t widget: the dialog widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being the item focused, otherwise return zero.
*/
XDC_API bool_t dialogctrl_set_focus_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION dialogctrl_get_focus_item: get focus item in dialog widget.
@INPUT res_win_t widget: the dialog widget.
@RETURN link_t_ptr: return the focused item link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr dialogctrl_get_focus_item(res_win_t widget);

/*
@FUNCTION dialogctrl_get_item_rect: get item rect int dialog widget client.
@INPUT res_win_t widget: the dialog widget.
@INPUT link_t_ptr ilk: the item link component.
@OUTPUT xrect_t* pxr: the rect struct using long member.
@RETURN void: none.
*/
XDC_API void dialogctrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

/*
@FUNCTION dialogctrl_get_dirty: get dialog is dirty in design mode.
@INPUT res_win_t widget: the dialog widget.
@RETURN bool_t: return nonzero for beging dirty.
*/
XDC_API bool_t dialogctrl_get_dirty(res_win_t widget);

/*
@FUNCTION dialogctrl_set_dirty: set dialog is dirty in design mode.
@INPUT res_win_t widget: the dialog widget.
@INPUT bool_t b_dirty: nonzero for setting dirty, zero for cleaning.
@RETURN void: none.
*/
XDC_API void dialogctrl_set_dirty(res_win_t widget, bool_t b_dirty);

/***********************************chart control*******************************************************************/
typedef struct _NOTICE_CHART{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr chart;
	link_t_ptr table;
	void* data;

	int ret;
}NOTICE_CHART;

typedef enum{

	NC_CHARTLBCLK,
	NC_CHARTRBCLK,
	NC_CHARTDBCLK,

	NC_CHARTTABLEHOVER,

	NC_CHARTTABLECHANGING,
	NC_CHARTTABLECHANGED,
	NC_CHARTTABLESELECTED,

	NC_CHARTTABLECALCED,
	NC_CHARTCALCED,

	NC_CHARTTABLEDRAG,
	NC_CHARTTABLEDROP,
	NC_CHARTTABLESIZING,
	NC_CHARTTABLESIZED,
}CHART_NOTICE_CODE;

/*
@FUNCTION chartctrl_create: create a chart widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t chartctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION chartctrl_attach: attach a chart document to widget.
@INPUT res_win_t widget: the chart widget.
@INPUT link_t_ptr ptr: the chart document.
@RETURN void: none.
*/
XDC_API void chartctrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION chartctrl_detach: detach the chart document from widget.
@INPUT res_win_t widget: the chart widget.
@RETURN link_t_ptr: the chart link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr chartctrl_detach(res_win_t widget);

/*
@FUNCTION chartctrl_fetch: get the chart document from widget.
@INPUT res_win_t widget: the chart widget.
@RETURN link_t_ptr: the chart link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr chartctrl_fetch(res_win_t widget);

/*
@FUNCTION chartctrl_redraw: redraw chart widget.
@INPUT res_win_t widget: the chart widget.
@RETURN void: none.
*/
XDC_API void chartctrl_redraw(res_win_t widget);

/*
@FUNCTION dialogctrl_redraw_item: redraw the table in chart widget.
@INPUT res_win_t widget: the chart widget.
@INPUT link_t_ptr ilk: the table link component.
@RETURN void: none.
*/
XDC_API void chartctrl_redraw_table(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION chartctrl_tabskip: tab focus to next table.
@INPUT res_win_t widget: the chart widget.
@INPUT int skip: the skip code, it can be WD_TAB_RIGHT, WD_TAB_LEFT, WD_TAB_UP, WD_TAB_DOWN, WD_TAB_PAGEUP, WD_TAB_PAGEDOWN, WD_TAB_HOME, WD_TAB_END.
@RETURN void: none.
*/
XDC_API void chartctrl_tabskip(res_win_t widget, int skip);

/*
@FUNCTION chartctrl_set_focus_table: set focus to the table.
@INPUT res_win_t widget: the chart widget.
@INPUT link_t_ptr flk: the table link component.
@RETURN bool_t: return nonzero for being the table focused, otherwise return zero.
*/
XDC_API bool_t chartctrl_set_focus_table(res_win_t widget, link_t_ptr flk);

/*
@FUNCTION chartctrl_get_focus_table: get focus table in chart widget.
@INPUT res_win_t widget: the chart widget.
@RETURN link_t_ptr: return the focused item link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr chartctrl_get_focus_table(res_win_t widget);

/*
@FUNCTION chartctrl_get_table_rect: get table rect int chart widget client.
@INPUT res_win_t widget: the chart widget.
@INPUT link_t_ptr flk: the table link component.
@OUTPUT xrect_t* pxr: the rect struct using long member.
@RETURN void: none.
*/
XDC_API void chartctrl_get_table_rect(res_win_t widget, link_t_ptr flk, xrect_t* pxr);

/*
@FUNCTION chartctrl_get_dirty: get chart is dirty in design mode.
@INPUT res_win_t widget: the chart widget.
@RETURN bool_t: return nonzero for beging dirty.
*/
XDC_API bool_t chartctrl_get_dirty(res_win_t widget);

/*
@FUNCTION chartctrl_set_dirty: set chart is dirty in design mode.
@INPUT res_win_t widget: the chart widget.
@INPUT bool_t b_dirty: nonzero for setting dirty, zero for cleaning.
@RETURN void: none.
*/
XDC_API void chartctrl_set_dirty(res_win_t widget, bool_t b_dirty);

/********************************image list control************************************************************/
typedef struct _NOTICE_IMAGES{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr images;
	link_t_ptr item;
	void* data;

	int ret;
}NOTICE_IMAGES;

typedef enum{

	NC_IMAGESLBCLK,
	NC_IMAGESRBCLK,
	NC_IMAGESDBCLK,

	NC_IMAGEITEMHOVER,

	NC_IMAGEITEMDRAG,
	NC_IMAGEITEMDROP,

	NC_IMAGEITEMCHANGING,
	NC_IMAGEITEMCHANGED,
	NC_IMAGEITEMCHECKED,

	NC_IMAGEITEMEDITING,
	NC_IMAGEITEMUPDATE,
	NC_IMAGEITEMCOMMIT,
	NC_IMAGEITEMROLLBACK,

	NC_IMAGESCALCED,
	NC_IMAGEITEMCALCED
}IMAGES_NOTICE_CODE;

/*
@FUNCTION imagesctrl_create: create a images widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t imagesctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION imagesctrl_attach: attach a images document to widget.
@INPUT res_win_t widget: the images widget.
@INPUT link_t_ptr ptr: the images document.
@RETURN void: none.
*/
XDC_API void	imagesctrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION imagesctrl_detach: detach the images document from widget.
@INPUT res_win_t widget: the images widget.
@RETURN link_t_ptr: the images link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr	imagesctrl_detach(res_win_t widget);

/*
@FUNCTION imagesctrl_fetch: get the images document from widget.
@INPUT res_win_t widget: the images widget.
@RETURN link_t_ptr: the images link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr	imagesctrl_fetch(res_win_t widget);

/*
@FUNCTION imagesctrl_redraw: redraw images widget.
@INPUT res_win_t widget: the images widget.
@RETURN void: none.
*/
XDC_API void	imagesctrl_redraw(res_win_t widget);

/*
@FUNCTION imagesctrl_redraw_item: redraw the item in images widget.
@INPUT res_win_t widget: the images widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN void: none.
*/
XDC_API void	imagesctrl_redraw_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION imagesctrl_tabskip: tab focus to next item.
@INPUT res_win_t widget: the images widget.
@INPUT int skip: the skip code, it can be WD_TAB_RIGHT, WD_TAB_LEFT, WD_TAB_UP, WD_TAB_DOWN, WD_TAB_PAGEUP, WD_TAB_PAGEDOWN, WD_TAB_HOME, WD_TAB_END.
@RETURN void: none.
*/
XDC_API void	imagesctrl_tabskip(res_win_t widget, int skip);

/*
@FUNCTION imagesctrl_set_focus_item: set focus to the item.
@INPUT res_win_t widget: the chart widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being the item focused, otherwise return zero.
*/
XDC_API bool_t	imagesctrl_set_focus_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION imagesctrl_get_focus_item: get focus item in images widget.
@INPUT res_win_t widget: the images widget.
@RETURN link_t_ptr: return the focused item link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr	imagesctrl_get_focus_item(res_win_t widget);

/*
@FUNCTION imagesctrl_get_item_rect: get item rect int images widget client.
@INPUT res_win_t widget: the images widget.
@INPUT link_t_ptr flk: the item link component.
@OUTPUT xrect_t* pxr: the rect struct using long member.
@RETURN void: none.
*/
XDC_API void	imagesctrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

/*
@FUNCTION imagesctrl_get_lock: get the images widget is locked.
@INPUT res_win_t widget: the images widget.
@RETURN bool_t: return nonzero for being images locked, otherwise return zero.
*/
XDC_API bool_t	imagesctrl_get_lock(res_win_t widget);

/*
@FUNCTION imagesctrl_set_lock: set the images widget is locked.
@INPUT res_win_t widget: the images widget.
@INPUT bool_t b_lock: nonzero for locking the images, zero for unlocking.
@RETURN void: none.
*/
XDC_API void	imagesctrl_set_lock(res_win_t widget, bool_t bLock);

/*
@FUNCTION imagesctrl_set_item_title: Set item text.
@INPUT res_win_t widget: the images widget.
@INPUT link_t_ptr ilk: the item link component.
@INPUT const tchar_t* sz_text: the title token.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDC_API bool_t imagesctrl_set_item_title(res_win_t widget, link_t_ptr ilk, const tchar_t* sz_text);

/*
@FUNCTION imagesctrl_popup_size: calcing the images popup size according to client size.
@INPUT res_win_t widget: the images widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void	imagesctrl_popup_size(res_win_t widget, xsize_t* pse);

/*********************************topog control**************************************************************/

typedef struct _NOTICE_TOPOG{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr topog;
	link_t_ptr spot;
	void* data;

	int row;
	int col;

	int ret;
}NOTICE_TOPOG;

typedef enum{
	NC_TOPOGLBCLK,
	NC_TOPOGRBCLK,
	NC_TOPOGDBCLK,

	NC_TOPOGSPOTHOVER,
	NC_TOPOGSPOTCHANGING,
	NC_TOPOGSPOTCHANGED,
	NC_TOPOGSPOTSELECTED,

	NC_TOPOGSPOTDRAG,
	NC_TOPOGSPOTDROP,

	NC_TOPOGCALCED,
	NC_TOPOGSPOTCALCED
}TOPOG_NOTICE_CODE;

/*
@FUNCTION topogctrl_create: create a topog widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t topogctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION topogctrl_attach: attach a topog document to widget.
@INPUT res_win_t widget: the topog widget.
@INPUT link_t_ptr ptr: the topog document.
@RETURN void: none.
*/
XDC_API void topogctrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION topogctrl_detach: detach the topog document from widget.
@INPUT res_win_t widget: the topog widget.
@RETURN link_t_ptr: the topog link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr topogctrl_detach(res_win_t widget);

/*
@FUNCTION topogctrl_fetch: get the topog document from widget.
@INPUT res_win_t widget: the topog widget.
@RETURN link_t_ptr: the topog link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr topogctrl_fetch(res_win_t widget);

/*
@FUNCTION topogctrl_redraw: redraw topog widget.
@INPUT res_win_t widget: the topog widget.
@RETURN void: none.
*/
XDC_API void topogctrl_redraw(res_win_t widget);

/*
@FUNCTION topogctrl_redraw_spot: redraw the sopt in topog widget.
@INPUT res_win_t widget: the topog widget.
@INPUT link_t_ptr ilk: the spot link component.
@RETURN void: none.
*/
XDC_API void topogctrl_redraw_spot(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION topogctrl_tabskip: tab focus to next item.
@INPUT res_win_t widget: the topog widget.
@INPUT int skip: the skip code, it can be WD_TAB_RIGHT, WD_TAB_LEFT, WD_TAB_UP, WD_TAB_DOWN, WD_TAB_PAGEUP, WD_TAB_PAGEDOWN, WD_TAB_HOME, WD_TAB_END.
@RETURN void: none.
*/
XDC_API void topogctrl_tabskip(res_win_t widget, int skip);

/*
@FUNCTION topogctrl_set_focus_spot: set focus to the spot.
@INPUT res_win_t widget: the topog widget.
@INPUT link_t_ptr ilk: the spot link component.
@RETURN bool_t: return nonzero for being the spot focused, otherwise return zero.
*/
XDC_API bool_t topogctrl_set_focus_spot(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION topogctrl_get_focus_spot: get focus spot in topog widget.
@INPUT res_win_t widget: the topog widget.
@RETURN link_t_ptr: return the focused spot link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr topogctrl_get_focus_spot(res_win_t widget);

/*
@FUNCTION topogctrl_get_focus_dot: get focus dot in topog widget.
@INPUT res_win_t widget: the topog widget.
@OUTPUT int* prow: for returning zero based row index.
@OUTPUT int* pcol: for returning zero based col index.
@RETURN void: none.
*/
XDC_API void topogctrl_get_focus_dot(res_win_t widget, int* prow, int* pcol);

/*
@FUNCTION topogctrl_get_spot_rect: get spot rect int topog widget client.
@INPUT res_win_t widget: the topog widget.
@INPUT link_t_ptr flk: the spot link component.
@OUTPUT xrect_t* pxr: the rect struct using long member.
@RETURN void: none.
*/
XDC_API void topogctrl_get_spot_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

/*
@FUNCTION topogctrl_get_lock: get the topog widget is locked.
@INPUT res_win_t widget: the topog widget.
@RETURN bool_t: return nonzero for being topog locked, otherwise return zero.
*/
XDC_API bool_t topogctrl_get_lock(res_win_t widget);

/*
@FUNCTION topogctrl_set_lock: set the topog widget is locked.
@INPUT res_win_t widget: the topog widget.
@INPUT bool_t b_lock: nonzero for locking the topog, zero for unlocking.
@RETURN void: none.
*/
XDC_API void topogctrl_set_lock(res_win_t widget, bool_t b_lock);

/*
@FUNCTION topogctrl_get_dirty: get topog is dirty in design mode.
@INPUT res_win_t widget: the topog widget.
@RETURN bool_t: return nonzero for beging dirty.
*/
XDC_API bool_t topogctrl_get_dirty(res_win_t widget);

/*
@FUNCTION topogctrl_set_dirty: set topog is dirty in design mode.
@INPUT res_win_t widget: the topog widget.
@INPUT bool_t b_dirty: nonzero for setting dirty, zero for cleaning.
@RETURN void: none.
*/
XDC_API void topogctrl_set_dirty(res_win_t widget, bool_t b_dirty);

/*
@FUNCTION topogctrl_set_bitmap: set a bitmap to topog widget.
@INPUT res_win_t widget: the topog widget.
@INPUT res_bmp_t bmp: the bitmap resource handle.
@RETURN bool_t: if succeeds return nonzero, otherwise return zero.
*/
XDC_API bool_t topogctrl_set_bitmap(res_win_t widget, res_bmp_t bmp);

/********************************bitmap control***************************************************************/
typedef struct _NOTICE_PHOTO{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr anno;
	link_t_ptr arti;

	res_ctx_t rdc;
	void* data;

	int ret;
}NOTICE_PHOTO;

typedef enum{
	NC_PHOTOLBCLK,
	NC_PHOTORBCLK,
	NC_PHOTODBCLK,

	NC_PHOTOANNOCHANGING,
	NC_PHOTOANNOCHANGED,
	NC_PHOTOANNOSIZING,
	NC_PHOTOANNOSIZED,
	NC_PHOTOANNODRAG,
	NC_PHOTOANNODROP,

	NC_PHOTOANNOEDITING,
	NC_PHOTOANNOUPDATE,
	NC_PHOTOANNOCOMMIT,
	NC_PHOTOANNOROLLBACK,

	NC_PHOTOFACEDRAW
}PHOTO_NOTICE_CODE;

/*
@FUNCTION photoctrl_create: create a photo widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t photoctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION photoctrl_attach: attach a photo document to widget.
@INPUT res_win_t widget: the photo widget.
@INPUT link_t_ptr ptr: the photo document.
@RETURN void: none.
*/
XDC_API void	photoctrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION photoctrl_detach: detach the photo document from widget.
@INPUT res_win_t widget: the photo widget.
@RETURN link_t_ptr: the photo link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr photoctrl_detach(res_win_t widget);

/*
@FUNCTION photoctrl_fetch: get the photo document from widget.
@INPUT res_win_t widget: the photo widget.
@RETURN link_t_ptr: the photo link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr photoctrl_fetch(res_win_t widget);

/*
@FUNCTION photoctrl_redraw: redraw photo widget.
@INPUT res_win_t widget: the photo widget.
@RETURN void: none.
*/
XDC_API void photoctrl_redraw(res_win_t widget);

/*
@FUNCTION photoctrl_set_focus_arti: set focus to the arti.
@INPUT res_win_t widget: the photo widget.
@INPUT link_t_ptr ilk: the arti link component.
@RETURN bool_t: return nonzero for being the arti focused, otherwise return zero.
*/
XDC_API bool_t photoctrl_set_focus_arti(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION photoctrl_get_focus_arti: get focus arti in photo widget.
@INPUT res_win_t widget: the photo widget.
@RETURN link_t_ptr: return the focused arti link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr photoctrl_get_focus_arti(res_win_t widget);

/*
@FUNCTION photoctrl_get_lock: get the photo widget is locked.
@INPUT res_win_t widget: the photo widget.
@RETURN bool_t: return nonzero for being photo locked, otherwise return zero.
*/
XDC_API bool_t photoctrl_get_lock(res_win_t widget);

/*
@FUNCTION photoctrl_set_lock: set the photo widget is locked.
@INPUT res_win_t widget: the photo widget.
@INPUT bool_t b_lock: nonzero for locking the photo, zero for unlocking.
@RETURN void: none.
*/
XDC_API void photoctrl_set_lock(res_win_t widget, bool_t b_lock);

/*
@FUNCTION photoctrl_get_dirty: get photo is dirty.
@INPUT res_win_t widget: the photo widget.
@RETURN bool_t: return nonzero for beging dirty.
*/
XDC_API bool_t photoctrl_get_dirty(res_win_t widget);

/*
@FUNCTION photoctrl_set_dirty: set photo is dirty in design mode.
@INPUT res_win_t widget: the photo widget.
@INPUT bool_t b_dirty: nonzero for setting dirty, zero for cleaning.
@RETURN void: none.
*/
XDC_API void photoctrl_set_dirty(res_win_t widget, bool_t bDirty);

/*
@FUNCTION photoctrl_set_bitmap: set a bitmap bytes data to photo widget.
@INPUT res_win_t widget: the photo widget.
@INPUT const byte_t* data: the bitmap bytes data.
@INPUT dword_t size: the bytes of bitmap data.
@RETURN void: none.
*/
XDC_API void photoctrl_set_bitmap(res_win_t widget, const byte_t* data, dword_t size);

/*
@FUNCTION photoctrl_get_bitmap: copy the bitmap bytes data from photo widget.
@INPUT res_win_t widget: the photo widget.
@OUTPUT byte_t* buf: the bytes buffer.
@INPUT dword_t max: the buffer size in bytes.
@RETURN dword_t: return the bytes copyed.
*/
XDC_API dword_t photoctrl_get_bitmap(res_win_t widget, byte_t* buf, dword_t max);

/*
@FUNCTION photoctrl_commit: commit and merge arti into bitmap.
@INPUT res_win_t widget: the photo widget.
@RETURN void: none.
*/
XDC_API void photoctrl_commit(res_win_t widget);

/***********************************table control*******************************************************************/
typedef struct _NOTICE_TABLE{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr table;
	link_t_ptr item;
	bool_t onkey;
	void* data;

	int ret;
}NOTICE_TABLE;

typedef enum{

	NC_TABLELBCLK,
	NC_TABLERBCLK,
	NC_TABLEDBCLK,

	NC_TABLEITEMINSERT,
	NC_TABLEITEMDELETE,

	NC_TABLEITEMCHANGING,
	NC_TABLEITEMCHANGED,

	NC_TABLEITEMEDITING,
	NC_TABLEITEMUPDATE,
	NC_TABLEITEMCOMMIT,
	NC_TABLEITEMROLLBACK,

	NC_TABLECALCED,
	NC_TABLEITEMCALCED
}TABLE_NOTICE_CODE;

/*
@FUNCTION tablectrl_create: create a string table widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t tablectrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION tablectrl_attach: attach a string table to widget.
@INPUT res_win_t widget: the string table widget.
@INPUT link_t_ptr ptr: the string table.
@RETURN void: none.
*/
XDC_API void	tablectrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION tablectrl_detach: detach the string table from widget.
@INPUT res_win_t widget: the table widget.
@RETURN link_t_ptr: the string table link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr tablectrl_detach(res_win_t widget);

/*
@FUNCTION tablectrl_fetch: get the string table from widget.
@INPUT res_win_t widget: the table widget.
@RETURN link_t_ptr: the string table link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr tablectrl_fetch(res_win_t widget);

/*
@FUNCTION tablectrl_accept: accept or discard current editor value in table widget.
@INPUT res_win_t widget: the table widget.
@INPUT bool_t b_accept: nonzero for accepting the editor value, zero for discarding.
@RETURN void: none.
*/
XDC_API void	tablectrl_accept(res_win_t widget, bool_t b_accept);

/*
@FUNCTION tablectrl_is_update: test string entities is updated.
@INPUT res_win_t widget: the table widget.
@RETURN bool_t: return nonzero if some entities content is updated.
*/
XDC_API bool_t	tablectrl_is_update(res_win_t widget);

/*
@FUNCTION tablectrl_redraw: redraw table widget.
@INPUT res_win_t widget: the table widget.
@RETURN void: none.
*/
XDC_API void	tablectrl_redraw(res_win_t widget);

/*
@FUNCTION tablectrl_redraw_item: redraw the item in table widget.
@INPUT res_win_t widget: the table widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN void: none.
*/
XDC_API void	tablectrl_redraw_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION tablectrl_tabskip: tab focus to next item.
@INPUT res_win_t widget: the table widget.
@INPUT int skip: the skip code, it can be WD_TAB_RIGHT, WD_TAB_LEFT, WD_TAB_UP, WD_TAB_DOWN, WD_TAB_PAGEUP, WD_TAB_PAGEDOWN, WD_TAB_HOME, WD_TAB_END.
@RETURN void: none.
*/
XDC_API void	tablectrl_tabskip(res_win_t widget, int skip);

/*
@FUNCTION tablectrl_set_focus_item: set focus to the item.
@INPUT res_win_t widget: the table widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being the item focused, otherwise return zero.
*/
XDC_API bool_t	tablectrl_set_focus_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION tablectrl_get_focus_item: get focus item in table widget.
@INPUT res_win_t widget: the table widget.
@RETURN link_t_ptr: return the focused item link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr tablectrl_get_focus_item(res_win_t widget);

/*
@FUNCTION tablectrl_get_lock: get the table widget is locked.
@INPUT res_win_t widget: the table widget.
@RETURN bool_t: return nonzero for being table locked, otherwise return zero.
*/
XDC_API bool_t	tablectrl_get_lock(res_win_t widget);

/*
@FUNCTION tablectrl_set_lock: set the table widget is locked.
@INPUT res_win_t widget: the table widget.
@INPUT bool_t b_lock: nonzero for locking the table, zero for unlocking.
@RETURN void: none.
*/
XDC_API void	tablectrl_set_lock(res_win_t widget, bool_t b_lock);

/*
@FUNCTION tablectrl_auto_insert: set the table add a new item automatic.
@INPUT res_win_t widget: the table widget.
@INPUT bool_t b_auto: nonzero for automatic.
@RETURN void: none.
*/
XDC_API void	tablectrl_auto_insert(res_win_t widget, bool_t b_auto);

/*
@FUNCTION tablectrl_delete_item: delete a item in table widget.
@INPUT res_win_t widget: the table widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDC_API bool_t	tablectrl_delete_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION tablectrl_insert_item: add a new item into table widget.
@INPUT res_win_t widget: the table widget.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: if succeeds return new item link component, fails return NULL.
*/
XDC_API link_t_ptr tablectrl_insert_item(res_win_t widget, link_t_ptr pos);

/*
@FUNCTION tablectrl_set_item_key_text: set the table item key value.
@INPUT res_win_t widget: the table widget.
@INPUT link_t_ptr ilk: the item link component.
@INPUT const tchar_t* key: the key token.
@RETURN void: none.
*/
XDC_API void	tablectrl_set_item_key_text(res_win_t widget, link_t_ptr ilk, const tchar_t* key);

/*
@FUNCTION tablectrl_set_item_val_text: set the table item text value.
@INPUT res_win_t widget: the table widget.
@INPUT link_t_ptr ilk: the item link component.
@INPUT const tchar_t* val: the value token.
@RETURN void: none.
*/
XDC_API void	tablectrl_set_item_val_text(res_win_t widget, link_t_ptr ilk, const tchar_t* val);

/*
@FUNCTION tablectrl_set_ratio: set the table item key and value column width ratio.
@INPUT res_win_t widget: the table widget.
@INPUT float r: the ratio value.
@RETURN void: none.
*/
XDC_API void	tablectrl_set_ratio(res_win_t widget, float r);

/*
@FUNCTION tablectrl_get_ratio: get the table item key and value column width ratio.
@INPUT res_win_t widget: the table widget.
@RETURN float: return the ratio value.
*/
XDC_API float	tablectrl_get_ratio(res_win_t widget);

/*********************************label control**************************************************************/

typedef struct _NOTICE_LABEL{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr label;
	link_t_ptr item;
	void* data;

	int ret;
}NOTICE_LABEL;


typedef enum{

	NC_LABELLBCLK,
	NC_LABELRBCLK,
	NC_LABELDBCLK,

	NC_LABELITEMHOVER,

	NC_LABELITEMCHANGING,
	NC_LABELITEMCHANGED,

	NC_LABELITEMDRAG,
	NC_LABELITEMDROP,

	NC_LABELCALCED,
	NC_LABELITEMCALCED
}LABEL_NOTICE_CODE;

/*
@FUNCTION labelctrl_create: create a label widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t labelctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION labelctrl_attach: attach a label document to widget.
@INPUT res_win_t widget: the label widget.
@INPUT link_t_ptr ptr: the label document.
@RETURN void: none.
*/
XDC_API void	labelctrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION labelctrl_detach: detach the label document from widget.
@INPUT res_win_t widget: the label widget.
@RETURN link_t_ptr: the label link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr labelctrl_detach(res_win_t widget);

/*
@FUNCTION labelctrl_fetch: get the label document from widget.
@INPUT res_win_t widget: the label widget.
@RETURN link_t_ptr: the label link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr labelctrl_fetch(res_win_t widget);

/*
@FUNCTION labelctrl_redraw: redraw label widget.
@INPUT res_win_t widget: the label widget.
@RETURN void: none.
*/
XDC_API void	labelctrl_redraw(res_win_t widget);

/*
@FUNCTION labelctrl_redraw_item: redraw the item in label widget.
@INPUT res_win_t widget: the label widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN void: none.
*/
XDC_API void	labelctrl_redraw_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION labelctrl_tabskip: tab focus to next item.
@INPUT res_win_t widget: the label widget.
@INPUT int skip: the skip code, it can be WD_TAB_RIGHT, WD_TAB_LEFT, WD_TAB_UP, WD_TAB_DOWN, WD_TAB_PAGEUP, WD_TAB_PAGEDOWN, WD_TAB_HOME, WD_TAB_END.
@RETURN void: none.
*/
XDC_API void	labelctrl_tabskip(res_win_t widget, int skip);

/*
@FUNCTION labelctrl_set_focus_item: set focus to the item.
@INPUT res_win_t widget: the label widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being the item focused, otherwise return zero.
*/
XDC_API bool_t	labelctrl_set_focus_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION labelctrl_get_focus_item: get focus item in label widget.
@INPUT res_win_t widget: the label widget.
@RETURN link_t_ptr: return the focused item link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr labelctrl_get_focus_item(res_win_t widget);

/*
@FUNCTION labelctrl_get_item_rect: get item rect in label widget client.
@INPUT res_win_t widget: the label widget.
@INPUT link_t_ptr flk: the item link component.
@OUTPUT xrect_t* pxr: the rect struct using long member.
@RETURN void: none.
*/
XDC_API void	labelctrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

/*
@FUNCTION labelctrl_move_to_page: move to the page.
@INPUT res_win_t widget: the label widget.
@INPUT int page: the 1-based page index.
@RETURN void: none.
*/
XDC_API void	labelctrl_move_to_page(res_win_t widget, int page);

/*
@FUNCTION labelctrl_move_first_page: move to the first page.
@INPUT res_win_t widget: the label widget.
@RETURN void: none.
*/
XDC_API void	labelctrl_move_first_page(res_win_t widget);

/*
@FUNCTION labelctrl_move_prev_page: move to the previous page.
@INPUT res_win_t widget: the label widget.
@RETURN void: none.
*/
XDC_API void	labelctrl_move_prev_page(res_win_t widget);

/*
@FUNCTION labelctrl_move_next_page: move to the next page.
@INPUT res_win_t widget: the label widget.
@RETURN void: none.
*/
XDC_API void	labelctrl_move_next_page(res_win_t widget);

/*
@FUNCTION labelctrl_move_last_page: move to the last page.
@INPUT res_win_t widget: the label widget.
@RETURN void: none.
*/
XDC_API void	labelctrl_move_last_page(res_win_t widget);

/*
@FUNCTION labelctrl_get_cur_page: get current page.
@INPUT res_win_t widget: the label widget.
@RETURN int: return 1-based page index.
*/
XDC_API int		labelctrl_get_cur_page(res_win_t widget);

/*
@FUNCTION labelctrl_get_max_page: get maximized page.
@INPUT res_win_t widget: the label widget.
@RETURN int: return the total pages.
*/
XDC_API int		labelctrl_get_max_page(res_win_t widget);

/*
@FUNCTION labelctrl_popup_size: calcing the label popup size according to client size.
@INPUT res_win_t widget: the label widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void	labelctrl_popup_size(res_win_t widget, xsize_t* pse);

/********************************list control************************************************************/
typedef struct _NOTICE_LIST{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr list;
	link_t_ptr item;
	void* data;

	int ret;
}NOTICE_LIST;

typedef enum{

	NC_LISTLBCLK,
	NC_LISTRBCLK,
	NC_LISTDBCLK,

	NC_LISTITEMHOVER,

	NC_LISTITEMCHANGING,
	NC_LISTITEMCHANGED,
	NC_LISTITEMCHECKED,

	NC_LISTITEMEDITING,
	NC_LISTITEMUPDATE,
	NC_LISTITEMCOMMIT,
	NC_LISTITEMROLLBACK,

	NC_LISTITEMEXPAND,
	NC_LISTITEMCOLLAPSE,
	NC_LISTITEMDRAG,
	NC_LISTITEMDROP,

	NC_LISTCALCED,
	NC_LISTITEMCALCED
}LIST_NOTICE_CODE;

/*
@FUNCTION listctrl_create: create a list widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t listctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION listctrl_attach: attach a list document to widget.
@INPUT res_win_t widget: the list widget.
@INPUT link_t_ptr ptr: the list document.
@RETURN void: none.
*/
XDC_API void	listctrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION listctrl_detach: detach the list document from widget.
@INPUT res_win_t widget: the list widget.
@RETURN link_t_ptr: the list link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr	listctrl_detach(res_win_t widget);

/*
@FUNCTION listctrl_fetch: get the list document from widget.
@INPUT res_win_t widget: the list widget.
@RETURN link_t_ptr: the list link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr	listctrl_fetch(res_win_t widget);

/*
@FUNCTION listctrl_accept: accept or discard current editor value in list widget.
@INPUT res_win_t widget: the list widget.
@INPUT bool_t b_accept: nonzero for accepting the editor value, zero for discarding.
@RETURN void: none.
*/
XDC_API void	listctrl_accept(res_win_t widget, bool_t b_accept);

/*
@FUNCTION listctrl_redraw: redraw list widget.
@INPUT res_win_t widget: the list widget.
@RETURN void: none.
*/
XDC_API void	listctrl_redraw(res_win_t widget);

/*
@FUNCTION listctrl_redraw_item: redraw the item in list widget.
@INPUT res_win_t widget: the list widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN void: none.
*/
XDC_API void	listctrl_redraw_item(res_win_t widget, link_t_ptr plk);

/*
@FUNCTION listctrl_tabskip: tab focus to next item.
@INPUT res_win_t widget: the list widget.
@INPUT int skip: the skip code, it can be WD_TAB_RIGHT, WD_TAB_LEFT, WD_TAB_UP, WD_TAB_DOWN, WD_TAB_PAGEUP, WD_TAB_PAGEDOWN, WD_TAB_HOME, WD_TAB_END.
@RETURN void: none.
*/
XDC_API void	listctrl_tabskip(res_win_t widget, int nSkip);

/*
@FUNCTION listctrl_set_focus_item: set focus to the item.
@INPUT res_win_t widget: the list widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being the item focused, otherwise return zero.
*/
XDC_API bool_t	listctrl_set_focus_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION listctrl_get_focus_item: get focus item in list widget.
@INPUT res_win_t widget: the list widget.
@RETURN link_t_ptr: return the focused item link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr	listctrl_get_focus_item(res_win_t widget);

/*
@FUNCTION listctrl_get_item_rect: get item rect in list widget client.
@INPUT res_win_t widget: the list widget.
@INPUT link_t_ptr ilk: the item link component.
@OUTPUT xrect_t* pxr: the rect struct using long member.
@RETURN void: none.
*/
XDC_API void	listctrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

/*
@FUNCTION listctrl_get_lock: get the list widget is locked.
@INPUT res_win_t widget: the list widget.
@RETURN bool_t: return nonzero for being list locked, otherwise return zero.
*/
XDC_API bool_t	listctrl_get_lock(res_win_t widget);

/*
@FUNCTION listctrl_set_lock: set the list widget is locked.
@INPUT res_win_t widget: the list widget.
@INPUT bool_t b_lock: nonzero for locking the list, zero for unlocking.
@RETURN void: none.
*/
XDC_API void	listctrl_set_lock(res_win_t widget, bool_t b_lock);

/*
@FUNCTION listctrl_set_item_title: set the list item text.
@INPUT res_win_t widget: the list widget.
@INPUT link_t_ptr ilk: the item link component.
@INPUT const tchar_t* sz_text: the text token.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDC_API bool_t listctrl_set_item_title(res_win_t widget, link_t_ptr ilk, const tchar_t* sz_text);

/*
@FUNCTION listctrl_find: find the list item from position.
@INPUT res_win_t widget: the list widget.
@INPUT const tchar_t* token: the text token.
@RETURN void: none.
*/
XDC_API void	listctrl_find(res_win_t widget, const tchar_t* token);

/*
@FUNCTION listctrl_popup_size: calcing the list popup size according to client size.
@INPUT res_win_t widget: the list widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void	listctrl_popup_size(res_win_t widget, xsize_t* pse);

/**************************proper control*************************************************************/
typedef struct _NOTICE_PROPER{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr proper;
	link_t_ptr section;
	link_t_ptr entity;
	void* data;

	int ret;
}NOTICE_PROPER;

typedef enum{

	NC_PROPERLBCLK,
	NC_PROPERRBCLK,
	NC_PROPERDBCLK,

	NC_ENTITYHOVER,

	NC_ENTITYCHANGING,
	NC_ENTITYCHANGED,

	NC_ENTITYEDITING,
	NC_ENTITYUPDATE,
	NC_ENTITYCOMMIT,
	NC_ENTITYROLLBACK,

	NC_PROPERCALCED,
	NC_SECTIONCALCED,
	NC_ENTITYCALCED
}PROPER_NOTICE_CODE;

/*
@FUNCTION properctrl_create: create a proper widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t properctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION properctrl_attach: attach a proper document to widget.
@INPUT res_win_t widget: the proper widget.
@INPUT link_t_ptr ptr: the proper document.
@RETURN void: none.
*/
XDC_API void	properctrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION properctrl_detach: detach the proper document from widget.
@INPUT res_win_t widget: the proper widget.
@RETURN link_t_ptr: the proper link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr properctrl_detach(res_win_t widget);

/*
@FUNCTION properctrl_fetch: get the proper document from widget.
@INPUT res_win_t widget: the proper widget.
@RETURN link_t_ptr: the proper link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr properctrl_fetch(res_win_t widget);

/*
@FUNCTION properctrl_accept: accept or discard current editor value in proper widget.
@INPUT res_win_t widget: the proper widget.
@INPUT bool_t b_accept: nonzero for accepting the editor value, zero for discarding.
@RETURN void: none.
*/
XDC_API void	properctrl_accept(res_win_t widget, bool_t b_accept);

/*
@FUNCTION properctrl_redraw: redraw proper widget.
@INPUT res_win_t widget: the proper widget.
@RETURN void: none.
*/
XDC_API void	properctrl_redraw(res_win_t widget);

/*
@FUNCTION properctrl_redraw_entity: redraw the entity in proper widget.
@INPUT res_win_t widget: the proper widget.
@INPUT link_t_ptr elk: the entity link component.
@RETURN void: none.
*/
XDC_API void	properctrl_redraw_entity(res_win_t widget, link_t_ptr elk);

/*
@FUNCTION properctrl_redraw_section: redraw the section in proper widget.
@INPUT res_win_t widget: the proper widget.
@INPUT link_t_ptr elk: the section link component.
@RETURN void: none.
*/
XDC_API void	properctrl_redraw_section(res_win_t widget, link_t_ptr slk);

/*
@FUNCTION properctrl_tabskip: tab focus to next entity.
@INPUT res_win_t widget: the proper widget.
@INPUT int skip: the skip code, it can be WD_TAB_RIGHT, WD_TAB_LEFT, WD_TAB_UP, WD_TAB_DOWN, WD_TAB_PAGEUP, WD_TAB_PAGEDOWN, WD_TAB_HOME, WD_TAB_END.
@RETURN void: none.
*/
XDC_API void	properctrl_tabskip(res_win_t widget, int skip);

/*
@FUNCTION properctrl_set_focus_entity: set focus to the entity.
@INPUT res_win_t widget: the proper widget.
@INPUT link_t_ptr ilk: the entity link component.
@RETURN bool_t: return nonzero for being the entity focused, otherwise return zero.
*/
XDC_API bool_t	properctrl_set_focus_entity(res_win_t widget, link_t_ptr elk);

/*
@FUNCTION properctrl_get_focus_entity: get focus entity in list widget.
@INPUT res_win_t widget: the proper widget.
@RETURN link_t_ptr: return the focused entity link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr properctrl_get_focus_entity(res_win_t widget);

/*
@FUNCTION properctrl_get_entity_rect: get entity rect in proper widget client.
@INPUT res_win_t widget: the proper widget.
@INPUT link_t_ptr ilk: the entity link component.
@OUTPUT xrect_t* pxr: the rect struct using long member.
@RETURN void: none.
*/
XDC_API void	properctrl_get_entity_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

/*
@FUNCTION properctrl_get_lock: get the proper widget is locked.
@INPUT res_win_t widget: the proper widget.
@RETURN bool_t: return nonzero for being proper locked, otherwise return zero.
*/
XDC_API bool_t	properctrl_get_lock(res_win_t widget);

/*
@FUNCTION properctrl_set_lock: set the proper widget is locked.
@INPUT res_win_t widget: the proper widget.
@INPUT bool_t b_lock: nonzero for locking the proper, zero for unlocking.
@RETURN void: none.
*/
XDC_API void	properctrl_set_lock(res_win_t widget, bool_t bLock);

/*
@FUNCTION properctrl_set_entity_value: set the proper entity text.
@INPUT res_win_t widget: the proper widget.
@INPUT link_t_ptr elk: the entity link component.
@INPUT const tchar_t* sz_text: the text token.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDC_API bool_t	properctrl_set_entity_value(res_win_t widget, link_t_ptr elk, const tchar_t* token);

/***************************************status control******************************************************************/
typedef struct _NOTICE_STATUS{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr status;
	link_t_ptr item;
	void* data;

	int ret;
}NOTICE_STATUS;

typedef enum{

	NC_STATUSLBCLK,
	NC_STATUSRBCLK,
	NC_STATUSDBCLK,

	NC_STATUSITEMHOVER,

	NC_STATUSCALCED,
	NC_STATUSITEMCALCED
}STATUS_NOTICE_CODE;

/*
@FUNCTION statusctrl_create: create a status widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t statusctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION statusctrl_attach: attach a status document to widget.
@INPUT res_win_t widget: the status widget.
@INPUT link_t_ptr ptr: the status document.
@RETURN void: none.
*/
XDC_API void	statusctrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION statusctrl_detach: detach the status document from widget.
@INPUT res_win_t widget: the status widget.
@RETURN link_t_ptr: the status link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr statusctrl_detach(res_win_t widget);

/*
@FUNCTION statusctrl_fetch: get the status document from widget.
@INPUT res_win_t widget: the status widget.
@RETURN link_t_ptr: the status link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr statusctrl_fetch(res_win_t widget);

/*
@FUNCTION statusctrl_redraw: redraw status widget.
@INPUT res_win_t widget: the status widget.
@RETURN void: none.
*/
XDC_API void	statusctrl_redraw(res_win_t widget);

/*
@FUNCTION statusctrl_redraw_title: redraw the title in status widget.
@INPUT res_win_t widget: the status widget.
@RETURN void: none.
*/
XDC_API void	statusctrl_redraw_title(res_win_t widget);

/*
@FUNCTION statusctrl_redraw_item: redraw the item in status widget.
@INPUT res_win_t widget: the status widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN void: none.
*/
XDC_API void	statusctrl_redraw_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION statusctrl_tabskip: tab focus to next item.
@INPUT res_win_t widget: the status widget.
@INPUT int skip: the skip code, it can be WD_TAB_RIGHT, WD_TAB_LEFT, WD_TAB_UP, WD_TAB_DOWN, WD_TAB_PAGEUP, WD_TAB_PAGEDOWN, WD_TAB_HOME, WD_TAB_END.
@RETURN void: none.
*/
XDC_API void	statusctrl_tabskip(res_win_t widget, int skip);

/*
@FUNCTION statusctrl_set_focus_item: set focus to the item.
@INPUT res_win_t widget: the status widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being the item focused, otherwise return zero.
*/
XDC_API bool_t	statusctrl_set_focus_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION statusctrl_get_focus_item: get focus item in status widget.
@INPUT res_win_t widget: the status widget.
@RETURN link_t_ptr: return the focused item link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr statusctrl_get_focus_item(res_win_t widget);

/*
@FUNCTION statusctrl_get_item_rect: get item rect in status widget client.
@INPUT res_win_t widget: the status widget.
@INPUT link_t_ptr ilk: the item link component.
@OUTPUT xrect_t* pxr: the rect struct using long member.
@RETURN void: none.
*/
XDC_API void	statusctrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

/*
@FUNCTION statusctrl_show_step: show or hide the step bar in stattus widget.
@INPUT res_win_t widget: the status widget.
@INPUT bool_t b_show: nonzero for showing, zero for hiding.
@RETURN void: none.
*/
XDC_API void	statusctrl_show_step(res_win_t widget, bool_t b_show);

/*
@FUNCTION statusctrl_step_it: add some step to status step bar.
@INPUT res_win_t widget: the status widget.
@INPUT int steps: the steps.
@INPUT const tchar_t* sz_step: the step text.
@RETURN void: none.
*/
XDC_API void	statusctrl_step_it(res_win_t widget, int steps, const tchar_t* sz_step);

/*******************************message control************************************************************/
typedef struct _NOTICE_NOTES{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr arch;
	link_t_ptr item;
	void* data;

	int ret;
}NOTICE_NOTES;

typedef enum{
	NC_NOTESLBCLK,
	NC_NOTESRBCLK,
	NC_NOTESDBCLK,

	NC_NOTESITEMHOVER,

	NC_NOTESITEMCHANGING,
	NC_NOTESITEMCHANGED,
	NC_NOTESITEMDELETE,

	NC_NOTESCALCED,
	NC_NOTESITEMCALCED
}NOTES_NOTICE_CODE;

/*
@FUNCTION notesctrl_create: create a notes widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t notesctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION notesctrl_attach: attach a notes document to widget.
@INPUT res_win_t widget: the notes widget.
@INPUT link_t_ptr ptr: the notes document.
@RETURN void: none.
*/
XDC_API void	notesctrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION notesctrl_detach: detach the notes document from widget.
@INPUT res_win_t widget: the notes widget.
@RETURN link_t_ptr: the notes link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr notesctrl_detach(res_win_t widget);

/*
@FUNCTION notesctrl_fetch: get the notes document from widget.
@INPUT res_win_t widget: the notes widget.
@RETURN link_t_ptr: the notes link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr notesctrl_fetch(res_win_t widget);

/*
@FUNCTION notesctrl_redraw: redraw notes widget.
@INPUT res_win_t widget: the notes widget.
@RETURN void: none.
*/
XDC_API void	notesctrl_redraw(res_win_t widget);

/*
@FUNCTION notesctrl_redraw_item: redraw the item in notes widget.
@INPUT res_win_t widget: the notes widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN void: none.
*/
XDC_API void	notesctrl_redraw_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION notesctrl_tabskip: tab focus to next item.
@INPUT res_win_t widget: the notes widget.
@INPUT int skip: the skip code, it can be WD_TAB_RIGHT, WD_TAB_LEFT, WD_TAB_UP, WD_TAB_DOWN, WD_TAB_PAGEUP, WD_TAB_PAGEDOWN, WD_TAB_HOME, WD_TAB_END.
@RETURN void: none.
*/
XDC_API void	notesctrl_tabskip(res_win_t widget, int skip);

/*
@FUNCTION notesctrl_set_focus_item: set focus to the item.
@INPUT res_win_t widget: the notes widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being the item focused, otherwise return zero.
*/
XDC_API bool_t	notesctrl_set_focus_item(res_win_t widget, link_t_ptr ent);

/*
@FUNCTION notesctrl_get_focus_item: get focus item in notes widget.
@INPUT res_win_t widget: the notes widget.
@RETURN link_t_ptr: return the focused item link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr notesctrl_get_focus_item(res_win_t widget);

/*
@FUNCTION notesctrl_get_item_rect: get item rect in notes widget client.
@INPUT res_win_t widget: the notes widget.
@INPUT link_t_ptr ilk: the item link component.
@OUTPUT xrect_t* pxr: the rect struct using long member.
@RETURN void: none.
*/
XDC_API void	notesctrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

/*************************************title control***********************************************************/

typedef struct _NOTICE_TITLE{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr title;
	link_t_ptr item;
	void* data;

	int ret;
}NOTICE_TITLE;

typedef enum{

	NC_TITLELBCLK,
	NC_TITLERBCLK,
	NC_TITLEDBCLK,

	NC_TITLEITEMHOVER,

	NC_TITLEITEMCHANGING,
	NC_TITLEITEMCHANGED,

	NC_TITLEITEMINSERT,
	NC_TITLEITEMDELETE,

	NC_TITLECALCED,
	NC_TITLEITEMCALCED
}TITLE_NOTICE_CODE;

/*
@FUNCTION titlectrl_create: create a title widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t titlectrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION titlectrl_attach: attach a title document to widget.
@INPUT res_win_t widget: the title widget.
@INPUT link_t_ptr ptr: the title document.
@RETURN void: none.
*/
XDC_API void	titlectrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION titlectrl_detach: detach the title document from widget.
@INPUT res_win_t widget: the title widget.
@RETURN link_t_ptr: the title link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr titlectrl_detach(res_win_t widget);

/*
@FUNCTION titlectrl_fetch: get the title document from widget.
@INPUT res_win_t widget: the title widget.
@RETURN link_t_ptr: the title link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr titlectrl_fetch(res_win_t widget);

/*
@FUNCTION titlectrl_redraw: redraw title widget.
@INPUT res_win_t widget: the title widget.
@RETURN void: none.
*/
XDC_API void	titlectrl_redraw(res_win_t widget);

/*
@FUNCTION titlectrl_redraw_item: redraw the item in title widget.
@INPUT res_win_t widget: the title widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN void: none.
*/
XDC_API void	titlectrl_redraw_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION titlectrl_tabskip: tab focus to next item.
@INPUT res_win_t widget: the title widget.
@INPUT int skip: the skip code, it can be WD_TAB_RIGHT, WD_TAB_LEFT, WD_TAB_UP, WD_TAB_DOWN, WD_TAB_PAGEUP, WD_TAB_PAGEDOWN, WD_TAB_HOME, WD_TAB_END.
@RETURN void: none.
*/
XDC_API void	titlectrl_tabskip(res_win_t widget, int skip);

/*
@FUNCTION titlectrl_set_focus_item: set focus to the item.
@INPUT res_win_t widget: the title widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being the item focused, otherwise return zero.
*/
XDC_API bool_t	titlectrl_set_focus_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION titlectrl_get_focus_item: get focus item in title widget.
@INPUT res_win_t widget: the title widget.
@RETURN link_t_ptr: return the focused item link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr titlectrl_get_focus_item(res_win_t widget);

/*
@FUNCTION titlectrl_get_item_rect: get item rect in title widget client.
@INPUT res_win_t widget: the title widget.
@INPUT link_t_ptr ilk: the item link component.
@OUTPUT xrect_t* pxr: the rect struct using long member.
@RETURN void: none.
*/
XDC_API void	titlectrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

/*
@FUNCTION titlectrl_insert_item: add a new item into title.
@INPUT res_win_t widget: the title widget.
@INPUT link_t_ptr pos: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: if succeeds return the new item link component, fails return NULL.
*/
XDC_API link_t_ptr titlectrl_insert_item(res_win_t widget, link_t_ptr pos);

/*
@FUNCTION titlectrl_delete_item: delete the item.
@INPUT res_win_t widget: the title widget.
@INPUT link_t_ptr pos: the item link component.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDC_API bool_t	titlectrl_delete_item(res_win_t widget, link_t_ptr ilk);

/*************************************title control******************************************************/
typedef struct _NOTICE_TOOL{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr tool;
	link_t_ptr item;
	void* data;

	int ret;
}NOTICE_TOOL;

typedef enum{

	NC_TOOLLBCLK,
	NC_TOOLRBCLK,
	NC_TOOLDBCLK,

	NC_TOOLITEMHOVER,

	NC_TOOLCALCED,
	NC_TOOLITEMCALCED
}TOOL_NOTICE_CODE;

/*
@FUNCTION toolctrl_create: create a tool widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t toolctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION toolctrl_attach: attach a tool document to widget.
@INPUT res_win_t widget: the tool widget.
@INPUT link_t_ptr ptr: the tool document.
@RETURN void: none.
*/
XDC_API void	toolctrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION toolctrl_detach: detach the tool document from widget.
@INPUT res_win_t widget: the tool widget.
@RETURN link_t_ptr: the tool link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr toolctrl_detach(res_win_t widget);

/*
@FUNCTION toolctrl_fetch: get the tool document from widget.
@INPUT res_win_t widget: the tool widget.
@RETURN link_t_ptr: the tool link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr toolctrl_fetch(res_win_t widget);

/*
@FUNCTION toolctrl_redraw: redraw tool widget.
@INPUT res_win_t widget: the tool widget.
@RETURN void: none.
*/
XDC_API void	toolctrl_redraw(res_win_t widget);

/*
@FUNCTION toolctrl_redraw_item: redraw the item in tool widget.
@INPUT res_win_t widget: the tool widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN void: none.
*/
XDC_API void	toolctrl_redraw_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION toolctrl_tabskip: tab focus to next item.
@INPUT res_win_t widget: the tool widget.
@INPUT int skip: the skip code, it can be WD_TAB_RIGHT, WD_TAB_LEFT, WD_TAB_UP, WD_TAB_DOWN, WD_TAB_PAGEUP, WD_TAB_PAGEDOWN, WD_TAB_HOME, WD_TAB_END.
@RETURN void: none.
*/
XDC_API void	toolctrl_tabskip(res_win_t widget, int nSkip);

/*
@FUNCTION toolctrl_set_focus_item: set focus to the item.
@INPUT res_win_t widget: the tool widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being the item focused, otherwise return zero.
*/
XDC_API bool_t	toolctrl_set_focus_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION toolctrl_get_focus_item: get focus item in tool widget.
@INPUT res_win_t widget: the tool widget.
@RETURN link_t_ptr: return the focused item link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr toolctrl_get_focus_item(res_win_t widget);

/*
@FUNCTION toolctrl_get_item_rect: get item rect in tool widget client.
@INPUT res_win_t widget: the tool widget.
@INPUT link_t_ptr ilk: the item link component.
@OUTPUT xrect_t* pxr: the rect struct using long member.
@RETURN void: none.
*/
XDC_API void	toolctrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

/*************************************tree control****************************************************/
typedef struct _NOTICE_TREE{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr tree;
	link_t_ptr item;
	void* data;

	int ret;
}NOTICE_TREE;

typedef enum{

	NC_TREELBCLK,
	NC_TREERBCLK,
	NC_TREEDBCLK,

	NC_TREEITEMHOVER,

	NC_TREEITEMCHANGING,
	NC_TREEITEMCHANGED,
	NC_TREEITEMCHECKED,

	NC_TREEITEMEDITING,
	NC_TREEITEMUPDATE,
	NC_TREEITEMCOMMIT,
	NC_TREEITEMROLLBACK,

	NC_TREEITEMEXPAND,
	NC_TREEITEMCOLLAPSE,

	NC_TREEITEMDRAG,
	NC_TREEITEMDROP,

	NC_TREECALCED,
	NC_TREEITEMCALCED
}TREE_NOTICE_CODE;

/*
@FUNCTION treectrl_create: create a tree widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t treectrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION treectrl_attach: attach a tree document to widget.
@INPUT res_win_t widget: the tree widget.
@INPUT link_t_ptr ptr: the tree document.
@RETURN void: none.
*/
XDC_API void	treectrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION treectrl_detach: detach the tree document from widget.
@INPUT res_win_t widget: the tree widget.
@RETURN link_t_ptr: the tree link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr treectrl_detach(res_win_t widget);

/*
@FUNCTION treectrl_fetch: get the tree document from widget.
@INPUT res_win_t widget: the tree widget.
@RETURN link_t_ptr: the tree link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr treectrl_fetch(res_win_t widget);

/*
@FUNCTION treectrl_accept: accept or discard current editor value in tree widget.
@INPUT res_win_t widget: the tree widget.
@INPUT bool_t b_accept: nonzero for accepting the editor value, zero for discarding.
@RETURN void: none.
*/
XDC_API void	treectrl_accept(res_win_t widget, bool_t b_accept);

/*
@FUNCTION treectrl_redraw: redraw tree widget.
@INPUT res_win_t widget: the tree widget.
@RETURN void: none.
*/
XDC_API void	treectrl_redraw(res_win_t widget);

/*
@FUNCTION treectrl_redraw_item: redraw the item in tree widget.
@INPUT res_win_t widget: the tree widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN void: none.
*/
XDC_API void	treectrl_redraw_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION treectrl_tabskip: tab focus to next item.
@INPUT res_win_t widget: the tree widget.
@INPUT int skip: the skip code, it can be WD_TAB_RIGHT, WD_TAB_LEFT, WD_TAB_UP, WD_TAB_DOWN, WD_TAB_PAGEUP, WD_TAB_PAGEDOWN, WD_TAB_HOME, WD_TAB_END.
@RETURN void: none.
*/
XDC_API void	treectrl_tabskip(res_win_t widget, int nSkip);

/*
@FUNCTION treectrl_set_focus_item: set focus to the item.
@INPUT res_win_t widget: the tree widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN bool_t: return nonzero for being the item focused, otherwise return zero.
*/
XDC_API bool_t	treectrl_set_focus_item(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION treectrl_get_focus_item: get focus item in tree widget.
@INPUT res_win_t widget: the tree widget.
@RETURN link_t_ptr: return the focused item link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr treectrl_get_focus_item(res_win_t widget);

/*
@FUNCTION treectrl_get_item_rect: get item rect in tree widget client.
@INPUT res_win_t widget: the tree widget.
@INPUT link_t_ptr ilk: the item link component.
@OUTPUT xrect_t* pxr: the rect struct using long member.
@RETURN void: none.
*/
XDC_API void	treectrl_get_item_rect(res_win_t widget, link_t_ptr ilk, xrect_t* pxr);

/*
@FUNCTION treectrl_get_lock: get the tree widget is locked.
@INPUT res_win_t widget: the tree widget.
@RETURN bool_t: return nonzero for being tree locked, otherwise return zero.
*/
XDC_API bool_t	treectrl_get_lock(res_win_t widget);

/*
@FUNCTION treectrl_set_lock: set the tree widget is locked.
@INPUT res_win_t widget: the tree widget.
@INPUT bool_t b_lock: nonzero for locking the tree, zero for unlocking.
@RETURN void: none.
*/
XDC_API void	treectrl_set_lock(res_win_t widget, bool_t bLock);

/*
@FUNCTION treectrl_set_item_title: set the tree item text.
@INPUT res_win_t widget: the tree widget.
@INPUT link_t_ptr ilk: the item link component or link indicator: LINK_FIRST, LINK_LAST.
@INPUT const tchar_t* sz_text: the text token.
@RETURN bool_t: if succeeds return nonzero, fails return zero..
*/
XDC_API bool_t treectrl_set_item_title(res_win_t widget, link_t_ptr ilk, const tchar_t* sz_text);

/*
@FUNCTION treectrl_find: find a item from position.
@INPUT res_win_t widget: the tree widget.
@INPUT const tchar_t* token: the matching token.
@RETURN void: none.
*/
XDC_API void	treectrl_find(res_win_t widget, const tchar_t* token);

/*
@FUNCTION treectrl_popup_size: calcing the tree popup size according to client size.
@INPUT res_win_t widget: the tree widget.
@INOUTPUT xsize_t* pxs: the size struct for inputing client size and outputing widget size.
@RETURN void: none.
*/
XDC_API void	treectrl_popup_size(res_win_t widget, xsize_t* pse);

/**************************************rich control**************************************************/
typedef struct _NOTICE_RICH{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr rich;
	link_t_ptr anch;
	void* data;

	int ret;
}NOTICE_RICH;

typedef enum{
	NC_RICHLBCLK,
	NC_RICHRBCLK,
	NC_RICHDBCLK,

	NC_RICHATOMEDITING,
	NC_RICHATOMUPDATE,
	NC_RICHATOMCOMMIT,
	NC_RICHATOMROLLBACK,

	NC_RICHANCHUPDATE,
	NC_RICHANCHCHANGED,

	NC_RICHCALCED,
	NC_RICHANCHCALCED
}RICH_NOTICE_CODE;

/*
@FUNCTION richctrl_create: create a rich widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t richctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION richctrl_attach: attach a rich document to widget.
@INPUT res_win_t widget: the rich widget.
@INPUT link_t_ptr ptr: the rich document.
@RETURN void: none.
*/
XDC_API void	richctrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION richctrl_detach: detach the rich document from widget.
@INPUT res_win_t widget: the rich widget.
@RETURN link_t_ptr: the rich link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr richctrl_detach(res_win_t widget);

/*
@FUNCTION richctrl_fetch: get the rich document from widget.
@INPUT res_win_t widget: the rich widget.
@RETURN link_t_ptr: the rich link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr richctrl_fetch(res_win_t widget);

/*
@FUNCTION richctrl_redraw: redraw rich widget.
@INPUT res_win_t widget: the rich widget.
@RETURN void: none.
*/
XDC_API void	richctrl_redraw(res_win_t widget);

/*
@FUNCTION richctrl_set_focus_anch: set focus to the anchor.
@INPUT res_win_t widget: the rich widget.
@INPUT link_t_ptr ilk: the anchor link component.
@RETURN bool_t: return nonzero for being the anchor focused, otherwise return zero.
*/
XDC_API void richctrl_set_focus_anch(res_win_t widget, link_t_ptr nlk);

/*
@FUNCTION richctrl_get_focus_anch: get focus anchor in rich widget.
@INPUT res_win_t widget: the rich widget.
@RETURN link_t_ptr: return the focused anchor link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr richctrl_get_focus_anch(res_win_t widget);

/*
@FUNCTION richctrl_get_anch_rect: get item rect in rich widget client.
@INPUT res_win_t widget: the rich widget.
@INPUT link_t_ptr ilk: the anchor link component.
@OUTPUT xrect_t* pxr: the rect struct using long member.
@RETURN void: none.
*/
XDC_API void richctrl_get_anch_rect(res_win_t widget, link_t_ptr nlk, xrect_t* pxr);

/*
@FUNCTION richctrl_get_lock: get the rich widget is locked.
@INPUT res_win_t widget: the rich widget.
@RETURN bool_t: return nonzero for being rich locked, otherwise return zero.
*/
XDC_API bool_t richctrl_get_lock(res_win_t widget);

/*
@FUNCTION richctrl_set_lock: set the rich widget is locked.
@INPUT res_win_t widget: the rich widget.
@INPUT bool_t b_lock: nonzero for locking the rich, zero for unlocking.
@RETURN void: none.
*/
XDC_API void richctrl_set_lock(res_win_t widget, bool_t bLock);

/*
@FUNCTION richctrl_move_to_page: move to the page.
@INPUT res_win_t widget: the rich widget.
@INPUT int page: the 1-based page index.
@RETURN void: none.
*/
XDC_API void richctrl_move_to_page(res_win_t widget, int page);

/*
@FUNCTION richctrl_move_first_page: move to the first page.
@INPUT res_win_t widget: the rich widget.
@RETURN void: none.
*/
XDC_API void richctrl_move_first_page(res_win_t widget);

/*
@FUNCTION richctrl_move_last_page: move to the last page.
@INPUT res_win_t widget: the rich widget.
@RETURN void: none.
*/
XDC_API void richctrl_move_last_page(res_win_t widget);

/*
@FUNCTION richctrl_move_next_page: move to the next page.
@INPUT res_win_t widget: the rich widget.
@RETURN void: none.
*/
XDC_API void richctrl_move_next_page(res_win_t widget);

/*
@FUNCTION richctrl_move_prev_page: move to the previous page.
@INPUT res_win_t widget: the rich widget.
@RETURN void: none.
*/
XDC_API void richctrl_move_prev_page(res_win_t widget);

/*
@FUNCTION richctrl_get_cur_page: get current page.
@INPUT res_win_t widget: the rich widget.
@RETURN int: return the 1-based page index.
*/
XDC_API int richctrl_get_cur_page(res_win_t widget);

/*
@FUNCTION richctrl_get_max_page: get maximized page.
@INPUT res_win_t widget: the rich widget.
@RETURN int: return the total pages.
*/
XDC_API int richctrl_get_max_page(res_win_t widget);

/*
@FUNCTION richctrl_get_dirty: get rich is dirty.
@INPUT res_win_t widget: the rich widget.
@RETURN bool_t: return nonzero for beging dirty.
*/
XDC_API bool_t richctrl_get_dirty(res_win_t widget);

/*
@FUNCTION richctrl_set_dirty: set rich is dirty in design mode.
@INPUT res_win_t widget: the rich widget.
@INPUT bool_t b_dirty: nonzero for setting dirty, zero for cleaning.
@RETURN void: none.
*/
XDC_API void richctrl_set_dirty(res_win_t widget, bool_t bDirty);

/*
@FUNCTION richctrl_delete_anch: delete the ahchor.
@INPUT res_win_t widget: the rich widget.
@INPUT link_t_ptr ilk: the anchor link component.
@RETURN void: none.
*/
XDC_API void richctrl_delete_anch(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION richctrl_insert_anch: add a new ahchor into rich widget.
@INPUT res_win_t widget: the rich widget.
@INPUT link_t_ptr pos: the anchor link component or link indicator: LINK_FIRST, LINK_LAST.
@RETURN link_t_ptr: if succeeds return the new anchor link component, fails return NULL.
*/
XDC_API link_t_ptr richctrl_insert_anch(res_win_t widget, link_t_ptr pos);

/*
@FUNCTION richctrl_set_anch_text: set the ahchor text.
@INPUT res_win_t widget: the rich widget.
@INPUT link_t_ptr ilk: the anchor link component.
@INPUT const tchar_t* token: the text token.
@RETURN void: none.
*/
XDC_API void richctrl_set_anch_text(res_win_t widget, link_t_ptr nlk, const tchar_t* token);

/*
@FUNCTION richctrl_select_cur: select currnet ahchor in widget.
@INPUT res_win_t widget: the rich widget.
@RETURN void: none.
*/
XDC_API void richctrl_select_cur(res_win_t widget);

/*
@FUNCTION richctrl_select_all: select all ahchor in widget.
@INPUT res_win_t widget: the rich widget.
@RETURN void: none.
*/
XDC_API void richctrl_select_all(res_win_t widget);

/*
@FUNCTION richctrl_get_selected_text: copy the selected ahchor text.
@INPUT res_win_t widget: the rich widget.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return characters copied.
*/
XDC_API int richctrl_get_selected_text(res_win_t widget, tchar_t* buf, int max);

/**************************************memo control**************************************************/
typedef struct _NOTICE_MEMO{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr memo;
	link_t_ptr line;
	void* data;

	int ret;
}NOTICE_MEMO;

typedef enum{
	NC_MEMOLBCLK,
	NC_MEMORBCLK,
	NC_MEMODBCLK,

	NC_MEMOLINEUPDATE,
	NC_MEMOLINECHANGED,

	NC_MEMOCALCED,
	NC_MEMOLINECALCED
}MEMO_NOTICE_CODE;

/*
@FUNCTION memoctrl_create: create a memo widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t memoctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION memoctrl_attach: attach a memo document to widget.
@INPUT res_win_t widget: the memo widget.
@INPUT link_t_ptr ptr: the memo document.
@RETURN void: none.
*/
XDC_API void	memoctrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION memoctrl_detach: detach the memo document from widget.
@INPUT res_win_t widget: the memo widget.
@RETURN link_t_ptr: the memo link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr memoctrl_detach(res_win_t widget);

/*
@FUNCTION memoctrl_fetch: get the memo document from widget.
@INPUT res_win_t widget: the memo widget.
@RETURN link_t_ptr: the memo link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr memoctrl_fetch(res_win_t widget);

/*
@FUNCTION memoctrl_redraw: redraw memo widget.
@INPUT res_win_t widget: the memo widget.
@RETURN void: none.
*/
XDC_API void	memoctrl_redraw(res_win_t widget);

/*
@FUNCTION memoctrl_get_focus_line: set focus to the line.
@INPUT res_win_t widget: the memo widget.
@INPUT link_t_ptr ilk: the line link component.
@RETURN bool_t: return nonzero for being the line focused, otherwise return zero.
*/
XDC_API void memoctrl_set_focus_line(res_win_t widget, link_t_ptr nlk);

/*
@FUNCTION memoctrl_get_focus_line: get focus line in memo widget.
@INPUT res_win_t widget: the memo widget.
@RETURN link_t_ptr: return the focused line link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr memoctrl_get_focus_line(res_win_t widget);

/*
@FUNCTION memoctrl_get_line_rect: get line rect in memo widget client.
@INPUT res_win_t widget: the memo widget.
@INPUT link_t_ptr ilk: the line link component.
@OUTPUT xrect_t* pxr: the rect struct using long member.
@RETURN void: none.
*/
XDC_API void memoctrl_get_line_rect(res_win_t widget, link_t_ptr nlk, xrect_t* pxr);

/*
@FUNCTION memoctrl_get_lock: get the memo widget is locked.
@INPUT res_win_t widget: the memo widget.
@RETURN bool_t: return nonzero for being memo locked, otherwise return zero.
*/
XDC_API bool_t memoctrl_get_lock(res_win_t widget);

/*
@FUNCTION memoctrl_set_lock: set the memo widget is locked.
@INPUT res_win_t widget: the memo widget.
@INPUT bool_t b_lock: nonzero for locking the memo, zero for unlocking.
@RETURN void: none.
*/
XDC_API void memoctrl_set_lock(res_win_t widget, bool_t bLock);

/*
@FUNCTION memoctrl_move_to_page: move to the page.
@INPUT res_win_t widget: the memo widget.
@INPUT int page: the 1-based page index.
@RETURN void: none.
*/
XDC_API void memoctrl_move_to_page(res_win_t widget, int page);

/*
@FUNCTION memoctrl_move_first_page: move to the first page.
@INPUT res_win_t widget: the memo widget.
@RETURN void: none.
*/
XDC_API void memoctrl_move_first_page(res_win_t widget);

/*
@FUNCTION memoctrl_move_last_page: move to the last page.
@INPUT res_win_t widget: the memo widget.
@RETURN void: none.
*/
XDC_API void memoctrl_move_last_page(res_win_t widget);

/*
@FUNCTION memoctrl_move_next_page: move to the next page.
@INPUT res_win_t widget: the memo widget.
@RETURN void: none.
*/
XDC_API void memoctrl_move_next_page(res_win_t widget);

/*
@FUNCTION memoctrl_move_prev_page: move to the previous page.
@INPUT res_win_t widget: the memo widget.
@RETURN void: none.
*/
XDC_API void memoctrl_move_prev_page(res_win_t widget);

/*
@FUNCTION memoctrl_get_cur_page: get current page.
@INPUT res_win_t widget: the memo widget.
@RETURN int: return the 1-based page index.
*/
XDC_API int memoctrl_get_cur_page(res_win_t widget);

/*
@FUNCTION memoctrl_get_max_page: get maximized page.
@INPUT res_win_t widget: the memo widget.
@RETURN int: return the tital pages.
*/
XDC_API int memoctrl_get_max_page(res_win_t widget);

/*
@FUNCTION memoctrl_get_dirty: get memo is dirty.
@INPUT res_win_t widget: the memo widget.
@RETURN bool_t: return nonzero for beging dirty.
*/
XDC_API bool_t memoctrl_get_dirty(res_win_t widget);

/*
@FUNCTION memoctrl_set_dirty: set memo is dirty in design mode.
@INPUT res_win_t widget: the memo widget.
@INPUT bool_t b_dirty: nonzero for setting dirty, zero for cleaning.
@RETURN void: none.
*/
XDC_API void memoctrl_set_dirty(res_win_t widget, bool_t bDirty);

/*
@FUNCTION memoctrl_delete_line: delete the line in widget.
@INPUT res_win_t widget: the memo widget.
@INPUT link_t_ptr ilk: the item link component.
@RETURN void: none.
*/
XDC_API void memoctrl_delete_line(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION memoctrl_set_line_text: set the line text.
@INPUT res_win_t widget: the memo widget.
@INPUT link_t_ptr ilk: the item link component.
@INPUT const tchar_t* token: the text token.
@INPUT int len: the length of text in characters.
@RETURN void: none.
*/
XDC_API void memoctrl_set_line_text(res_win_t widget, link_t_ptr nlk, const tchar_t* token, int len);

/*
@FUNCTION memoctrl_selec_cur: select currnet line in widget.
@INPUT res_win_t widget: the memo widget.
@RETURN void: none.
*/
XDC_API void memoctrl_selec_cur(res_win_t widget);

/*
@FUNCTION memoctrl_select_all: select all line in widget.
@INPUT res_win_t widget: the memo widget.
@RETURN void: none.
*/
XDC_API void memoctrl_select_all(res_win_t widget);

/*
@FUNCTION memoctrl_get_selected_text: copy the selected line text.
@INPUT res_win_t widget: the memo widget.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return characters copied.
*/
XDC_API int memoctrl_get_selected_text(res_win_t widget, tchar_t* buf, int max);

/*
@FUNCTION memoctrl_set_text: set the memo text.
@INPUT res_win_t widget: the memo widget.
@INPUT const tchar_t* token: the text token.
@INPUT int len: the length of text in characters.
@RETURN void: none.
*/
XDC_API void memoctrl_set_text(res_win_t widget, const tchar_t* text, int len);

/*
@FUNCTION memoctrl_get_text: copy the whole text in memo control.
@INPUT res_win_t widget: the memo widget.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return characters copied.
*/
XDC_API int memoctrl_get_text(res_win_t widget, tchar_t* buf, int max);

/**************************************tag control**************************************************/
typedef struct _NOTICE_TAG{
	res_win_t widget;
	unsigned long id;
	unsigned long code;

	link_t_ptr tag;
	link_t_ptr joint;
	void* data;

	int ret;
}NOTICE_TAG;

typedef enum{
	NC_TAGLBCLK,
	NC_TAGRBCLK,
	NC_TAGDBCLK,

	NC_TAGJOINTEDITING,
	NC_TAGJOINTUPDATE,
	NC_TAGJOINTCHANGED,
	NC_TAGJOINTCOMMIT,
	NC_TAGJOINTROLLBACK,

	NC_TAGCALCED,
	NC_TAGJOINTCALCED
}TAG_NOTICE_CODE;

/*
@FUNCTION tagctrl_create: create a tag widget.
@INPUT const tchar_t* wname: the widget title.
@INPUT dword_t style: the widget style.
@INPUT const xrect_t* pxr: the widget rect.
@INPUT res_win_t owner: the owner widget.
@RETURN res_win_t: return the new widget resource handle.
*/
XDC_API res_win_t tagctrl_create(const tchar_t* wname, dword_t wstyle, const xrect_t* pxr, res_win_t wparent);

/*
@FUNCTION tagctrl_attach: attach a tag document to widget.
@INPUT res_win_t widget: the tag widget.
@INPUT link_t_ptr ptr: the tag document.
@RETURN void: none.
*/
XDC_API void	tagctrl_attach(res_win_t widget, link_t_ptr ptr);

/*
@FUNCTION tagctrl_detach: detach the tag document from widget.
@INPUT res_win_t widget: the tag widget.
@RETURN link_t_ptr: the tag link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr tagctrl_detach(res_win_t widget);

/*
@FUNCTION tagctrl_fetch: get the tag document from widget.
@INPUT res_win_t widget: the tag widget.
@RETURN link_t_ptr: the tag link component if exist, otherwise return NULL.
*/
XDC_API link_t_ptr tagctrl_fetch(res_win_t widget);

/*
@FUNCTION tagctrl_redraw: redraw tag widget.
@INPUT res_win_t widget: the tag widget.
@RETURN void: none.
*/
XDC_API void	tagctrl_redraw(res_win_t widget);

/*
@FUNCTION tagctrl_set_focus_joint: set focus to the joint.
@INPUT res_win_t widget: the tag widget.
@INPUT link_t_ptr ilk: the joint link component.
@RETURN bool_t: return nonzero for being the joint focused, otherwise return zero.
*/
XDC_API void tagctrl_set_focus_joint(res_win_t widget, link_t_ptr ilk);

/*
@FUNCTION tagctrl_get_focus_joint: get focus joint in tag widget.
@INPUT res_win_t widget: the tag widget.
@RETURN link_t_ptr: return the focused joint link component if exists, otherwise return NULL.
*/
XDC_API link_t_ptr tagctrl_get_focus_joint(res_win_t widget);

/*
@FUNCTION tagctrl_get_joint_rect: get joint rect in tag widget client.
@INPUT res_win_t widget: the tag widget.
@INPUT link_t_ptr ilk: the joint link component.
@OUTPUT xrect_t* pxr: the rect struct using long member.
@RETURN void: none.
*/
XDC_API void tagctrl_get_joint_rect(res_win_t widget, link_t_ptr nlk, xrect_t* pxr);

/*
@FUNCTION tagctrl_get_lock: get the tag widget is locked.
@INPUT res_win_t widget: the tag widget.
@RETURN bool_t: return nonzero for being tag locked, otherwise return zero.
*/
XDC_API bool_t tagctrl_get_lock(res_win_t widget);

/*
@FUNCTION tagctrl_set_lock: set the tag widget is locked.
@INPUT res_win_t widget: the tag widget.
@INPUT bool_t b_lock: nonzero for locking the tag, zero for unlocking.
@RETURN void: none.
*/
XDC_API void tagctrl_set_lock(res_win_t widget, bool_t bLock);

/*
@FUNCTION tagctrl_move_to_page: move to the page.
@INPUT res_win_t widget: the tag widget.
@INPUT int page: the 1-based page index.
@RETURN void: none.
*/
XDC_API void tagctrl_move_to_page(res_win_t widget, int page);

/*
@FUNCTION tagctrl_move_first_page: move to the first page.
@INPUT res_win_t widget: the tag widget.
@RETURN void: none.
*/
XDC_API void tagctrl_move_first_page(res_win_t widget);

/*
@FUNCTION tagctrl_move_last_page: move to the last page.
@INPUT res_win_t widget: the tag widget.
@RETURN void: none.
*/
XDC_API void tagctrl_move_last_page(res_win_t widget);

/*
@FUNCTION tagctrl_move_next_page: move to the next page.
@INPUT res_win_t widget: the tag widget.
@RETURN void: none.
*/
XDC_API void tagctrl_move_next_page(res_win_t widget);

/*
@FUNCTION tagctrl_move_prev_page: move to the previous page.
@INPUT res_win_t widget: the tag widget.
@RETURN void: none.
*/
XDC_API void tagctrl_move_prev_page(res_win_t widget);

/*
@FUNCTION tagctrl_get_cur_page: get current page.
@INPUT res_win_t widget: the tag widget.
@RETURN int: return the 1-based page index.
*/
XDC_API int tagctrl_get_cur_page(res_win_t widget);

/*
@FUNCTION tagctrl_get_max_page: get maximized page.
@INPUT res_win_t widget: the tag widget.
@RETURN int: return the total pages.
*/
XDC_API int tagctrl_get_max_page(res_win_t widget);

/*
@FUNCTION tagctrl_get_dirty: get tag is dirty.
@INPUT res_win_t widget: the tag widget.
@RETURN bool_t: return nonzero for beging dirty.
*/
XDC_API bool_t tagctrl_get_dirty(res_win_t widget);

/*
@FUNCTION tagctrl_set_dirty: set tag is dirty in design mode.
@INPUT res_win_t widget: the tag widget.
@INPUT bool_t b_dirty: nonzero for setting dirty, zero for cleaning.
@RETURN void: none.
*/
XDC_API void tagctrl_set_dirty(res_win_t widget, bool_t bDirty);

/*
@FUNCTION tagctrl_delete_joint: delete the joint in widget.
@INPUT res_win_t widget: the tag widget.
@INPUT link_t_ptr ilk: the joint link component.
@RETURN void: none.
*/
XDC_API void tagctrl_delete_joint(res_win_t widget, link_t_ptr nlk);

/*
@FUNCTION tagctrl_set_joint_text: set the joint text.
@INPUT res_win_t widget: the tag widget.
@INPUT link_t_ptr ilk: the joint link component.
@INPUT const tchar_t* token: the text token.
@INPUT int len: the token length in characters.
@RETURN void: none.
*/
XDC_API void tagctrl_set_joint_text(res_win_t widget, link_t_ptr nlk, const tchar_t* token, int len);

/*
@FUNCTION tagctrl_select_all: select all joint in widget.
@INPUT res_win_t widget: the tag widget.
@RETURN void: none.
*/
XDC_API void tagctrl_select_all(res_win_t widget);

/*
@FUNCTION tagctrl_select_cur: select currnet joint in widget.
@INPUT res_win_t widget: the tag widget.
@RETURN void: none
*/
XDC_API void tagctrl_select_cur(res_win_t widget);

/*
@FUNCTION tagctrl_get_selected_text: copy the selected joint text.
@INPUT res_win_t widget: the tag widget.
@OUTPUT tchar_t* buf: the string buffer.
@INPUT int max: the string buffer size in characters.
@RETURN int: return characters copied.
*/
XDC_API int tagctrl_get_selected_text(res_win_t widget, tchar_t* buf, int max);

/*
@FUNCTION tagctrl_setup_tag: set the current selected text as a tag.
@INPUT res_win_t widget: the tag widget.
@INPUT const tchar_t* tname: the tag name token.
@RETURN void: none.
*/
XDC_API void tagctrl_setup_tag(res_win_t widget, const tchar_t* tname);

/*
@FUNCTION tagctrl_setup_tag: clear the current joint tag.
@INPUT res_win_t widget: the tag widget.
@RETURN void: none.
*/
XDC_API void tagctrl_clean_tag(res_win_t widget);


#ifdef	__cplusplus
}
#endif

#endif /*XDCCTRL_H*/
