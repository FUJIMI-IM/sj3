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
 * $SonyRCSfile: kctype.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:46 $
 */



#if !defined(__sony_news) || (defined(__sony_news) && !defined(SVR4))
#ifndef __KCTYPE
#define __KCTYPE

#define sjis2euc        sj3_sjis2euc
#define jis2sjis        sj3_jis2sjis
#define euctosjis       sj3_euctosjis
#define sjis2jis        sj3_sjis2jis
#define euc2sjis        sj3_euc2sjis
#define jis2euc         sj3_jis2euc
#define sjistoeuc       sj3_sjistoeuc

#ifdef issjis1
#undef issjis1
#endif
#define issjis1(c)   (((0x81 <= (c)) && ((c) <= 0x9f)) || \
                      ((0xe0 <= (c)) && ((c) <= 0xfc)))

#ifdef issjis2
#undef issjis2
#endif
#define issjis2(c)   (((0x40 <= (c)) && ((c) <= 0xfc)) && ((c) != 0x7f))

#ifdef iseuc
#undef iseuc
#endif
#define iseuc(c)     ((0xa1 <= (c)) && ((c) <= 0xfe))

#ifdef iseuckana
#undef iseuckana
#endif
#define iseuckana(c) ((c) == 0x8e)

#ifdef iskana2
#undef iskana2
#endif
#define iskana2(c)   ((0xa1 <= (c)) && ((c) <= 0xdf))

#ifdef jiszen
#undef jiszen
#endif
#define jiszen(c)    (issjis1(((c) >> 8) & 0xff) && issjis2((c) & 0xff))

#ifdef isjjis
#undef isjjis
#endif
#define isjjis(c)    ((0x21 <= (c)) && ((c) <= 0x7e))

#ifdef jstrlen
#undef jstrlen
#endif
#define jstrlen(s)   euclen(s, 65535)

#ifdef iseuc3byte
#undef iseuc3byte
#endif
#define iseuc3byte(c) ((c) == 0x8f)

#ifdef iseuc2byte
#undef iseuc2byte
#endif
#define iseuc2byte(c) (iseuc(c) || ((c) == 0x8e))

#ifdef iskanji
#undef iskanji
#endif
#define iskanji   issjis1
#ifdef iskanji2
#undef iskanji2
#endif
#define iskanji2  issjis2
#ifdef iskana
#undef iskana
#endif
#define iskana    iskana2
#endif
#endif

