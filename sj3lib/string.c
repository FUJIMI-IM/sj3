/*-
 * SPDX-License-Identifier: MIT-open-group
 * 
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
 */

#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

#include "sj_euc.h"

#ifndef TRUE
#define TRUE (1)
#define FALSE (0)
#endif

#define sjis2euc        sj3_sjis2euc
#define euc2sjis        sj3_euc2sjis

#define issjis1(c)   (((0x81 <= (c)) && ((c) <= 0x9f)) || \
                      ((0xe0 <= (c)) && ((c) <= 0xfc)))
#define iseuc(c)     ((0xa1 <= (c)) && ((c) <= 0xfe))
#define iskana(c)   ((0xa1 <= (c)) && ((c) <= 0xdf))

unsigned short sjis2euc(), euc2sjis();

void sj_sjis2jis(), sj_jis2sjis();

int
sj3_str_sjistoeuc(out, outlen, in, sjis_default, useflag)
unsigned char *out, *in, *sjis_default;
int outlen, *useflag;
{
	unsigned char  *sjis = in, *euc = out;
	int            n = 0;
	unsigned short code, default_code;

	euc[0] = '\0';
	*useflag = 0;

 	if (!sjis) return 0;
 
	while (*sjis && n < outlen) {
		if (issjis1(*sjis)) {
			code = sjis2euc( (*sjis << 8) | *(sjis+1) );
 			if (n + 2 > outlen) return -1;
			if (code == 0) { 
				default_code = sjis2euc((sjis_default[0] << 8) 
						   | sjis_default[1]);
				euc[n++] = default_code >> 8;
				euc[n++] = default_code & 0xff;
				(*useflag)++;
			} else {
				euc[n++] = code >> 8;
				euc[n++] = code & 0xff;
			}
			sjis += 2;
		} else {
			if( *sjis >= 0xa1  &&  *sjis <= 0xdf ) {  
 				if (n + 2 > outlen) return -1;
			        euc[n++] = SS2;                 
			}
			euc[n++] = *sjis;
			sjis++;
		}

	}
	if (n > outlen) return -1;
	
	euc[n] = '\0';
	return n;
}


int
sj3_str_euctosjis(out, outlen, in, sjis_default, useflag)
unsigned char *out, *in, *sjis_default;
int outlen, *useflag;
{
	unsigned char  *euc = in, *sjis = out;
	int            n = 0;
	unsigned short code, default_code;

	sjis[0] = '\0';
	*useflag = 0;

	if (!euc) return 0;

	while (*euc && n < outlen) {
		if (iseuc(*euc)) {
			code = euc2sjis( (*euc << 8) | *(euc+1) );
			if (n + 2 > outlen) return -1;
			if (code == 0) {
				sjis[n++] = sjis_default[0];
				sjis[n++] = sjis_default[1];
				(*useflag)++;
			} else {
				sjis[n++] = code >> 8;
				sjis[n++] = code & 0xff;
			}
			euc += 2;
		} else if (*euc == SS3) {
			if (n + 2 > outlen) return -1;
			sjis[n++] = sjis_default[0];
			sjis[n++] = sjis_default[1];
			euc += 3;
			(*useflag)++;
		} else if (*euc == SS2) {
			sjis[n++] = *(euc + 1);
			euc += 2;
		} else {
			sjis[n++] = *euc;
			euc++;
		}
	}
	if (n > outlen) return -1;

	sjis[n] = '\0';
	return n;
}
        
int
sj3_sjistoeuclen(str, max)
unsigned char *str;
int max;
{
        register int len = 0, bytes = 1;

        if (!max) return 0;
	while ((bytes <= max) && *str) {
		if (issjis1(*str)) {
                        str++;
                        bytes++;
                        if (!*str) break;      
                        len += 2;
		} else if (*str & 0x80) {
			len += 2;
		} else {
                        len++;
		}
                str++; 
                bytes++;
	}
        return len;
}
	
#define MASK    0x7f
#define MSB     0x80

static  unsigned char def_char[] = {0x81, 0x40};

int 
sj3_sjistoeuc(e, elen, s, slen)
unsigned char *s, *e;
int slen, elen;
{
	int dummy;

	return sj3_str_sjistoeuc(e, elen, s, def_char, &dummy);
}

int 
sj3_euctosjis(s, slen, e, elen)
unsigned char *s, *e;
int slen, elen;
{
	int dummy;

	return sj3_str_euctosjis(s, slen, e, def_char, &dummy);
}

void
sj_euc2sjis (s)
unsigned char *s;
{
        s[0] &= MASK;
        s[1] &= MASK;
        sj_jis2sjis (s);
}

void
sj_jis2sjis (s)
unsigned char  *s;
{
        register int    high, low;
        register int    nh, nl;

        high = s[0];
        low = s[1];
        nh = ((high - 0x21) >> 1) + 0x81;
        if (nh > 0x9f)
                nh += 0x40;
	if (high & 1) {
                nl = low + 0x1f;
                if (low > 0x5f)
                        nl ++;
	}
        else
                nl = low + 0x7e;
        s[0] = nh;
        s[1] = nl;
}

void
sj_sjis2euc (s)
unsigned char  *s;
{
        sj_sjis2jis (s);
        s[0] |= MSB;
        s[1] |= MSB;
}

void
sj_sjis2jis (s)
unsigned char  *s;
{
        register int    byte1, byte2;
        register unsigned char *sp;

        sp = s;
        byte1 = *sp++;
        byte2 = *sp--;

        if (byte1 <= 0x80 || byte1 >= 0xf0
         || ( byte1>=0xa0 && byte1<=0xdf )
	    || byte2 <= 0x3f || byte2 >= 0xfe || byte2==0x7f ) {
                        byte1 = 0x81;
                        byte2 = 0x40;
	    }
      byte1 -= (byte1 >= 0xa0) ? 0xc1 : 0x81;
	if (byte2 >= 0x9f) {
                *sp++ = (byte1 << 1) + 0x22;
                *sp = byte2 - 0x7e;
	}
	else {
                *sp++ = (byte1 << 1) + 0x21;
	      *sp = byte2 - ((byte2 <= 0x7e) ? 0x1f : 0x20 );
	}
}

unsigned short
sj3_jis2sjis(code)
unsigned short  code;
{
        unsigned char tmp[3];
                        
        tmp[0] = (code >> 8) & 0xff;
        tmp[1] = code & 0xff;
        tmp[2] = '\0';
                
        sj_jis2sjis((unsigned char *) tmp);
                        
        code = (tmp[0] << 8) + tmp[1];
         
        return(code);
}

unsigned short
sj3_jis2euc(code)
unsigned short  code;
{
        unsigned char tmp[3];
                        
        tmp[0] = (code >> 8) & 0xff;
        tmp[1] = code & 0xff;
        tmp[2] = '\0';
                        
        code = ((tmp[0] | 0x80) << 8) + (tmp[1] | 0x80);
         
        return(code);
}

unsigned short
sj3_sjis2jis(code)
unsigned short code;
{
        unsigned char tmp[3];
         
        tmp[0] = (code >> 8) & 0xff;
        tmp[1] = code & 0xff;
        tmp[2] = '\0';

        sj_sjis2jis((unsigned char *) tmp);

        code = (tmp[0] << 8) + tmp[1];

        return(code);
}

unsigned short
sj3_euc2sjis(code)
unsigned short code;
{
        unsigned char tmp[3];  

        tmp[0] = (code >> 8) & 0xff;
        tmp[1] = code & 0xff;
        tmp[2] = '\0';

        sj_euc2sjis((unsigned char *) tmp);
 
        code = (tmp[0] << 8) + tmp[1];

        return(code);
}

unsigned short
sj3_sjis2euc(code)
unsigned short code;
{
        unsigned char tmp[3];

        tmp[0] = (code >> 8) & 0xff;
        tmp[1] = code & 0xff;
        tmp[2] = '\0';

        sj_sjis2euc((unsigned char *) tmp);

        code = (tmp[0] << 8) + tmp[1];

        return(code);
}       
