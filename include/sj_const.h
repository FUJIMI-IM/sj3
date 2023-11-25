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
 * $SonyRCSfile: sj_const.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:01 $
 */





#ifndef	_SJ_CONST

#define	_SJ_CONST
#include "sj_euc.h"


#define	MATCH		0	
#define	OVER		1	
#define	PARTLY		2	
#define	CONT		3	
#define	UNDER		4	




#define	FALSE		0
#define	TRUE		-1




#define	AD_Done		0x0000	
#define	AD_NotUserDict	0x0001	
#define	AD_BadYomi	0x0002	
#define	AD_BadKanji	0x0004	
#define	AD_BadHinsi	0x0008	

#define	AD_ArExist	0x0010	
#define	AD_OvflwDouBlk	0x0020	
#define	AD_OvflwDouNum	0x0040	
#define	AD_OvflwUsrDic	0x0080	
#define	AD_OvflwIndex	0x0100	

#define	AD_NoMidasi	0x0010	
#define	AD_NoHinsi	0x0020	
#define	AD_NoKanji	0x0040	




#define	C_WAKACHI	0x00		
#define	C_MINASI	0x01		
#define	C_BUNSETU	0x02		
#define	C_DICT		0x03		
#define	C_N_ARABIA	0x04		
#define	C_N_ARACMA	0x05		
#define	C_N_KAZU	0x06		
#define	C_N_SUUJI	0x07		
#define	C_N_NANSUU	0x08		
#define	C_N_KANKURA	0x09		
#define	C_N_ARAKURA	0x0a		
#define	C_N_KAZULONG	0x0b		
#define	C_N_KAZULCMA	0x0c		
#define	C_N_SUUJILONG	0x0d		




#define	JFLAG_N10	(1 << 15)	
#define	JFLAG_N_1	(1 << 14)	
#define	JFLAG_NS1	(1 << 13)	
#define	JFLAG_N01	(1 << 12)	
#define	JFLAG_N23	(1 << 11)	
#define	JFLAG_NK2	(1 << 10)	
#define	JFLAG_NK4	(1 << 9)	
#define	JFLAG_NN0	(1 << 8)	
#define	JFLAG_NSN	(1 << 7)	
#define	JFLAG_KA	(1 << 0)	



#define	DO_CLSTUDY	0x01	
#define	DO_IKKATU	0x02	




#define	K_DOUSHI	1	
#define	K_YOUGEN	2	
#define	K_TAIGEN	3	
#define	K_FUKUSI	4	
#define	K_RENTAISI	5	
#define	K_SONOTA	6	






#define EUC_a1          (Uchar)0xa1
#define EUC_a2          (Uchar)0xa2
#define EUC_a3          (Uchar)0xa3
#define EUC_a4          (Uchar)0xa4
#define EUC_a5          (Uchar)0xa5


#define EUC_START       (Uchar)0xa1
#define EUC_END         (Uchar)0xfe
#define EUC_END_KIGOU   (Uchar)0xae
#define EUC_0           (Uchar)0xb0
#define EUC_END_0       (Uchar)0xb9
#define EUC_A           (Uchar)0xc1
#define EUC_END_A       (Uchar)0xda
#define EUC_a           (Uchar)0xe1
#define EUC_END_a       (Uchar)0xfa
#define EUC_END_HIRA    (Uchar)0xf3
#define EUC_VU          (Uchar)0xf4
#define EUC_XKA         (Uchar)0xf5
#define EUC_XKE         (Uchar)0xf6


#define	isknj1(c)	(((c) == SS2) || (((c) & 0x80) && ((c) != SS3)))




#define	SHU		0x01		
#define	T_KANOU		0x80		
#define	T_SHUUJO	0x40		



#define	DICTOP  	0x01	
#define	FZKTOP		0x02	
#define	DEPEND		0x04	
#define	NUMBER		0x08	
#define	FZK_KGU  	0x10	
#define	TAI_KGU		0x20	
#define	BEFORNUM	0x40	
#define	MIDNUM		0x80	
#define	NOTNRM		0xFE	




#define	CNJEND		0xff	
#define	TYOUONFLG	0x80	




#define	MAXFREC		32	
#define	MAXFLVL		10	
#define	FZKEND		0xff	



#define	SETTOU_O	1	
#define	SETTOU_GO	2	
#define	SETTOU_DAI	3	
#define	SETTOU_KIGOU	4	




#define	NumKetaLength	16		
#define	NumWordBuf	((NumKetaLength+3)/4)	


#endif
