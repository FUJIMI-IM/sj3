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
 * $SonyRCSfile: sj_study.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:18 $
 */





#ifndef	_SJ_STUDY

#define	_SJ_STUDY

#define	StudyExist()	(stdy_base)

#define	StudyCount	(stdy_base -> stdycnt)
#define	StudyMax	(stdy_base -> stdymax)
#define	StudyDict	(stdy_base -> stdydic)

#define	ClStudyIdx	(stdy_base -> clstdyidx)
#define	ClStudyStep	(stdy_base -> clstdystep)
#define	ClStudyLen 	(stdy_base -> clstdylen)
#define	ClStudyDict	(stdy_base -> clstdydic)

#define	StudyTail	(StudyDict + StudyMax)
#define	StudyRecSize	(sizeof(STDYIN))

#define	ClStudyTail	(ClStudyDict + ClStudyLen)



#define	StudyNormEnd	0
#define	StudyNoDict	1
#define	StudyFullArea	2
#define	StudyNotStudy	3
#define	StudyError	4




#define	CLSTDY_TMPLEN	256
#define	ClstudyNormEnd	0			
#define	ClstudyNoDict	1			
#define	ClstudyParaErr	2			
#define	ClstudyYomiErr	3			
#define	ClstudyNoArea	4			

#define	CL_YLEN		0
#define	CL_GRAM2	1
#define	CL_Y1LEN	2
#define	CL_NUMH		3
#define	CL_NUML		4
#define	CL_YDATA	5
#define	CL_HDLEN	5



#define	iseocl(p)	( !ClYomiLen(p) || (Uchar *)(p) >= ClStudyTail )



#define	ClGramCode(p)	*((p) + CL_GRAM2)



#define	ClYomiLen(p)	*((p) + CL_YLEN)



#define	ClYomi1Len(p)	*((p) + CL_Y1LEN)



#define	ClHighNum(p)	*(p + CL_NUMH)
#define	ClLowNum(p)	*(p + CL_NUML)
#define	ClGetNum(p)	(ClHighNum(p) * 0x100 + ClLowNum(p))
#define	ClSetNum(p, n)	(ClHighNum(p)=(Uchar)((n) >> 8), ClLowNum(p)=(Uchar)(n))



#define	ClYomiPos(p)	((p) + CL_YDATA)



#define	ClBlkLen(p)	(ClYomiLen(p) + CL_HDLEN)



#define	ClNextTag(p)	((p) + ClBlkLen(p))

#endif
