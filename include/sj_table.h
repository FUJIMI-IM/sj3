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
 * $SonyRCSfile: sj_table.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:21 $
 */





#ifndef	_SJ_TABLE

#define	_SJ_TABLE


#define	isdpnd(c)	(Chrtbl[c] & DEPEND)


#define	CnjAskLen(p)	(((unsigned int) *(p) & 0x70) >> 4)


#define	CnjNkrLen(p)	(*(p) & 0x0f)


#define	CnjRight(p)	*(p + CnjNkrLen(p) + 1)


#define	CnjRecLen(p)	(CnjNkrLen(p) + 2)


#define	FzkAskLen(p)	((unsigned int) *(p) >> 5)


#define	FzkNkrLen(p)	(*(p) & 0x07)


#define	FzkStrKa(p)	(((unsigned int) *(p) >> 3) & 1)


#define	FzkHasCode(p)	(*(p) & 0x10)


#define	FzkLeft(p)	(*((p) + nlen + 1))


#define	FzkRight(p)	(*((p) + nlen + 2))


#define	SttYomiLen(p)	(*(p) & 0x0f)


#define	SttHiraKnj(p)	(*(p) & 0x80)


#define	StbKnjLen(p)	(*(p) & 0x07)


#define	StbYomiLen(p)	(*(p) >> 4)


#define	StbYomiTop(p)	((p) + 2)


#define	StbBakeru(p)	(*(p) & 0x08)


#define	StbHinsi(p)	(*((p) + 1))

extern	Uchar	charsize[], chrtbl[], *conjadr[], connadr[];
extern  Uchar   *fzkadr[], kigou[], kurai1_tbl[], kurai2_tbl[];
extern	Uchar	num1tbl[], num2tbl[], num3tbl[], num4tbl[];
extern	Uchar	num5tbl[], num6tbl[], *rigtadr[], scncttbl[2][24];
extern	Uchar	*settou_ptr[], *stbadr[], sttpri[5][31];
extern  Uchar   suuji_tbl[], taipri[7][45], termtbl[];
extern	Ushort	*selsjadrs[];
extern	UCPFunc	getkan_func[], makekan_func[];
extern	VFunc	cvtnum_func[];
extern  IFunc	hiraknj_func[], setjrec_func[], srchg_func[];
#define	Charsize	charsize
#define	Chrtbl		chrtbl
#define	Conjadr(i)	conjadr[i]
#define	Connadr		connadr
#define	Cvtnum_func	cvtnum_func
#define	Fzkadr(i)	fzkadr[i]
#define	Getkan_func	getkan_func
#define	Hiraknj_func	hiraknj_func
#define	Kigou		kigou
#define	Kurai1_tbl	kurai1_tbl
#define	Kurai2_tbl	kurai2_tbl
#define	Makekan_func	makekan_func
#define	Num1tbl		num1tbl
#define	Num2tbl		num2tbl
#define	Num3tbl		num3tbl
#define	Num4tbl		num4tbl
#define	Num5tbl		num5tbl
#define	Num6tbl		num6tbl
#define	Rigtadr(i)	rigtadr[i]
#define	Scncttbl(i, j)	scncttbl[i][j]
#define	Selsjadrs(i)	selsjadrs[i]
#define	Setjrec_func	setjrec_func
#define	Settou_ptr(i)	settou_ptr[i]
#define	Srchg_func	srchg_func
#define	Stbadr(i)	stbadr[i]
#define	Sttpri(i, j)	sttpri[i][j]
#define	Suuji_tbl	suuji_tbl
#define	Taipri(i, j)	taipri[i][j]
#define	Termtbl		termtbl

#endif
