﻿/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc image image_list view document

	@module	imageview.c | image_list document implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
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
#include "imagesview.h"
#include "xdldoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"

#ifdef XDL_SUPPORT_VIEW


int calc_images_items_per_row(link_t_ptr ptr, float pw)
{
	int hs;

	hs = (int)(pw / get_images_item_width(ptr));
	if (hs == 0)
		hs = 1;

	return hs;
}

int calc_images_items_per_col(link_t_ptr ptr, float ph)
{
	int hs;

	hs = (int)(ph / get_images_item_height(ptr));
	if (hs == 0)
		hs = 1;

	return hs;
}

int calc_images_cols_per_page(link_t_ptr ptr, float pw)
{
	float iw;

	iw = get_images_item_width(ptr);

	return (int)(pw / iw);
}

int calc_images_rows_per_page(link_t_ptr ptr, float ph)
{
	float ih;

	ih = get_images_item_height(ptr);

	return (int)(ph / ih);
}

float calc_images_height(const canvbox_t* pbox, link_t_ptr ptr)
{
	float ih;
	int lines, re;
	int count, per;

	ih = get_images_item_height(ptr);

	count = get_images_item_count(ptr);

	per = calc_images_items_per_row(ptr, pbox->fw);

	re = count % per;
	lines = count / per;
	if (re)
		lines++;

	return ih * lines;
}

float calc_images_width(const canvbox_t* pbox, link_t_ptr ptr)
{
	float iw;
	int lines, re;
	int count, per;

	iw = get_images_item_width(ptr);

	count = get_images_item_count(ptr);

	per = calc_images_items_per_col(ptr, pbox->fh);

	re = count % per;
	lines = count / per;
	if (re)
		lines++;

	return iw * lines;
}

bool_t calc_images_item_rect(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr)
{
	link_t_ptr nlk;
	int pi, count;
	float iw, ih;
	const tchar_t* sz_lay;

	xmem_zero((void*)pxr, sizeof(xrect_t));

	sz_lay = get_images_layer_ptr(ptr);

	iw = get_images_item_width(ptr);
	ih = get_images_item_height(ptr);

	if (compare_text(sz_lay, -1, ATTR_LAYER_HORZ, -1, 0) == 0)
	{
		pi = calc_images_items_per_col(ptr, pbox->fh);
	}
	else
	{
		pi = calc_images_items_per_row(ptr, pbox->fw);
	}

	count = 0;
	nlk = get_images_next_item(ptr, LINK_FIRST);
	while (nlk)
	{
		if (compare_text(sz_lay, -1, ATTR_LAYER_HORZ, -1, 0) == 0)
		{
			pxr->fx = iw * (count / pi);
			pxr->fy = ih * (count % pi);
		}
		else
		{
			pxr->fx = iw * (count % pi);
			pxr->fy = ih * (count / pi);
		}

		if (nlk == ilk)
		{
			pxr->fw = iw;
			pxr->fh = ih;
			return 1;
		}

		count++;
		nlk = get_images_next_item(ptr, nlk);
	}

	return 0;
}

bool_t calc_images_image_rect(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr)
{
	float ic;

	if (!calc_images_item_rect(pbox, ptr, ilk, pxr))
		return 0;

	ic = get_images_icon_span(ptr);
	pxr->fh -= ic;

	return 1;
}

bool_t calc_images_text_rect(const canvbox_t* pbox, link_t_ptr ptr, link_t_ptr ilk, xrect_t* pxr)
{
	float ic;

	if (!calc_images_item_rect(pbox, ptr, ilk, pxr))
		return 0;

	ic = get_images_icon_span(ptr);
	pxr->fy = pxr->fy + pxr->fh - ic;
	pxr->fh = ic;

	return 1;
}

int	calc_images_hint(const canvbox_t* pbox, const xpoint_t* ppt, link_t_ptr ptr,  link_t_ptr* pilk)
{
	link_t_ptr nlk;
	int pi, count;
	float lw, lh, ic;
	float x1, y1, x2, y2;
	const tchar_t* sz_lay;

	if (pilk)
		*pilk = NULL;

	sz_lay = get_images_layer_ptr(ptr);

	lw = get_images_item_width(ptr);
	lh = get_images_item_height(ptr);
	ic = get_images_icon_span(ptr);

	if (compare_text(sz_lay, -1, ATTR_LAYER_HORZ, -1, 0) == 0)
		pi = calc_images_items_per_col(ptr, pbox->fh);
	else
		pi = calc_images_items_per_row(ptr, pbox->fw);

	x1 = 0;
	y1 = 0;
	x2 = x1 + lw;
	y2 = y1 + lh;

	count = 0;
	nlk = get_images_next_item(ptr, LINK_FIRST);
	while (nlk)
	{
		if (compare_text(sz_lay, -1, ATTR_LAYER_HORZ, -1, 0) == 0)
		{
			x1 = lw * (count / pi);
			y1 = lh * (count % pi);
			x2 = x1 + lw;
			y2 = y1 + lh;
		}
		else
		{
			x1 = lw * (count % pi);
			y1 = lh * (count / pi);
			x2 = x1 + lw;
			y2 = y1 + lh;
		}

		if (ft_inside(ppt->fx, ppt->fy, x1, y1, x2, y2))
		{
			if (pilk)
				*pilk = nlk;

			if (ft_inside(ppt->fx, ppt->fy, x1, y1, x1 + ic, y1 + ic))
				return IMAGE_HINT_CHECK;

			if (ft_inside(ppt->fx, ppt->fy, x1, y2 - ic, x2, y2))
				return IMAGE_HINT_TEXT;

			return IMAGE_HINT_ITEM;
		}

		count++;
		nlk = get_images_next_item(ptr, nlk);
	}

	return IMAGE_HINT_NONE;
}

void draw_images(const if_canvas_t* pif, const canvbox_t* pbox, link_t_ptr ptr)
{
	link_t_ptr nlk;
	int pi, count;
	float iw, ih, ic;
	xrect_t xr, xr_box;
	xfont_t xf = { 0 };
	xface_t xa = { 0 };
	ximage_t xi = { 0 };
	xcolor_t xc = { 0 };
	const tchar_t* layer;
	bool_t b_print;
	float px, py, pw, ph;

	XDL_ASSERT( pif != NULL);

	px = pbox->fx;
	py = pbox->fy;
	pw = pbox->fw;
	ph = pbox->fh;

	b_print = ((*pif->pf_canvas_type)(pif->canvas) == _CANV_PRINTER) ? 1 : 0;

	default_xfont(&xf);
	default_xface(&xa);
	xscpy(xa.text_align, GDI_ATTR_TEXT_ALIGN_CENTER);
	xscpy(xa.line_align, GDI_ATTR_TEXT_ALIGN_CENTER);

	if (!b_print)
	{
		format_xcolor(&pif->clr_txt, xf.color);
	}

	parse_xcolor(&xc, xf.color);

	iw = get_images_item_width(ptr);
	ih = get_images_item_height(ptr);
	ic = get_images_icon_span(ptr);

	layer = get_images_layer_ptr(ptr);

	if (compare_text(layer, -1, ATTR_LAYER_HORZ, -1, 0) == 0)
		pi = calc_images_items_per_col(ptr, ph);
	else
		pi = calc_images_items_per_row(ptr, pw);

	if (!b_print)
	{
		format_xcolor(&pif->clr_msk, xi.color);
	}

	xr.fx = px;
	xr.fy = py;
	xr.fw = iw;
	xr.fh = ih;

	count = 0;
	nlk = get_images_next_item(ptr, LINK_FIRST);
	while (nlk)
	{
		if (compare_text(layer, -1, ATTR_LAYER_HORZ, -1, 0) == 0)
		{
			xr.fx = iw * (count / pi) + px;
			xr.fy = ih * (count % pi) + py;
			xr.fw = iw;
			xr.fh = ih;
		}
		else
		{
			xr.fx = iw * (count % pi) + px;
			xr.fy = ih * (count / pi) + py;
			xr.fw = iw;
			xr.fh = ih;
		}

		xr_box.fx = xr.fx;
		xr_box.fy = xr.fy;
		xr_box.fw = xr.fw;
		xr_box.fh = xr.fh - ic;

		parse_ximage_from_source(&xi, get_images_item_src_ptr(nlk));
		(*pif->pf_draw_image)(pif->canvas, &xi, &xr_box);

		xr_box.fx = xr.fx;
		xr_box.fy = xr.fy + xr.fh - ic;
		xr_box.fw = xr.fw;
		xr_box.fh = ic;

		(*pif->pf_draw_text)(pif->canvas, &xf, &xa, &xr_box, get_images_item_alt_ptr(nlk), -1);
			
		if (get_images_item_checked(nlk))
		{
			xr_box.fx = xr.fx;
			xr_box.fy = xr.fy;
			xr_box.fw = ic;
			xr_box.fh = ic;

			ft_center_rect(&xr_box, DEF_SMALL_ICON, DEF_SMALL_ICON);
			(*pif->pf_draw_icon)(pif->canvas, &xc, &xr_box, ATTR_ICON_CHECKED);
		}

		count++;
		nlk = get_images_next_item(ptr, nlk);
	}
}

#endif //XDLVIEW