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
 * $SonyDate: 1994/06/03 08:00:24 $
 */





#ifndef	_CONST_
#define	_CONST_

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

#define	FALSE			0
#define	TRUE			(!FALSE)

#define	DIVLIST		1
#define	ASSYUKUREC	2
#define	STRING		3
#define	HINDOREC	4
#define	OFFSETREC	5
#define	KANJIREC	6
#define	HINSIREC	7
#define	DOUONREC	8

#endif
