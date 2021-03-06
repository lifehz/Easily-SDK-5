﻿/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc encoding document

	@module	oemconv.c | encoding implement file

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

#include "oemconv.h"
#include "impmem.h"
#include "imperr.h"
#include "impmbcs.h"

#ifdef GPL_SUPPORT_ACP
#include "../acp/acp.h"

#ifdef _OS_WINDOWS
#pragma comment(lib,"acp.lib")
#endif

#endif

bool_t is_ascii(byte_t b)
{
	return ((b & ~0x7F) == 0) ? 1 : 0;
}

int ascii_byte_to_unicode(char b, wchar_t* c)
{
	if (c)
		*c = (wchar_t)b;

	return 1;
}

int unicode_byte_to_ascii(wchar_t c, char* b)
{
	if (b)
		*b = (char)c;

	return 1;
}

int ascii_to_unicode(const char* src, int len, wchar_t* dst, int max)
{
	int i;

	if (len < 0)
	{
		len = 0;
		while (src && *src)
			len++;
	}

	len = (len < max) ? len : max;
	if (dst)
	{
		for (i = 0; i < len; i++)
			dst[i] = (wchar_t)src[i];
	}

	return len;
}

int unicode_to_ascii(const wchar_t* src, int len, char* dst, int max)
{
	int i;

	if (len < 0)
	{
		len = 0;
		while (src && *src)
			len++;
	}

	len = (len < max) ? len : max;
	if (dst)
	{
		for (i = 0; i < len; i++)
			dst[i] = (char)src[i];
	}

	return len;
}

/*
GB2312 编码
01 - 09区为特殊符号
16 - 55区为一级汉字，按拼音排序。
56 - 87区为二级汉字，按部首 / 笔画排序。
10 - 15区及88 - 94区则未有编码。
*/
int gb2313_index(const byte_t* src)
{
	return (src[0] - 0xA0 - 16) * 94 + (src[1] - 0xA0 - 1);
}

/*
GBK 编码
GBK/1	0xA1A1 - 0xA9FE		846		717
GBK/2	0xB0A1 - 0xF7FE		6768	6763 //兼容gb2312编码
GBK/3	0x8140 - 0xA0FE		6080	6080
GBK/4	0xAA40 - 0xFEA0		8160	8160
GBK/5	0xA840 - 0xA9A0		192		166
EUDC/1	0xAAA1 - 0xAFFE		564		用户定义1
EUDC/2	0xF8A1 - 0xFEFE		658		用户定义2
EUDC/3	0xA140 - 0xA7A0		672		用户定义3
第一字节 0x81-0xFE
第二字节 0x40-0x7E, 0x80-0xFE；每行定义190汉字
*/

int gbk_index(const byte_t* src)
{
	return (src[1] >= 0x80) ? ((src[0] - 0x81) * 190 + (src[1] - 0x80)) : ((src[0] - 0x81) * 190 + (src[1] - 0x40));
}

/*
UNICODE 编码
0x4E00-0x9FA5
*/
int ucs_index(wchar_t wc)
{
	return wc - 0x4E00;
}
/****************************************************************************************************/
#if defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)

dword_t gb2312_sequence(byte_t b)
{
#ifdef XDK_SUPPORT_MBCS
	return (dword_t)gbk_code_sequence(b);
#else
	return (dword_t)gb2312_code_sequence(b);
#endif
}


int gb2312_byte_to_ucs(const byte_t* src, wchar_t* dest)
{
#ifdef XDK_SUPPORT_MBCS
	return gbk_byte_to_ucs(src, dest);
#else
	int n;
	unsigned short us = 0;
	
	n = gb2312_seek_unicode((unsigned char*)src, &us);

	if (dest)
	{
		*dest = (wchar_t)us;
	}

	return n;
#endif
}

int gb2312_to_ucs(const byte_t* src, dword_t slen, wchar_t* dest, int dlen)
{
#ifdef XDK_SUPPORT_MBCS
	return gbk_to_ucs(src, slen, dest, dlen);
#else
	dword_t i = 0;
	int seq, count = 0;

	while (i < slen && count < dlen)
	{
		seq = gb2312_sequence(*(src + i));
		if (!seq)
			break;

		count += gb2312_byte_to_ucs(src + i, ((dest)? dest + count : NULL));
		i += seq;
	}

	return count;
#endif
}

dword_t gb2312_byte_to_utf16lit(const byte_t* src, byte_t* dest)
{
	wchar_t ch;

	gb2312_byte_to_ucs(src, &ch);

	return ucs_byte_to_utf16lit(ch, dest);
}

dword_t gb2312_to_utf16lit(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen)
{
	dword_t i, total = 0;
	wchar_t ch;

	i = 0;
	while (i < slen)
	{
		if (total >= dlen)
			break;

		gb2312_byte_to_ucs(src + i, &ch);
		i += gb2312_sequence(*(src + i));

		total += ucs_byte_to_utf16lit(ch, ((dest) ? (dest + total) : NULL));
	}

	return total;
}

dword_t gb2312_byte_to_utf16big(const byte_t* src, byte_t* dest)
{
	wchar_t ch;

	gb2312_byte_to_ucs(src, &ch);

	return ucs_byte_to_utf16big(ch, dest);
}

dword_t gb2312_to_utf16big(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen)
{
	dword_t i, total = 0;
	wchar_t ch;

	i = 0;
	while (i < slen)
	{
		if (total >= dlen)
			break;

		gb2312_byte_to_ucs(src + i, &ch);
		i += gb2312_sequence(*(src + i));

		total += ucs_byte_to_utf16big(ch, ((dest) ? (dest + total) : NULL));
	}

	return total;
}

dword_t gb2312_byte_to_utf8(const byte_t* src, byte_t* dest)
{
	wchar_t ch;

	gb2312_byte_to_ucs(src, &ch);

	return ucs_byte_to_utf8(ch, dest);
}

dword_t gb2312_to_utf8(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen)
{
	dword_t i, total = 0;
	wchar_t ch;

	i = 0;
	while (i < slen)
	{
		if (total >= dlen)
			break;

		gb2312_byte_to_ucs(src + i, &ch);
		i += gb2312_sequence(*(src + i));

		total += ucs_byte_to_utf8(ch, ((dest) ? (dest + total) : NULL));
	}

	return total;
}

dword_t gb2312_byte_to_unn(const byte_t* src, byte_t* dest)
{
	wchar_t ch;

	gb2312_byte_to_ucs(src, &ch);

	return ucs_byte_to_unn(ch, dest);
}

dword_t gb2312_to_unn(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen)
{
	dword_t i, total = 0;
	wchar_t ch;

	i = 0;
	while (i < slen)
	{
		if (total >= dlen)
			break;

		gb2312_byte_to_ucs(src + i, &ch);

		i += gb2312_sequence(*(src + i));

		total += ucs_byte_to_unn(ch, ((dest) ? (dest + total) : NULL));
	}

	return total;
}

int gb2312_byte_to_mbs(const byte_t* src, schar_t* dest)
{
#if DEF_MBS == _GB2312
	int len;
	len = (int)gb2312_sequence(*src);
	if (dest)
		xmem_copy((void*)dest, (void*)src, len);
	return len;
#else
	return (int)gb2312_byte_to_utf8(src, (byte_t*)dest);
#endif
}

int gb2312_to_mbs(const byte_t* src, dword_t slen, schar_t* dest, int dlen)
{
#if DEF_MBS == _GB2312
	dlen = (dlen < (int)slen) ? dlen : (int)slen;
	if (dest)
		xmem_copy((void*)dest, (void*)src, dlen);
	return dlen;
#else
	return (int)gb2312_to_utf8(src, slen, (byte_t*)dest, dlen);
#endif
}

#endif //defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
//////////////////////////////////////////////////////////////////////////////////////////

dword_t utf8_sequence(byte_t b)
{
	return (dword_t)utf8_code_sequence(b);
}

int utf8_byte_to_ucs(const byte_t* src, wchar_t* dest)
{
	int n;
	unsigned short us = 0;

	n = utf8_seek_unicode((unsigned char*)src, &us);

	if (dest)
	{
		*dest = (wchar_t)us;
	}

	return n;
}

int utf8_to_ucs(const byte_t* src, dword_t slen, wchar_t* dest, int dlen)
{
	int seq, count = 0;
	dword_t i = 0;

	while (i < slen && count < dlen)
	{
		seq = utf8_sequence(*(src + i));
		if (!seq)
			break;

		count += utf8_byte_to_ucs(src + i, ((dest) ? dest + count : NULL));
		i += seq;
	}

	return count;
}

dword_t utf8_byte_to_utf16lit(const byte_t* src, byte_t* dest)
{
	wchar_t ch;

	utf8_byte_to_ucs(src, &ch);

	return ucs_byte_to_utf16lit(ch, dest);
}

dword_t utf8_byte_to_utf16big(const byte_t* src, byte_t* dest)
{
	wchar_t ch;

	utf8_byte_to_ucs(src, &ch);

	return ucs_byte_to_utf16big(ch, dest);
}

dword_t utf8_to_utf16lit(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen)
{
	dword_t i, total = 0;
	wchar_t ch;

	i = 0;
	while (i < slen)
	{
		if (total >= dlen)
			break;

		utf8_byte_to_ucs(src + i, &ch);
		i += utf8_sequence(*(src + i));

		total += ucs_byte_to_utf16lit(ch, ((dest) ? (dest + total) : NULL));
	}

	return total;
}

dword_t utf8_to_utf16big(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen)
{
	dword_t i, total = 0;
	wchar_t ch;

	i = 0;
	while (i < slen)
	{
		if (total >= dlen)
			break;

		utf8_byte_to_ucs(src + i, &ch);
		i += utf8_sequence(*(src + i));

		total += ucs_byte_to_utf16big(ch, ((dest) ? (dest + total) : NULL));
	}

	return total;
}

#if defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
dword_t utf8_byte_to_gb2312(const byte_t* src, byte_t* dest)
{
	wchar_t ch;

	utf8_byte_to_ucs(src, &ch);

	return ucs_byte_to_gb2312(ch, dest);
}

dword_t utf8_to_gb2312(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen)
{
	dword_t i, total = 0;
	wchar_t ch;

	i = 0;
	while (i < slen)
	{
		if (total >= dlen)
			break;

		utf8_byte_to_ucs(src + i, &ch);
		i += utf8_sequence(*(src + i));

		total += ucs_byte_to_gb2312(ch, ((dest) ? (dest + total) : NULL));
	}

	return total;
}
#endif

dword_t utf8_byte_to_unn(const byte_t* src, byte_t* dest)
{
	wchar_t ch;

	utf8_byte_to_ucs(src, &ch);

	return ucs_byte_to_unn(ch, dest);
}

dword_t utf8_to_unn(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen)
{
	dword_t i, total = 0;
	wchar_t ch;

	i = 0;
	while (i < slen)
	{
		if (total >= dlen)
			break;

		utf8_byte_to_ucs(src + i, &ch);
		i += utf8_sequence(*(src + i));

		total += ucs_byte_to_unn(ch, ((dest) ? (dest + total) : NULL));
	}

	return total;
}

int utf8_byte_to_mbs(const byte_t* src, schar_t* dest)
{
#if DEF_MBS == _GB2312
	return (int)utf8_byte_to_gb2312(src, (byte_t*)dest);
#else
	int len;
	len = (int)utf8_sequence(*src);
	if (dest)
		xmem_copy((void*)dest, (void*)src, len);
	return len;
#endif
}

int utf8_to_mbs(const byte_t* src, dword_t slen, schar_t* dest, int dlen)
{
#if DEF_MBS == _GB2312
	return (int)utf8_to_gb2312(src, slen, (byte_t*)dest, dlen);
#else
	dlen = (dlen < (int)slen) ? dlen : (int)slen;
	if (dest)
		xmem_copy((void*)dest, (void*)src, dlen);
	return dlen;
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////

dword_t utf16_sequence(byte_t b)
{
	return 2;
}

//unicode:	0000 0000 yyyy yyyy yyxx xxxx xxxx
//utf16:	1101 10yy yyyy yyyy 1101 11xx xxxx xxxx
int utf16lit_byte_to_ucs(const byte_t* src, wchar_t* dest)
{
	int n;
	unsigned short us = 0;

	n = utf16lit_seek_unicode((unsigned char*)src, &us);

	if (dest)
	{
		*dest = (wchar_t)us;
	}

	return n;
}

int utf16big_byte_to_ucs(const byte_t* src, wchar_t* dest)
{
	int n;
	unsigned short us = 0;

	n = utf16big_seek_unicode((unsigned char*)src, &us);

	if (dest)
	{
		*dest = (wchar_t)us;
	}

	return n;
}


int utf16lit_to_ucs(const byte_t* src, dword_t slen, wchar_t* dest, int dlen)
{
	dword_t i = 0;
	int seq, count = 0;

	while (i < slen && count < dlen)
	{
		seq = utf16lit_code_sequence(*(src + i));
		if (!seq)
			break;

		count += utf16lit_byte_to_ucs(src + i, ((dest) ? dest + count : NULL));
		i += seq;
	}

	return count;
}

int utf16big_to_ucs(const byte_t* src, dword_t slen, wchar_t* dest, int dlen)
{
	dword_t i = 0;
	int seq, count = 0;

	while (i < slen && count < dlen)
	{
		seq = utf16big_code_sequence(*(src + i));
		if (!seq)
			break;

		count += utf16big_byte_to_ucs(src + i, ((dest) ? dest + count : NULL));
		i += seq;
	}

	return count;
}


dword_t utf16lit_byte_to_utf8(const byte_t* src, byte_t* dest)
{
	wchar_t ch;

	utf16lit_byte_to_ucs(src, &ch);

	return ucs_byte_to_utf8(ch, dest);
}

dword_t utf16big_byte_to_utf8(const byte_t* src, byte_t* dest)
{
	wchar_t ch;

	utf16big_byte_to_ucs(src, &ch);

	return ucs_byte_to_utf8(ch, dest);
}

dword_t utf16lit_to_utf8(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen)
{
	dword_t i, total = 0;
	wchar_t ch;

	i = 0;
	while (i < slen)
	{
		if (total >= dlen)
			break;

		utf16lit_byte_to_ucs(src + i, &ch);
		i += utf16_sequence(*(src + i));

		total += ucs_byte_to_utf8(ch, ((dest) ? (dest + total) : NULL));
	}

	return total;
}

dword_t utf16big_to_utf8(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen)
{
	dword_t i, total = 0;
	wchar_t ch;

	i = 0;
	while (i < slen)
	{
		if (total >= dlen)
			break;

		utf16big_byte_to_ucs(src + i, &ch);
		i += utf16_sequence(*(src + i));

		total += ucs_byte_to_utf8(ch, ((dest) ? (dest + total) : NULL));
	}

	return total;
}

#if defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
dword_t utf16lit_byte_to_gb2312(const byte_t* src, byte_t* dest)
{
	wchar_t ch;

	utf16lit_byte_to_ucs(src, &ch);

	return ucs_byte_to_gb2312(ch, dest);
}

dword_t utf16big_byte_to_gb2312(const byte_t* src, byte_t* dest)
{
	wchar_t ch;

	utf16big_byte_to_ucs(src, &ch);

	return ucs_byte_to_gb2312(ch, dest);
}

dword_t utf16lit_to_gb2312(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen)
{
	dword_t i, total = 0;
	wchar_t ch;

	i = 0;
	while (i < slen)
	{
		if (total >= dlen)
			break;

		utf16lit_byte_to_ucs(src + i, &ch);
		i += utf16_sequence(*(src + i));

		total += ucs_byte_to_gb2312(ch, ((dest) ? (dest + total) : NULL));
	}

	return total;
}

dword_t utf16big_to_gb2312(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen)
{
	dword_t i, total = 0;
	wchar_t ch;

	i = 0;
	while (i < slen)
	{
		if (total >= dlen)
			break;

		utf16big_byte_to_ucs(src + i, &ch);
		i += utf16_sequence(*(src + i));

		total += ucs_byte_to_gb2312(ch, ((dest) ? (dest + total) : NULL));
	}

	return total;
}
#endif

dword_t utf16lit_byte_to_unn(const byte_t* src, byte_t* dest)
{
	wchar_t ch;

	utf16lit_byte_to_ucs(src, &ch);

	return ucs_byte_to_unn(ch, dest);
}

dword_t utf16big_byte_to_unn(const byte_t* src, byte_t* dest)
{
	wchar_t ch;

	utf16big_byte_to_ucs(src, &ch);

	return ucs_byte_to_unn(ch, dest);
}

dword_t utf16lit_to_unn(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen)
{
	dword_t i, total = 0;
	wchar_t ch;

	i = 0;
	while (i < slen)
	{
		if (total >= dlen)
			break;

		utf16lit_byte_to_ucs(src + i, &ch);
		i += utf16_sequence(*(src + i));

		total += ucs_byte_to_unn(ch, ((dest) ? (dest + total) : NULL));
	}

	return total;
}

dword_t utf16big_to_unn(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen)
{
	dword_t i, total = 0;
	wchar_t ch;

	i = 0;
	while (i < slen)
	{
		if (total >= dlen)
			break;

		utf16big_byte_to_ucs(src + i, &ch);
		i += utf16_sequence(*(src + i));

		total += ucs_byte_to_unn(ch, ((dest) ? (dest + total) : NULL));
	}

	return total;
}

int utf16lit_byte_to_mbs(const byte_t* src, schar_t* dest)
{
#if DEF_MBS == _GB2312
	return (int)utf16lit_byte_to_gb2312(src, (byte_t*)dest);
#else
	return (int)utf16lit_byte_to_utf8(src, (byte_t*)dest);
#endif
}

int utf16big_byte_to_mbs(const byte_t* src, schar_t* dest)
{
#if DEF_MBS == _GB2312
	return (int)utf16big_byte_to_gb2312(src, (byte_t*)dest);
#else
	return (int)utf16big_byte_to_utf8(src, (byte_t*)dest);
#endif
}

int utf16lit_to_mbs(const byte_t* src, dword_t slen, schar_t* dest, int dlen)
{
#if DEF_MBS == _GB2312
	return (int)utf16lit_to_gb2312(src, slen, (byte_t*)dest, dlen);
#else
	return (int)utf16lit_to_utf8(src, slen, (byte_t*)dest, dlen);
#endif
}

int utf16big_to_mbs(const byte_t* src, dword_t slen, schar_t* dest, int dlen)
{
#if DEF_MBS == _GB2312
	return (int)utf16lit_to_gb2312(src, slen, (byte_t*)dest, dlen);
#else
	return (int)utf16lit_to_utf8(src, slen, (byte_t*)dest, dlen);
#endif
}
//////////////////////////////////////////////////////////////////////////////////////////
dword_t ucs_sequence(wchar_t ch)
{
	return 1;
}

#if defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
dword_t ucs_byte_to_gb2312(wchar_t ch, byte_t* dest)
{
#ifdef XDK_SUPPORT_MBCS
	return (dword_t)ucs_byte_to_gbk(ch, dest);
#else
	return (dword_t)unicode_seek_gb2312((unsigned short)ch, dest);
#endif
}

dword_t ucs_to_gb2312(const wchar_t* src, int slen, byte_t* dest, dword_t dlen)
{
#ifdef XDK_SUPPORT_MBCS
	return (dword_t)ucs_to_gbk(src, slen, dest, dlen);
#else
	dword_t count = 0;
	int seq, i = 0;

	if (slen < 0)
	{
		slen = 0;
		while (src && *(src + slen))
			slen++;
	}

	while (i < slen && count < dlen)
	{
		seq = ucs_sequence(*(src + i));
		if (!seq)
			break;
		
		count += ucs_byte_to_gb2312(*(src + i), ((dest) ? dest + count : NULL));

		i += seq;
	}

	return count;
#endif
}
#endif

dword_t ucs_byte_to_utf8(wchar_t ch, byte_t* dest)
{
	return unicode_seek_utf8((unsigned short)ch, dest);
}

dword_t ucs_to_utf8(const wchar_t* src, int slen, byte_t* dest, dword_t dlen)
{
	dword_t count = 0;
	int seq, i = 0;

	if (slen < 0)
	{
		slen = 0;
		while (src && *(src + slen))
			slen++;
	}

	while (i < slen && count < dlen)
	{
		seq = ucs_sequence(*(src + i));
		if (!seq)
			break;

		count += ucs_byte_to_utf8(*(src + i), ((dest) ? dest + count : NULL));

		i += seq;
	}

	return count;
}

dword_t ucs_byte_to_utf16lit(wchar_t ch, byte_t* dest)
{
	return unicode_seek_utf16lit((unsigned short)ch, dest);
}

dword_t ucs_to_utf16lit(const wchar_t* src, int slen, byte_t* dest, dword_t dlen)
{
	dword_t count = 0;
	int seq, i = 0;

	if (slen < 0)
	{
		slen = 0;
		while (src && *(src + slen))
			slen++;
	}

	while (i < slen && count < dlen)
	{
		seq = ucs_sequence(*(src + i));
		if (!seq)
			break;

		count += ucs_byte_to_utf16lit(*(src + i), ((dest) ? dest + count : NULL));
		i += seq;
	}

	return count;
}

dword_t ucs_byte_to_utf16big(wchar_t ch, byte_t* dest)
{
	return unicode_seek_utf16big((unsigned short)ch, dest);
}

dword_t ucs_to_utf16big(const wchar_t* src, int slen, byte_t* dest, dword_t dlen)
{
	dword_t count = 0;
	int seq, i = 0;

	if (slen < 0)
	{
		slen = 0;
		while (src && *(src + slen))
			slen++;
	}

	while (i < slen && count < dlen)
	{
		seq = ucs_sequence(*(src + i));
		if (!seq)
			break;

		count += ucs_byte_to_utf16big(*(src + i), ((dest) ? dest + count : NULL));

		i += seq;
	}

	return count;
}

dword_t ucs_byte_to_unn(wchar_t ch, byte_t* buf)
{
	if (buf)
	{
		buf[0] = GETLBYTE(ch);
		buf[1] = GETHBYTE(ch);
	}
	return 2;
}

dword_t ucs_to_unn(const wchar_t* src, int slen, byte_t* dest, dword_t dlen)
{
	dword_t i = 0;
	int pos = 0;

	if (slen < 0)
	{
		slen = 0;
		while (src && *(src + slen))
			slen++;
	}

	while (pos < slen && i < dlen)
	{
		i += ucs_byte_to_unn(src[pos], ((dest) ? dest + i : NULL));
		pos++;
	}

	return i;
}

int ucs_byte_to_mbs(wchar_t ch, schar_t* buf)
{
#if DEF_MBS == _GB2312
	return (int)ucs_byte_to_gb2312(ch, (byte_t*)buf);
#else
	return (int)ucs_byte_to_utf8(ch, (byte_t*)buf);
#endif
}

int ucs_to_mbs(const wchar_t* src, int slen, schar_t* dest, int dlen)
{
	if (slen < 0)
	{
		slen = 0;
		while (src && *(src + slen))
			slen++;
	}

#if DEF_MBS == _GB2312
	return (int)ucs_to_gb2312(src, slen, (byte_t*)dest, dlen);
#else
	return (int)ucs_to_utf8(src, slen, (byte_t*)dest, dlen);
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
dword_t mbs_sequence(schar_t b)
{
#if DEF_MBS == _GB2312
	return gb2312_sequence((byte_t)b);
#else
	return utf8_sequence(b);
#endif
}

int mbs_byte_to_ucs(const schar_t* src, wchar_t* dest)
{
#if DEF_MBS == _GB2312
	return gb2312_byte_to_ucs((byte_t*)src, dest);
#else
	return utf8_byte_to_ucs((byte_t*)src, dest);
#endif
}

int mbs_to_ucs(const schar_t* src, int slen, wchar_t* dest, int dlen)
{
	if (slen < 0)
	{
		slen = 0;
		while (src && *(src + slen))
			slen++;
	}

#if DEF_MBS == _GB2312
	return gb2312_to_ucs((byte_t*)src, slen, dest, dlen);
#else
	return utf8_to_ucs((byte_t*)src, slen, dest, dlen);
#endif
}

dword_t mbs_byte_to_utf8(const schar_t* src, byte_t* dest)
{
#if DEF_MBS == _GB2312
	return gb2312_byte_to_utf8((byte_t*)src, dest);
#else
	int len;
	len = utf8_sequence(*(byte_t*)src);
	if (dest)
		xmem_copy((void*)dest, (void*)src, len);
	return len;
#endif
}

dword_t mbs_to_utf8(const schar_t* src, int slen, byte_t* dest, dword_t dlen)
{
	if (slen < 0)
	{
		slen = 0;
		while (src && *(src + slen))
			slen++;
	}

#if DEF_MBS == _GB2312
	return gb2312_to_utf8((byte_t*)src, slen, dest, dlen);
#else
	dlen = (dlen < slen) ? dlen : slen;
	if (dest)
		xmem_copy((void*)dest, (void*)src, dlen);
	return dlen;
#endif
}

dword_t mbs_byte_to_utf16lit(const schar_t* src, byte_t* dest)
{
#if DEF_MBS == _GB2312
	return gb2312_byte_to_utf16lit((byte_t*)src, dest);
#else
	return utf8_byte_to_utf16lit((byte_t*)src, dest);
#endif
}

dword_t mbs_byte_to_utf16big(const schar_t* src, byte_t* dest)
{
#if DEF_MBS == _GB2312
	return gb2312_byte_to_utf16big((byte_t*)src, dest);
#else
	return utf8_byte_to_utf16big((byte_t*)src, dest);
#endif
}

dword_t mbs_to_utf16lit(const schar_t* src, int slen, byte_t* dest, dword_t dlen)
{
	if (slen < 0)
	{
		slen = 0;
		while (src && *(src + slen))
			slen++;
	}

#if DEF_MBS == _GB2312
	return gb2312_to_utf16lit((byte_t*)src, slen, dest, dlen);
#else
	return utf8_to_utf16lit((byte_t*)src, slen, dest, dlen);
#endif
}

dword_t mbs_to_utf16big(const schar_t* src, int slen, byte_t* dest, dword_t dlen)
{
	if (slen < 0)
	{
		slen = 0;
		while (src && *(src + slen))
			slen++;
	}

#if DEF_MBS == _GB2312
	return gb2312_to_utf16big((byte_t*)src, slen, dest, dlen);
#else
	return utf8_to_utf16big((byte_t*)src, slen, dest, dlen);
#endif
}

#if defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
dword_t mbs_byte_to_gb2312(const schar_t* src, byte_t* dest)
{
#if DEF_MBS == _GB2312
	dword_t len;
	len = gb2312_sequence(*(byte_t*)src);
	if (dest)
		xmem_copy((void*)dest, (void*)src, len);
	return len;
#else
	return utf8_byte_to_gb2312((byte_t*)src, dest);
#endif
}

dword_t mbs_to_gb2312(const schar_t* src, int slen, byte_t* dest, dword_t dlen)
{
	if (slen < 0)
	{
		slen = 0;
		while (src && *(src + slen))
			slen++;
	}

#if DEF_MBS == _GB2312
	dlen = (dlen < (dword_t)slen) ? dlen : (dword_t)slen;
	if (dest)
		xmem_copy((void*)dest, (void*)src, dlen);
	return dlen;
#else
	return utf8_to_gb2312((byte_t*)src, slen, dest, dlen);
#endif
}
#endif

dword_t mbs_byte_to_unn(const schar_t* src, byte_t* dest)
{
#if DEF_MBS == _GB2312
	return gb2312_byte_to_unn((byte_t*)src, dest);
#else
	return utf8_byte_to_unn((byte_t*)src, dest);
#endif
}

dword_t mbs_to_unn(const schar_t* src, int slen, byte_t* dest, dword_t dlen)
{
	if (slen < 0)
	{
		slen = 0;
		while (src && *(src + slen))
			slen++;
	}

#if DEF_MBS == _GB2312
	return gb2312_to_unn((byte_t*)src, slen, dest, dlen);
#else
	return utf8_to_unn((byte_t*)src, slen, dest, dlen);
#endif
}

int mbs_byte_to_mbs(const schar_t* src, schar_t* dest)
{
	int len;
	len = mbs_sequence(*(schar_t*)src);
	if (dest)
		xmem_copy((void*)dest, (void*)src, len);
	return len;
}

int mbs_to_mbs(const schar_t* src, int slen, schar_t* dest, int dlen)
{
	if (slen < 0)
	{
		slen = 0;
		while (src && *(src + slen))
			slen++;
	}

	dlen = (dlen < slen) ? dlen : slen;
	if (dest)
		xmem_copy(dest, (void*)src, dlen);
	return dlen;
}

/********************************************************************************************/
dword_t unn_sequence(byte_t b)
{
#ifdef _UNICODE
	return 2;
#else
	return 1;
#endif
}

int unn_byte_to_ucs(const byte_t* src, wchar_t* dest)
{
	*dest = MAKESWORD(src[0], src[1]);

	return 1;
}

int unn_to_ucs(const byte_t* src, dword_t slen, wchar_t* dest, int dlen)
{
	int i = 0;
	dword_t pos = 0;

	while (pos < slen && i < dlen)
	{
		i += unn_byte_to_ucs(src + pos, ((dest) ? dest + i : NULL));
		pos += unn_sequence(src[pos]);
	}

	return i;
}

#if defined(GPL_SUPPORT_ACP) || defined(XDK_SUPPORT_MBCS)
dword_t unn_byte_to_gb2312(const byte_t* src, byte_t* dest)
{
	wchar_t ch;

	unn_byte_to_ucs(src, &ch);

	return ucs_byte_to_gb2312(ch, dest);
}

dword_t unn_to_gb2312(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen)
{
	dword_t i = 0;
	dword_t pos = 0;

	while (pos < slen && i < dlen)
	{
		i += unn_byte_to_gb2312(src + pos, ((dest) ? dest + i : NULL));
		pos += unn_sequence(src[pos]);
	}

	return i;
}
#endif

dword_t unn_byte_to_utf8(const byte_t* src, byte_t* dest)
{
	wchar_t ch;

	unn_byte_to_ucs(src, &ch);

	return ucs_byte_to_utf8(ch, dest);
}

dword_t unn_to_utf8(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen)
{
	dword_t i = 0;
	dword_t pos = 0;

	while (pos < slen && i < dlen)
	{
		i += unn_byte_to_utf8(src + pos, ((dest) ? dest + i : NULL));
		pos += unn_sequence(src[pos]);
	}

	return i;
}

dword_t unn_byte_to_utf16lit(const byte_t* src, byte_t* dest)
{
	wchar_t ch;

	unn_byte_to_ucs(src, &ch);

	return ucs_byte_to_utf16lit(ch, dest);
}

dword_t unn_to_utf16lit(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen)
{
	dword_t i = 0;
	dword_t pos = 0;

	while (pos < slen && i < dlen)
	{
		i += unn_byte_to_utf16lit(src + pos, ((dest) ? dest + i : NULL));
		pos += unn_sequence(src[pos]);
	}

	return i;
}

dword_t unn_byte_to_utf16big(const byte_t* src, byte_t* dest)
{
	wchar_t ch;

	unn_byte_to_ucs(src, &ch);

	return ucs_byte_to_utf16big(ch, dest);
}

dword_t unn_to_utf16big(const byte_t* src, dword_t slen, byte_t* dest, dword_t dlen)
{
	dword_t i = 0;
	dword_t pos = 0;

	while (pos < slen && i < dlen)
	{
		i += unn_byte_to_utf16big(src + pos, ((dest) ? dest + i : NULL));
		pos += unn_sequence(src[pos]);
	}

	return i;
}

int unn_byte_to_mbs(const byte_t* src, schar_t* dest)
{
	wchar_t ch;

	unn_byte_to_ucs(src, &ch);

	return ucs_byte_to_unn(ch, (byte_t*)dest);
}

int unn_to_mbs(const byte_t* src, dword_t slen, schar_t* dest, int dlen)
{
	int i = 0;
	dword_t pos = 0;

	while (pos < slen && i < dlen)
	{
		i += unn_byte_to_mbs(src + pos, ((dest) ? dest + i : NULL));
		pos += unn_sequence(src[pos]);
	}

	return i;
}


