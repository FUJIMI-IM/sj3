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
 * $SonyRCSfile: sj_suuji.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:19 $
 */





#ifndef	_SJ_SUUJI

#define	_SJ_SUUJI



#define	SjTblRecLen	6			
#define	SjTblRecNum	34			
#define	SjTblYPos(p)	(p)			
#define	SjTblYLen	3			
#define	SjTblModeP(p)	(p + 3)			
#define	SjTblNumber(md)	((md) & 0x0f)		
#define	SjTblNArabiaF	0x80			
#define	SjTblNYomiF	0x40			
#define	SjTblNNarabiF	0x20			
#define	SjTblNNanSuuF	0x10			
#define	SjTblNCont(md)	(md & SjTblNContF)	
#define	SjTblCnctP(p)	(p + 4)			



#define	Kr1TblRecLen	5			
#define	Kr1TblRecNum	10			
#define	Kr1TblYPos(p)	(p)			
#define	Kr1TblYLen	3			
#define	Kr1TblModeP(p)	(p + 3)			
#define	Kr1TblTopF	0x80			
#define	Kr1TblTop(md)	(md & Kr1TblTopF)	
#define	Kr1TblKeta(md)	(((md) >> 4) & 7)	
#define	Kr1TblCnct(md)	((md) & 0x0f)		
#define	Kr1TblCnctP(p)	(p + 3)			



#define	Kr2TblRecLen	4			
#define	Kr2TblRecNum	3			
#define	Kr2TblYPos(p)	(p)			
#define	Kr2TblYLen	3			
#define	Kr2TblModeP(p)	(p + 3)			
#define	Kr2TblCnct(md)	((md) & 0x0f)		
#define	Kr2TblKeta(md)	(((md) >> 4) & 0x0f)	

#define	_Num0		0x01
#define	_Num1		0x02
#define	_Num2		0x03
#define	_Num3		0x04
#define	_Num4		0x05
#define	_Num5		0x06
#define	_Num6		0x07
#define	_Num7		0x08
#define	_Num8		0x09
#define	_Num9		0x0a
#define	_Num1S		0x0b
#define	_NumS		0x0c
#define	_NumN		0x0d

#define	CommaHigh	0xa1		
#define	CommaLow	0xa4		




#define	SelArb		1	
#define	SelArb0		2	
#define	SelArbCma	3	
#define	SelKanSj0	5	
#define	SelKanSj20	8	
#define	SelKanSjYm	9	
#define	SelKanSjYm2	10	
#define	SelLongArb	11	
#define	SelLongArbCma	12	
#define	SelLongSj	13	
#define	SelArbKr	14	
#define	SelKanSjKr	15	
#define	SelNumTerm	0xffff	

#define	SelKanSj	4	
#define	SelKanSj2	7	
#define	SelKanSjCma	6	



#define	SelNum0123	(JFLAG_N01 | JFLAG_N23)
#define SelNum1_23_10_1	(JFLAG_N_1 | JFLAG_N23 | JFLAG_N10 | JFLAG_NS1)
#define	SelNumMust	(JFLAG_NK2 | JFLAG_NK4 | JFLAG_NN0 | JFLAG_NSN)
#define	SelNumALL	(JFLAG_N10 | JFLAG_N_1 | JFLAG_NS1 | JFLAG_N01 \
				| JFLAG_N23 | JFLAG_NK2 | JFLAG_NK4 \
				| JFLAG_NN0 | JFLAG_NSN)



#define	SelNumFunc(c)		((c) & (~SelNumALL))
#define	SelNumCond(c)		((c) & SelNumALL)



#define	Number123456789NS	((1<<_Num1)|(1<<_Num2)|(1<<_Num3)|(1<<_Num4) \
				|(1<<_Num5)|(1<<_Num6)|(1<<_Num7)|(1<<_Num8) \
				|(1<<_Num9)|(1<<_Num1S)\
				|(1<<_NumN)|(1<<_NumS))
#define	Number0		(1<<_Num0)
#define	Number1		((1<<_Num1)|(1<<_Num1S))
#define	NumberS1	(1<<_Num1S)
#define	Number01	(Number0|Number1)
#define	Number23	((1<<_Num2)|(1<<_Num3))

#endif
