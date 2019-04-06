﻿/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc memo document

	@module	memodoc.c | memo document implement file

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

#include "memodoc.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdldoc.h"

#ifdef XDL_SUPPORT_DOC

link_t_ptr create_memo_doc()
{
	link_t_ptr ptr;

	ptr = create_dom_doc();
	set_dom_node_name(ptr, DOC_MEMO, -1);

	set_memo_width(ptr, PAPER_A4_WIDTH);
	set_memo_height(ptr, PAPER_A4_HEIGHT);

	return ptr;
}

void destroy_memo_doc(link_t_ptr ptr)
{
	destroy_dom_doc(ptr);
}

link_t_ptr get_memo_lineset(link_t_ptr ptr)
{
	return ptr;
}

bool_t is_memo_doc(link_t_ptr ptr)
{
	return (compare_text(get_dom_node_name_ptr(ptr), -1, DOC_MEMO, -1, 0) == 0) ? 1 : 0;
}

bool_t is_memo_line(link_t_ptr ptr, link_t_ptr ilk)
{
	return is_dom_child_node(ptr, ilk);
}

void clear_memo_doc(link_t_ptr ptr)
{
	ptr = get_memo_lineset(ptr);

	delete_dom_child_nodes(ptr);
}

int get_memo_line_count(link_t_ptr ptr)
{
	ptr = get_memo_lineset(ptr);

	return get_dom_child_node_count(ptr);
}

link_t_ptr insert_memo_line(link_t_ptr ptr, link_t_ptr pos)
{
	link_t_ptr ilk;

	ptr = get_memo_lineset(ptr);

	ilk = insert_dom_node(ptr, pos);

	set_dom_node_name(ilk, DOC_MEMO_LINE, -1);

	return ilk;
}

void delete_memo_line(link_t_ptr elk)
{
	delete_dom_node(elk);
}

link_t_ptr merge_memo_line(link_t_ptr nlk)
{
	link_t_ptr pre;
	int n;
	tchar_t* buf;

	pre = get_prev_link(nlk);
	if (!pre)
		return NULL;

	n = get_memo_line_indent(nlk);
	while (n--)
		memo_line_text_ins_chars(pre, -1, _T("\t"), 1);

	n = xslen(get_memo_line_text_ptr(pre)) + xslen(get_memo_line_text_ptr(nlk));

	if (n)
	{
		buf = detach_dom_node_text(pre);
		buf = xsrealloc(buf, n + 1);
		xscat(buf, get_memo_line_text_ptr(nlk));

		attach_dom_node_text(pre, buf);
	}

	delete_memo_line(nlk);

	return pre;
}

link_t_ptr split_memo_line(link_t_ptr nlk, int pos)
{
	link_t_ptr plk;
	tchar_t* buf;
	int n,len;

	len = xslen(get_dom_node_text_ptr(nlk));

	if (pos > len)
		return NULL;

	buf = detach_dom_node_text(nlk);
	
	set_dom_node_text(nlk, buf, pos);

	n = 0;
	len -= pos;
	plk = insert_memo_line(get_dom_parent_node(nlk), nlk);

	if (!len)
		return plk;

	while (len)
	{
		if (buf[pos] == _T('\t'))
		{
			pos++;
			n++;
		}
		len--;
	}

	while (buf[pos] != _T('\0'))
	{
		buf[len++] = buf[pos++];
	} 
	buf[len] = _T('\0');

	set_memo_line_indent(plk, n);
	attach_dom_node_text(plk, buf);

	return plk;
}

link_t_ptr get_memo_next_line(link_t_ptr ptr,link_t_ptr pos)
{
	ptr = get_memo_lineset(ptr);

	if (pos == LINK_FIRST)
		return get_dom_first_child_node(ptr);
	else if (pos == LINK_LAST)
		return NULL;
	else
		return get_dom_next_sibling_node(pos);
}

link_t_ptr get_memo_prev_line(link_t_ptr ptr,link_t_ptr pos)
{
	ptr = get_memo_lineset(ptr);

	if (pos == LINK_FIRST)
		return NULL;
	else if (pos == LINK_LAST)
		return get_dom_last_child_node(ptr);
	else
		return get_dom_prev_sibling_node(pos);
}

link_t_ptr get_memo_line_at(link_t_ptr ptr, int index)
{
	link_t_ptr ilk;

	if (index < 0)
		return NULL;

	ilk = get_memo_next_line(ptr, LINK_FIRST);
	while (ilk && index--)
		ilk = get_memo_next_line(ptr, ilk);

	return ilk;
}

int get_memo_line_index(link_t_ptr ptr, link_t_ptr ilk)
{
	link_t_ptr plk;
	int index = -1;

	plk = get_memo_next_line(ptr, LINK_FIRST);
	while (plk)
	{
		index++;
		if (plk == ilk)
			break;
		plk = get_memo_next_line(ptr, ilk);
	}

	return index;
}

int format_memo_doc(link_t_ptr ptr, tchar_t* buf, int max)
{
	link_t_ptr ilk;
	int ind, len, total = 0;

	ilk = get_memo_next_line(ptr, LINK_FIRST);
	while (ilk)
	{
		len = get_memo_line_indent(ilk);
		if (len + total > max)
			return total;

		if (buf)
		{
			ind = 0;
			while (ind < len)
			{
				buf[total + ind] = _T('\t');
				ind++;
			}
		}
		total += len;

		len = get_memo_line_text(ilk, NULL, MAX_LONG);
		if (len + total > max)
			return total;

		if (buf)
		{
			get_memo_line_text(ilk, buf + total, len);
		}
		total += len;

		len = 1;
		if (len + total > max)
			return total;

		if (buf)
		{
			buf[total] = _T('\n');
		}
		total += len;

		ilk = get_memo_next_line(ptr, ilk);
	}

	return total;
}

bool_t parse_memo_doc(link_t_ptr ptr, const tchar_t* buf, int len)
{
	link_t_ptr ilk;
	const tchar_t *tk_inde, *tk_line;
	int len_inde, len_line, total = 0;

	if (len < 0)
		len = xslen(buf);

	clear_memo_doc(ptr);

	while (total < len)
	{
		tk_inde = buf + total;
		len_inde = 0;
		while (*(tk_inde + len_inde) == _T('\t'))
		{
			len_inde++;
		}
		total += len_inde;

		tk_line = buf + total;
		len_line = 0;
		while (*(tk_line + len_line) != _T('\r') && *(tk_line + len_line) != _T('\n') && *(tk_line + len_line) != _T('\0') && total < len)
		{
			len_line++;
			total++;
		}

		ilk = insert_memo_line(ptr, LINK_LAST);
		if (len_inde)
		{
			set_memo_line_indent(ilk, len_inde);
		}
		set_memo_line_text(ilk, tk_line, len_line);

		while (*(tk_line + len_line) == _T('\r') || *(tk_line + len_line) == _T('\n'))
		{
			len_line++;
			total++;
		}

		if (*(buf + total) == _T('\0'))
			break;
	}

	return 1;
}


#endif //XDLDOC
