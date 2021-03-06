﻿/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc file document

	@module	file.c | xdl file implement file

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

#include "xdlfile.h"
#include "xdlimp.h"
#include "xdloem.h"
#include "xdlstd.h"
#include "xdlnet.h"
#include "xdldoc.h"


xhand_t	xfile_bio(file_t fh)
{
	if_bio_t* pfn = (if_bio_t*)fh;

	XDL_ASSERT(fh);

	return pfn->bio;
}

file_t xfile_open(const secu_desc_t* psd, const tchar_t* file, dword_t mode)
{
	if_bio_t* pfn;
	int proto;

	proto = parse_proto(file);
	if (proto < 0)
		return NULL;

	pfn = (if_bio_t*)xmem_alloc(sizeof(if_bio_t));

	if (IS_INET_FILE(proto))
		pfn->bio = xinet_open_file(psd, file, mode);
	else
		pfn->bio = xuncf_open_file(psd, file, mode);

	if (!pfn->bio)
	{
		xmem_free(pfn);
		return NULL;
	}

	if (IS_INET_FILE(proto))
	{
		pfn->pf_close = xinet_close_file;
		pfn->pf_read = xinet_read_file;
		pfn->pf_read_range = xinet_read_file_range;
		pfn->pf_write = xinet_write_file;
		pfn->pf_write_range = xinet_write_file_range;
		pfn->pf_flush = NULL;
		pfn->pf_set_time = xinet_set_filetime;
		pfn->pf_set_since = xinet_set_filesince;
	}
	else
	{
		pfn->pf_close = xuncf_close_file;
		pfn->pf_read = xuncf_read_file;
		pfn->pf_read_range = xuncf_read_file_range;
		pfn->pf_write = xuncf_write_file;
		pfn->pf_write_range = xuncf_write_file_range;
		pfn->pf_flush = xuncf_flush_file;
		pfn->pf_set_time = xuncf_set_filetime;
		pfn->pf_set_since = NULL;
	}

	return (file_t)pfn;
}

bool_t	xfile_read(file_t fh, byte_t* buf, dword_t size)
{
	if_bio_t* pfn = (if_bio_t*)fh;
	dword_t nbys, npos = 0;
	bool_t rt = 1;

	XDL_ASSERT(fh && pfn->pf_read);
	
	while (npos < size)
	{
		nbys = size - npos;

		rt = (*pfn->pf_read)(pfn->bio, (void*)(buf + npos), &nbys);

		if (!rt || !nbys)
			break;

		npos += nbys;
	}

	return rt;
}

bool_t xfile_write(file_t fh, const byte_t* buf, dword_t size)
{
	if_bio_t* pfn = (if_bio_t*)fh;
	dword_t nbys, npos = 0;
	bool_t rt = 1;

	XDL_ASSERT(fh && pfn->pf_write);

	while (npos < size)
	{
		nbys = size - npos;

		rt = (*pfn->pf_write)(pfn->bio, (void*)(buf + npos), &nbys);

		if (!rt || !nbys)
			break;

		npos += nbys;
	}

	return rt;
}

bool_t xfile_read_range(file_t fh, dword_t hoff, dword_t loff, byte_t* buf, dword_t size)
{
	if_bio_t* pfn = (if_bio_t*)fh;

	XDL_ASSERT(fh && pfn->pf_read_range);

	return (*pfn->pf_read_range)(pfn->bio, hoff, loff, buf, size);
}

bool_t	xfile_write_range(file_t fh, dword_t hoff, dword_t loff, const byte_t* buf, dword_t size)
{
	if_bio_t* pfn = (if_bio_t*)fh;

	XDL_ASSERT(fh && pfn->pf_write_range);

	return (*pfn->pf_write_range)(pfn->bio, hoff, loff, buf, size);
}

bool_t xfile_flush(file_t fh)
{
	if_bio_t* pfn = (if_bio_t*)fh;

	XDL_ASSERT(fh);

	if (pfn->pf_flush)
	{
		return (*pfn->pf_flush)(pfn->bio);
	}
	else
	{
		return 1;
	}
}

void xfile_set_since(file_t fh, int fs)
{
	if_bio_t* pfn = (if_bio_t*)fh;

	XDL_ASSERT(fh);

	if (pfn->pf_set_since)
	{
		(*pfn->pf_set_since)(pfn->bio, fs);
	}
}

void xfile_settime(file_t fh, const tchar_t* ftime)
{
	if_bio_t* pfn = (if_bio_t*)fh;

	XDL_ASSERT(fh);

	if (pfn->pf_set_time)
	{
		(*pfn->pf_set_time)(pfn->bio, ftime);
	}
}

void xfile_close(file_t fh)
{
	if_bio_t* pfn = (if_bio_t*)fh;

	XDL_ASSERT(fh && pfn->pf_close);

	if (pfn->bio)
	{
		(*pfn->pf_close)(pfn->bio);
	}

	xmem_free(pfn);
}

bool_t xfile_info(const secu_desc_t* psd, const tchar_t* fname, tchar_t* ftime, tchar_t* fsize, tchar_t* fetag, tchar_t* fencode)
{
	int proto;

	if(is_null(fname))
		return 0;

	proto = parse_proto(fname);

	if (IS_INET_FILE(proto))
	{
#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_DOC)
		return xinet_file_info(psd, fname, ftime, fsize, fetag, fencode);
#else
		return 0;
#endif
	}
	if (!IS_INET_FILE(proto))
	{
#if defined(XDK_SUPPORT_FILE)
		return xuncf_file_info(psd, fname, ftime, fsize, fetag, fencode);
#else
		return 0;
#endif
	}

	return 0;
}

bool_t xfile_delete(const secu_desc_t* psd, const tchar_t* fname)
{
	int proto;

	if (is_null(fname))
		return 0;

	proto = parse_proto(fname);

	if (IS_INET_FILE(proto))
	{
#if defined(XDK_SUPPORT_SOCK) && defined(XDL_SUPPORT_DOC)
		return xinet_delete_file(psd, fname);
#else
		return 0;
#endif
	}
	if (!IS_INET_FILE(proto))
	{
#if defined(XDK_SUPPORT_FILE)
		return xuncf_delete_file(psd, fname);
#else
		return 0;
#endif
	}

	return 0;
}

bool_t	xfile_rename(const secu_desc_t* psd, const tchar_t* fname, const tchar_t* nname)
{
	int proto;

	if (is_null(fname))
		return 0;

	proto = parse_proto(fname);

	if (IS_INET_FILE(proto))
	{
		return 0;
	}
	if (!IS_INET_FILE(proto))
	{
#if defined(XDK_SUPPORT_FILE)
		return xuncf_rename_file(psd, fname, nname);
#else
		return 0;
#endif
	}

	return 0;
}

XDL_API bool_t xfile_mkdir(const secu_desc_t* psd, const tchar_t* path)
{
	int proto;

	if (is_null(path))
		return 0;

	proto = parse_proto(path);

	if (IS_INET_FILE(proto))
	{
		return 0;
	}
	if (!IS_INET_FILE(proto))
	{
#if defined(XDK_SUPPORT_FILE)
		return xuncf_create_directory(psd, path);
#endif
	}
	return 0;
}

XDL_API bool_t xfile_rmdir(const secu_desc_t* psd, const tchar_t* path)
{
	int proto;

	if (is_null(path))
		return 0;

	proto = parse_proto(path);

	if (IS_INET_FILE(proto))
		return 0;

#if defined(XDK_SUPPORT_FILE)
	if (!IS_INET_FILE(proto))
		return xuncf_remove_directory(psd, path);
	else
		return 0;
#endif
}

bool_t xfile_copy(const secu_desc_t* psd, const tchar_t* srcfile, const tchar_t* destfile, int fsin)
{
	tchar_t fsize[NUM_LEN + 1] = { 0 };
	tchar_t ftime[DATE_LEN + 1] = { 0 };
	tchar_t fsince[DATE_LEN + 1] = { 0 };
	xdate_t dt_since, dt_time;

	dword_t len;
	bool_t rt;
	bool_t b_range = 0;
	dword_t hoff, loff;
	dword_t max = 2 * XHTTP_ZIPED_SIZE;
	long long l_bytes, l_total = 0;

	byte_t* d_buf = NULL;
	file_t d_src = NULL;
	file_t d_dest = NULL;

	if (!xfile_info(psd, srcfile, ftime, fsize, NULL, NULL))
		return 0;

	if (is_zero_size(fsize))
	{
		set_last_error(_T("-1"), _T("not support zero file copy"), -1);
		return 0;
	}

	if (fsin)
	{
		xfile_info(psd, destfile, fsince, NULL, NULL, NULL);

		parse_gmttime(&dt_since, fsince);
		parse_gmttime(&dt_time, ftime);

		if (compare_datetime(&dt_since, &dt_time) >= 0)
		{
			set_last_error(_T("-1"), _T("file not modified"), -1);
			return 0;
		}
	}

	parse_longlong(&hoff, &loff, fsize);
	b_range = (hoff || loff > max) ? 1 : 0;
	l_total = xstoll(fsize);

	d_src = xfile_open(psd, srcfile, 0);
	if (!d_src)
		return 0;

	d_dest = xfile_open(psd, destfile, FILE_OPEN_CREATE);
	if (!d_dest)
	{
		xfile_close(d_src);
		return 0;
	}
		
	if (b_range)
	{
		hoff = loff = 0;
		while (l_total)
		{
			if ((long long)MAKELONGLONG(max, 0) > l_total)
				len = (dword_t)l_total;
			else
				len = max;

			d_buf = (byte_t*)xmem_alloc(len);
			rt = xfile_read_range(d_src, hoff, loff, d_buf, len);
			if (rt)
			{
				//last set filetime
				if (len == (dword_t)l_total)
				{
					xfile_settime(d_dest, ftime);
				}

				rt = xfile_write_range(d_dest, hoff, loff, d_buf, len);
			}
			xmem_free(d_buf);

			if (!rt)
				break;

			l_bytes = (long long)MAKELONGLONG(loff, hoff) + len;
			hoff = GETHDWORD(l_bytes);
			loff = GETLDWORD(l_bytes);

			l_total -= len;
		}

		xfile_close(d_src);
		d_src = NULL;
	}
	else
	{
		len = xstol(fsize);
		d_buf = (byte_t*)xmem_alloc(len);

		rt = xfile_read(d_src, d_buf, len);

		xfile_close(d_src);
		d_src = NULL;

		if (rt)
		{
			xfile_settime(d_dest, ftime);

			rt = xfile_write(d_dest, d_buf, len);
		}

		xmem_free(d_buf);
		d_buf = NULL;
	}

	rt = xfile_flush(d_dest);

	xfile_close(d_dest);
	d_dest = NULL;

	return rt;
}

static void _list_file(const file_info_t* pfi, void* pa)
{
	link_t_ptr ptr = (link_t_ptr)pa;
	link_t_ptr nlk;

	if (pfi->is_dir)
		nlk = insert_list_directory_item(ptr, LINK_LAST);
	else
		nlk = insert_list_file_item(ptr, LINK_LAST);

	set_list_item_file_info(nlk, pfi);
}

bool_t xfile_list(const secu_desc_t* psd, const tchar_t* path, link_t_ptr ptr)
{
	int proto;

	if (is_null(path))
		return 0;

	proto = parse_proto(path);

	if (IS_INET_FILE(proto))
	{
#if defined(XDK_SUPPORT_SOCK) 
		return xinet_list_file(psd, path, _list_file, (void*)ptr);
#else
		return 0;
#endif
	}
	if (!IS_INET_FILE(proto))
	{
#if defined(XDK_SUPPORT_FILE)
		return xuncf_list_file(psd, path, _list_file, (void*)ptr);
#else
		return 0;
#endif
	}

	return 0;
}

static void _dump_file(const file_info_t* pfi, void* pa)
{
	stream_t stm = (stream_t)pa;

	string_t vs = NULL;
	dword_t pos = 0;
	tchar_t feed[3] = { TXT_ITEMFEED, TXT_LINEFEED, _T('\n') };
	tchar_t ftime[DATE_LEN + 1];
	tchar_t fsize[NUM_LEN + 1];
	tchar_t fetag[KEY_LEN + 1];

	vs = string_alloc();

	if (pfi->is_dir)
		string_cat(vs, DOC_LIST_DIRECTORY, -1);
	else
		string_cat(vs, DOC_LIST_FILE, -1);

	string_cat(vs, feed, 1);

	string_cat(vs, pfi->file_name, -1);
	string_cat(vs, feed, 1);

	format_gmttime(&pfi->create_time, ftime);
	string_cat(vs, ftime, -1);
	string_cat(vs, feed, 1);

	format_gmttime(&pfi->access_time, ftime);
	string_cat(vs, ftime, -1);
	string_cat(vs, feed, 1);

	format_gmttime(&pfi->write_time, ftime);
	string_cat(vs, ftime, -1);
	string_cat(vs, feed, 1);

	if (pfi->is_dir)
		xscpy(fsize, _T(""));
	else
		format_longlong(pfi->high_size, pfi->low_size, fsize);

	string_cat(vs, fsize, -1);
	string_cat(vs, feed, 1);

	if (pfi->is_dir)
		xscpy(fetag, _T(""));
	else
		xscpy(fetag, pfi->file_etag);

	string_cat(vs, fetag, -1);
	string_cat(vs, feed, 1);

	string_cat(vs, feed + 1, 2);

	pos = 0;
	stream_write_line(stm, vs, &pos);

	string_free(vs);
}

bool_t xfile_dump(const secu_desc_t* psd, const tchar_t* path, stream_t stm)
{
	int proto;
	bool_t b = 0;

	if (!is_null(path))
	{
		proto = parse_proto(path);

		if (IS_INET_FILE(proto))
		{
#if defined(XDK_SUPPORT_SOCK)
			b = xinet_list_file(psd, path, _dump_file, (void*)stm);
#endif
		}
		if (!IS_INET_FILE(proto))
		{
#if defined(XDK_SUPPORT_FILE)
			b = xuncf_list_file(psd, path, _dump_file, (void*)stm);
#endif
		}
	}

	stream_write_line(stm, NULL, NULL);

	return stream_flush(stm);
}

void xfile_tree(const secu_desc_t* psd, const tchar_t* path, link_t_ptr ptr)
{
	link_t_ptr nlk;
	tchar_t file[PATH_LEN] = { 0 };
	tchar_t* token;

	xsncpy(file, path, PATH_LEN);
	xscat(file, _T("/*.*"));

	xfile_list(psd, file, ptr);

	nlk = get_list_first_child_item(ptr);
	while (nlk)
	{
		if (is_list_directory_item(nlk))
		{
			token = file + xslen(file);
			while (*token != _T('/') && *token != _T('\\') && token != file)
				*(token--) = _T('\0');

			*token = _T('/');
			xscat(token, get_list_item_file_name_ptr(nlk));

			xfile_tree(psd, file, nlk);
		}

		nlk = get_list_next_sibling_item(nlk);
	}
}
