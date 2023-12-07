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
 * $SonyRCSfile: stbtbl.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:34 $
 */




#include "sj_rename.h"
#include "sj_typedef.h"
#include "sj_yomi.h"
#include "sj_hinsi.h"
static	Uchar	name_stb[] = {  	
	0x22, MEISI_1,	_KU, _NN,
			0xb7, 0xaf,			
	0x22, MEISI_1,	_SA, _MA,
			0xcd, 0xcd,			
	0x24, MEISI_1,	_SA, _NN,
			0xa4, 0xb5, 0xa4, 0xf3,		
	0x12, MEISI_1,	_SI,
			0xbb, 0xe1,			
	0x36, MEISI_1,	_TI, _XYA, _NN,
			0xa4, 0xc1, 0xa4, 0xe3, 0xa4, 0xf3, 
	0x22, MEISI_1,	_DO, _NO,
			0xc5, 0xc2,			
	0x00
};

static	Uchar	timei_stb[] = {  	
	0x22, MEISI_1,	_E, _KI,
		 	0xb1, 0xd8,			
	0x44, MEISI_1,	_E, _KI, _HA, _TU,
		 	0xb1, 0xd8, 0xc8, 0xaf,		
	0x44, MEISI_1,	_E, _KI, _YU, _KI,
		 	0xb1, 0xd8, 0xb9, 0xd4,		
	0x22, MEISI_1,	_GU, _NN,
	 		0xb7, 0xb4,			
	0x12, MEISI_1,	_SI,
			0xbb, 0xd4,			
	0x32, MEISI_1,	_TI, _XYO, _U,
			0xc4, 0xae,			
	0x22, MEISI_1,	_HA, _TU,
			0xc8, 0xaf,			
	0x22, MEISI_1,	_MA, _TI,
	 		0xc4, 0xae,			
	0x22, MEISI_1,	_MU, _RA,
			0xc2, 0xbc,			
	0x00
};

static	Uchar	meisi_stb[] = {		
	0x44, MEISI_6,	_A, _TU, _KA, _I,
			0xb0, 0xb7, 0xa4, 0xa4,		
	0x1a, MEISI_6,	_KA,
			0xb2, 0xbd,			
	0x22, MEISI_8,	_TE, _KI,
			0xc5, 0xaa,			
	0x56, FUKUSI_1, _NI, _TO, _MO, _NA, _I,	
			0xa4, 0xcb, 0xc8, 0xbc, 0xa4, 0xa4,	
	0x56, FUKUSI_1, _NI, _TO, _MO, _NA, _U,	
			0xa4, 0xcb, 0xc8, 0xbc, 0xa4, 0xa6,	
	0x22, MEISI_8,	_HU, _U,
			0xc9, 0xf7,			
	0x00
};

static	Uchar	sahen_stb[] = {		
	0x44, MEISI_6,	_A, _TU, _KA, _I,
			0xb0, 0xb7, 0xa4, 0xa4,		
	0x1a, MEISI_6,	_KA,
			0xb2, 0xbd,			
	0x12, MEISI_11,	_GO,
			0xb8, 0xe5,			
	0x22, MEISI_11, _GO, _TO,
			0xcb, 0xe8,				
	0x32, MEISI_11,	_ZI, _XYO, _U,
			0xbe, 0xe5,			
	0x32, MEISI_11,	_TI, _XYU, _U,
			0xc3, 0xe6,			
	0x22, MEISI_8,	_TE, _KI,
			0xc5, 0xaa,			
	0x56, FUKUSI_1, _NI, _TO, _MO, _NA, _I,	
			0xa4, 0xcb, 0xc8, 0xbc, 0xa4, 0xa4,	
	0x56, FUKUSI_1, _NI, _TO, _MO, _NA, _U,	
			0xa4, 0xcb, 0xc8, 0xbc, 0xa4, 0xa6,	
	0x22, MEISI_8,	_HU, _U,
			0xc9, 0xf7,			
	0x22, MEISI_11,	_MA, _E,
		 	0xc1, 0xb0,			
	0x00
};

static	Uchar	human_stb[] = {  	
	0x24, MEISI_1,	_SA, _NN,
			0xa4, 0xb5, 0xa4, 0xf3,		
	0x00
};

static	Uchar	ippan_stb[] = {		
	0x56, FUKUSI_1, _NI, _TO, _MO, _NA, _I,	
			0xa4, 0xcb, 0xc8, 0xbc, 0xa4, 0xa4,	
	0x56, FUKUSI_1, _NI, _TO, _MO, _NA, _U,	
			0xa4, 0xcb, 0xc8, 0xbc, 0xa4, 0xa6,	
	0x00
};

Uchar	*stbadr[] = {
	0,
	ippan_stb, ippan_stb, ippan_stb, meisi_stb, sahen_stb,
	sahen_stb, sahen_stb, meisi_stb, meisi_stb, meisi_stb,
	ippan_stb, ippan_stb, human_stb, ippan_stb, ippan_stb,
	ippan_stb, ippan_stb, ippan_stb, 	 0, 	    0,
	name_stb,  name_stb,  0, 	 timei_stb,
};
