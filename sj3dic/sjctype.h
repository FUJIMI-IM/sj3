/*
 * Copyright (c) 1991-1994  Sony Corporation
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
 * $SonyRCSfile: sjctype.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:39 $
 */



#ifndef __sony_news
#ifndef __SJCTYPE
#define __SJCTYPE

#include "sj_sysvdef.h"
#define sjis2euc        sj3_sjis2euc
#define jis2sjis        sj3_jis2sjis
#define euctosjis       sj3_euctosjis
#define sjis2jis        sj3_sjis2jis
#define euc2sjis        sj3_euc2sjis
#define jis2euc         sj3_jis2euc
#define sjistoeuc       sj3_sjistoeuc

#define issjis1(c)   (((0x81 <= ((unsigned char)(c))) && (((unsigned char)(c)) <= 0x9f)) || \
                      ((0xe0 <= ((unsigned char)(c))) && (((unsigned char)(c)) <= 0xfc)))

#define issjis2(c)   (((0x40 <= ((unsigned char)(c))) && (((unsigned char)(c)) <= 0xfc)) && (((unsigned char)(c)) != 0x7f))

#define iseuc(c)     ((0xa1 <= ((unsigned char)(c))) && (((unsigned char)(c)) <= 0xfe))

#define iseuckana(c) (((unsigned char)(c)) == 0x8e)

#define iskana2(c)   ((0xa1 <= ((unsigned char)(c))) && (((unsigned char)(c)) <= 0xdf))

#define jiszen(c)    (issjis1(((c) >> 8) & 0xff) && issjis2((c) & 0xff))

#define isjjis(c)    ((0x21 <= (c)) && ((c) <= 0x7e))

#define jstrlen(s)   euclen(s, 65535)

#define iseuc3byte(c) ((c) == 0x8f)

#define iseuc2byte(c) (iseuc(c) || ((c) == 0x8e))

#define iskanji   issjis1
#define iskanji2  issjis2
#define iskana    iskana2
#endif
#else
#ifndef SVR4
#define euctosjis       sj3_euctosjis
#define sjistoeuc       sj3_sjistoeuc
#endif 
#endif 

