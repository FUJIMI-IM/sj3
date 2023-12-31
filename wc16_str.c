/*
 * Copyright (c) 1994  Sony Corporation
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of Sony Corporation
 * shall not be used in advertising or otherwise to promote the sale, use
 * or other dealings in this Software without prior written authorization
 * from Sony Corporation.
 *
 */

/*
 * $SonyRCSfile: wc16_str.c,v $  
 * $SonyRevision: 1.3 $ 
 * $SonyDate: 1994/11/17 06:49:21 $
 */


#include "sj_sysvdef.h"
#include <stdio.h>
#include <locale.h>
#include "sj_euc.h"
#include "wchar16.h"
#if defined(__sony_news) && defined(SVR4)
#include <sonyils.h>
#endif
#include "sj3lib.h"

#define MSB 0x80

#ifndef TRUE
#define TRUE (1)
#define FALSE (0)
#endif

#ifndef __sony_news 
#define sjis2euc        sj3_sjis2euc
#define euc2sjis        sj3_euc2sjis

#define issjis1(c)   (((0x81 <= (c)) && ((c) <= 0x9f)) || \
                      ((0xe0 <= (c)) && ((c) <= 0xfc)))
#define iseuc(c)     ((0xa1 <= (c)) && ((c) <= 0xfe))
#define iskana(c)   ((0xa1 <= (c)) && ((c) <= 0xdf))
#endif

extern int current_locale;

int sj3_iswcntrl16(wchar16_t wc)
{
	if ((wc < 0x20) || (wc == 0x7f)) return TRUE;

	return FALSE;
}

int  sj3_iswupper16(wchar16_t wc)
{
	if (((0x40 < wc) && (wc < 0x5b)) || 
	    ((0xa3c0 < wc) && (wc < 0xa3db))) return TRUE;

	return FALSE;
}

int sj3_iswdigit16(wchar16_t wc)
{
	if (((0x29 < wc) && (wc < 0x3a)) || 
	    ((0xa3af < wc) && (wc < 0xa3ba))) return TRUE;

	return FALSE;
}

int sj3_iswxdigit16(wchar16_t wc)
{
	if (((0x29 < wc) && (wc < 0x3a)) || 
	    ((0xa3af < wc) && (wc < 0xa3ba))) return TRUE;

	if (((0x60 < wc) && (wc < 0x67)) ||
	    ((0x40 < wc) && (wc < 0x47))) return TRUE;
	
	if (((0xa3c0 < wc) && (wc < 0xa3c7)) ||
	    ((0xa3e0 < wc) && (wc < 0xa3e6))) return TRUE;

	return FALSE;
}

int sj3_wslen16(wchar16_t *ws)
{
	int i=0;

	if (!ws) return 0;

	while (*ws++) i++;
	return i;
}

int sj3_wscmp16(wchar16_t *ws1, wchar16_t *ws2)
{
	while ((*ws1 && *ws2) && (*ws1 == *ws2)) {
		ws1++;
		ws2++;
	}
	if (!*ws1 && !*ws2) return 0;
	if (!*ws1) return -1;
	if (!*ws2) return  1;
	if (*ws1 < *ws2) return -1;
	return  1;
}

int sj3_wsncmp16(wchar16_t *ws1, wchar16_t *ws2, int n)
{
	int i=0;

	while ((*ws1 && *ws2) && (*ws1 == *ws2) && (i < n)) {
		ws1++;
		ws2++;
		i++;
	}
	if (!*ws1 && !*ws2) return 0;
	if (i == n) return 0;
	if (!*ws1) return -1;
        if (!*ws2) return  1;
        if (*ws1 < *ws2) return -1;
	return 1;
}

wchar16_t *sj3_wscpy16(wchar16_t *ws1, wchar16_t *ws2)
{
	wchar16_t *ws;

	if (!ws2) return NULL;
	ws = ws1;
	while (*ws2) {
		*ws++ = *ws2++;
	}
	*ws = 0;

	return ws1;
}

wchar16_t *sj3_wsncpy16(wchar16_t *ws1, wchar16_t *ws2, int n)
{
	wchar16_t *ws;
	int i=0;

	if (!ws2) return NULL;
	ws = ws1;
	while (*ws2 && (i < n)) {
		*ws++ = *ws2++;
		i++;
	}
	if (i < n) *ws = 0;

	return ws1;
}

wchar16_t *sj3_wscat16(wchar16_t *ws1, wchar16_t *ws2)
{
	wchar16_t *ws;

	if (!ws1) return NULL;
	if (!ws2) return NULL;

	ws = ws1;

	while (*ws) ws++;
	while (*ws2) {
		*ws++ = *ws2++;
	}
	*ws = 0;

	return ws1;
}


wchar16_t sj3_euc2wc16(unsigned int code)
{
	wchar16_t wc = 0;

	if (((code >> 16) & 0xff) == SS3) {
		wc = code & 0xff7f;
	} else if (((code >> 8) & 0xff) == SS2) {
		wc = code & 0xff;
	} else {
		wc = code;
	} 

	return wc;
}
		
wchar16_t sj3_sjis2wc16(unsigned int code)
{
	unsigned short ch;

	if ((code >> 8) & 0xff) {
		ch = sjis2euc(code);
	} else if (code & MSB) {
		ch = (SS2 << 8) + (code & 0xff);
	} else {
		return code;
	}
	return (sj3_euc2wc16(ch));
}

unsigned int sj3_wc2euc16(wchar16_t wc)
{
	unsigned int code = 0;
	unsigned char tmp;

	tmp = (wc >> 8) & 0xff;
	if (tmp) {
		if (wc & MSB) {
			code = wc;
		} else {
			code = (SS3 << 16) + wc + MSB;
		}
	} else {
		if (wc & MSB) {
			code = (SS2 << 8) + wc;
		} else {
			code = wc;
		}
	}

	return code;
}

unsigned int sj3_wc2sjis16(wchar16_t wc)
{
	unsigned int ch;

	ch = sj3_wc2euc16(wc);
	if ((ch >> 16) == SS3) {
		return 0;
	} else if ((ch >> 8) == SS2) {
		return (ch & 0xff);
	} else if ((ch >> 8) & 0xff) {
		return euc2sjis(ch);
	} else {
		return wc;
	}
}

int sj3_wcs2eucs16(unsigned char *mb, wchar16_t *ws, int n)
{
	int i = 0;
	unsigned int code;
	unsigned char c;
	
	while (*ws && (i < n)) {
		code = sj3_wc2euc16(*ws);
		if ((c = ((code >> 16) & 0xff))) {
			*mb++ = c;
			i++;
			if (i == n) break;
			*mb++ = ((code >> 8) & 0xff);
			i++;
			if (i == n) break;
			*mb++ = code & 0xff;
		        i++;
			if (i == n) break;
		} else if ((c = ((code >> 8) & 0xff))) {
			*mb++ = c;
			i++;
			if (i == n) break;
			*mb++ = code & 0xff;
		        i++;
			if (i == n) break;
		} else if ((c = (code & 0xff))) {
			*mb++ = c;
			i++;
		} else {
			return -1;
		}
		ws++;
	}

	if (!*ws) {
		*mb = '\0';
	}

	return i;
}
	
int sj3_wcs2sjiss16(unsigned char *mb, wchar16_t *ws, int n)
{
	int i = 0;
	unsigned int code;
	unsigned char c;
	
	while (*ws && (i < n)) {
		code = sj3_wc2sjis16(*ws);
		if ((c = ((code >> 8) & 0xff))) {
			*mb++ = c;
			i++;
			if (i == n) break;
			*mb++ = code & 0xff;
			i++;
			if (i == n) break;
		} else if ((c = (code & 0xff))) {
			*mb++ = c;
			i++;
		} else {
			return -1;
		}
		ws++;
	}

	if (!*ws) {
		*mb = '\0';
	}

	return i;
}

int sj3_eucs2wcs16(wchar16_t *ws, unsigned char *mb, int n)
{
	int i = 0;
	unsigned int code;

	while (*mb && (i < n)) {
		if (*mb == SS3) {
			code = *mb++ << 16;
			code += *mb++ << 8;
			code += *mb++;
		} else if ((*mb == SS2) || (*mb & 0x80)) {
			code = *mb++ << 8;
			code += *mb++;
		} else {
			code = *mb++;
		}
		*ws = sj3_euc2wc16(code);
		if (!*ws) return -1;
		ws++;
		i++;
	}
	if (!*mb) {
		*ws = 0;
	}

	return i;
}

int sj3_sjiss2wcs16(wchar16_t *ws, unsigned char *mb, int n)
{
	int i = 0;
	unsigned int code;

	while (*mb && (i < n)) {
		if (issjis1(*mb)) {
			code = *mb++ << 8;
			code += *mb++;
		} else {
			code = *mb++;
		}
		*ws = sj3_sjis2wc16(code);
		if (!*ws) return -1;
		ws++;
		i++;
	}
	if (!*mb) {
		*ws = 0;
	}

	return i;
}

int sj3_mbstowcs16(wchar16_t *ws, unsigned char *mb, int n)
{
	if (current_locale == LC_CTYPE_EUC)
	  return sj3_eucs2wcs16(ws, mb, n);
	else 
	  return sj3_sjiss2wcs16(ws, mb, n);
}

int sj3_wcstombs16(unsigned char *mb, wchar16_t *ws, int n)
{
	if (current_locale == LC_CTYPE_EUC)
	  return sj3_wcs2eucs16(mb, ws, n);
	else
	  return sj3_wcs2sjiss16(mb, ws, n);
}


