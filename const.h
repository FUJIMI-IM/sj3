/*-
 * SPDX-License-Identifier: MIT-open-group
 *
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
 */

#ifndef SJ3_CONST_H_
#define SJ3_CONST_H_

#include "Paths.h"
#include "sj_euc.h"

#define	HostNameLen		128			
#define	UserNameLen		128
#define	ProgNameLen		128
#define	PathNameLen		1024			
#define	PassWordLen		16

#define	YomiBufSize		1024
#define	KanjiBufSize		2048

#define	MinIdxLen		512
#define	MinSegLen		512
#define	MinSegNum		1
#define	MinStyNum		512
#define	MaxStyNum		0x7fff
#define	MinClStep		1
#define	MaxClStep		256
#define	MinClLen		512
#define	MaxClLen		0x7fff

#define	DebugOutFile		ErrorOutFile
#define	DebugLevel		0
#define	ForkFlag		1
#define	MaxClientNum		512
#define	LogOutFile		ErrorOutFile
#define	PortName		"sj3"
#define	PortNumber		3086
#define	DirectryMode		0755
#define	DictFileMode		0644
#define	MODE_MASK		0777

#define	DefIdxLen		2048
#define	DefSegLen		2048
#define	DefSegNum		256
#define	DefStyNum		2048
#define	DefClStep		1
#define	DefClLen		2048

#define	NotLocked		-1

#define	ERROR			-1

#ifndef	FALSE
#	define	TRUE		-1
#	define	FALSE		(!TRUE)
#endif

#define	MATCH		0	
#define	OVER		1	
#define	PARTLY		2	
#define	CONT		3	
#define	UNDER		4	

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

#define EUC_a1          (unsigned char)0xa1
#define EUC_a2          (unsigned char)0xa2
#define EUC_a3          (unsigned char)0xa3
#define EUC_a4          (unsigned char)0xa4
#define EUC_a5          (unsigned char)0xa5

#define EUC_START       (unsigned char)0xa1
#define EUC_END         (unsigned char)0xfe
#define EUC_END_KIGOU   (unsigned char)0xae
#define EUC_0           (unsigned char)0xb0
#define EUC_END_0       (unsigned char)0xb9
#define EUC_A           (unsigned char)0xc1
#define EUC_END_A       (unsigned char)0xda
#define EUC_a           (unsigned char)0xe1
#define EUC_END_a       (unsigned char)0xfa
#define EUC_END_HIRA    (unsigned char)0xf3
#define EUC_VU          (unsigned char)0xf4
#define EUC_XKA         (unsigned char)0xf5
#define EUC_XKE         (unsigned char)0xf6

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

#define	MainSegmentLength	2048 	
#define	MainIndexLength		2048		
#define	MainIndexNumber		512		

#define	MaxYomiLength		32	
#define	MaxKanjiLength		32	
#define	MaxHinsiNumber		16	
#define	MaxAtrNumber		16	
#define	MaxJosiNumber		16	

#define	MaxKnjAskNumber		16	
#define	MaxYomiAskNumber	16	

#define	DouonBlkSizeNumber	3	

#define	DouonBlkTerm		0xff	
#define	HinsiBlkTerm		0xff	

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
#define	AtrMsKanjiOfs		0xc0c0	

#define	MaxHindoNumber		2000	
#define	MaxOffsetNumber		1000	

#define	IllegalFormat		"\245\325\245\251\241\274\245\336\245\303\245\310\244\254\260\333\276\357\244\307\244\271"
#define	TooLongYomi		"\306\311\244\337\312\270\273\372\316\363\244\254\304\271\244\271\244\256\244\336\244\271"
#define	TooLongKanji		"\264\301\273\372\312\270\273\372\316\363\244\254\304\271\244\271\244\256\244\336\244\271"
#define	TooLongHinsi		"\311\312\273\354\312\270\273\372\316\363\244\254\304\271\244\271\244\256\244\336\244\271"
#define	TooLongGroup		"\245\260\245\353\241\274\245\327\314\276\244\254\260\333\276\357\244\307\244\271"
#define	TooLongJosi		"\275\365\273\354\312\270\273\372\316\363\244\254\260\333\276\357\244\307\244\271"
#define	BadHinsi		"\305\320\317\277\244\265\244\354\244\306\244\244\244\312\244\244\311\312\273\354\244\307\244\271"
#define	BadGroup		"\305\320\317\277\244\265\244\354\244\306\244\244\244\312\244\244\245\260\245\353\241\274\245\327\244\307\244\271"
#define	BadJosi			"\305\320\317\277\244\265\244\354\244\306\244\244\244\312\244\244\275\365\273\354\244\307\244\271"
#define	NoYomiString		"\306\311\244\337\312\270\273\372\316\363\244\254\274\350\306\300\244\307\244\255\244\336\244\273\244\363"
#define	NoKanjiString		"\264\301\273\372\312\270\273\372\316\363\244\254\274\350\306\300\244\307\244\255\244\336\244\273\244\363"
#define	TooManyAtr		"\302\260\300\255\244\316\277\364\244\254\302\277\244\271\244\256\244\336\244\271"
#define	TooManyHinsi		"\311\312\273\354\244\316\277\364\244\254\302\277\244\271\244\256\244\336\244\271"
#define	TooManyJosi		"\275\365\273\354\244\316\277\364\244\254\302\277\244\271\244\256\244\336\244\271"
#define	NoDataInMain		"\245\341\245\244\245\363\274\255\275\361\244\313\302\270\272\337\244\267\244\336\244\273\244\363"
#define	TooManyTarget		"\311\312\273\354\244\362\243\261\244\304\273\330\304\352\244\267\244\306\244\257\244\300\244\265\244\244"

#define	HinsiTankan	cnvhinsi("\303\261\264\301")

#define	DIVLIST		1
#define	ASSYUKUREC	2
#define	STRING		3
#define	HINDOREC	4
#define	OFFSETREC	5
#define	KANJIREC	6
#define	HINSIREC	7
#define	DOUONREC	8

#endif /* SJ3_CONST_H_ */
