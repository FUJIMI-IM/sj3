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
 * $SonyRCSfile: sj_dict.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:03 $
 */





#ifndef	_SJ_DICT

#define	_SJ_DICT



#define	DIC_MAIN		0
#define	DIC_USER		1




#define	DicSegBase		0	
#define	MaxDouonBlk		0xfff	
#define	DouonBlkSizeNumber	3	
#define	DicSegTerm		0xff	
#define	DouonBlkTerm		0xff	
#define	HinsiBlkTerm		0xff	
#define	MaxSegmentNumber	0xffff	



#define NormalKanjiMask         0x7f
#define SS3Kanji                0x0080
#define	ZenHiraAssyuku		0x80	
#define	OffsetAssyuku		0x90	
#define	ZenKataAssyuku		0xa0	
#define	AiAttribute		0xb0	
#define	KanjiAssyuku		0xc0	
#define	LeadingHankaku		0xd0	
#define	KanjiStrEnd		0x00	
#define IsAssyuku               0x80
#define AssukuUse2byte          0x90
#define KnjAssyukuMask          0x0f    
#define KanjiCodeMask           0x0f    
#define KanjiModeMask           0xf0
#define AtrMsKanjiOfs           0xc0c0 

#define	MaxKnjAskNumber		16	
#define	MaxYomiAskNumber	16

#define	MaxWdYomiLen		31	
#define	MaxWdKanjiLen		64	

#define	AssyukuYomiTop		0x80
#define	NokoriYomiTop		0x40




#define getsize(p)	(((Int)(*(p) & 0x0f) * 0x100) + *((p) + 1))



#define	getntag(p)	((*(p) == DicSegTerm) ? (p) : (p) + getsize(p))



#define	getplen(p)	(((*(p) & AssyukuYomiTop)?0x10:0) + (*((p)+2) >> 4))



#define	getnlen(p)	(((*(p) & NokoriYomiTop)?0x10:0) + (*((p)+2) & 0x0f))



#define	segtop()	(dicbuf + *dicbuf + 1)



#define	segend(p)	(*(p) == DicSegTerm || (p) >= dicbuf + curdict->seglen)

#endif

