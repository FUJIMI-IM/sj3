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
 * $SonyRCSfile: kana.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:45 $
 */




#define ZKATA1	0xa5	
#define ZHIRA1	0xa4	
#define ZKATAF2	0xa1	
#define ZKATAE2	0xf6	
#define ZHIRAF2	0xa1	
#define ZHIRAE2	0xf3	
#define ZKATAMU	0xe0	
#define ZHIRAMU	0xe0	

#define ZKATAn	(ZKATAE2-ZKATAF2+1)	
				


#define ZEN_SPACE		0xa1a1
#define ZEN_KTEN		0xa1a3
#define ZEN_FKAKKO		0xa1d6
#define ZEN_EKAKKO		0xa1d7
#define ZEN_TTEN		0xa1a2
#define ZEN_CTEN		0xa1a6
#define ZEN_CHOUON		0xa1bc
#define ZEN_DTEN		0xa1ab
#define ZEN_HDTEN		0xa1ac
#define ZEN_VU			0xa5f4

#define KANA_KTEN		0xa1
#define KANA_FKAKKO		0xa2
#define KANA_EKAKKO		0xa3
#define KANA_TTEN		0xa4
#define KANA_CTEN		0xa5
#define KANA_CHOUON		0xb0
#define KANA_DTEN		0xde
#define KANA_HDTEN		0xdf

#define KANA_XA			0xa7
#define KANA_XYA		0xac
#define KANA_XTSU		0xaf

#define KANA_A			0xb1
#define KANA_KA			0xb6
#define KANA_SA			0xbb
#define KANA_TA			0xc0
#define KANA_NA			0xc5
#define KANA_HA			0xca
#define KANA_MA			0xcf
#define KANA_YA			0xd4
#define KANA_RA			0xd7
#define KANA_WA			0xdc
#define KANA_WO			0xa6

#define KANA_GA			(KANA_KA << 8) + KANA_DTEN
#define KANA_ZA			(KANA_SA << 8) + KANA_DTEN
#define KANA_DA			(KANA_TA << 8) + KANA_DTEN
#define KANA_BA			(KANA_HA << 8) + KANA_DTEN
#define KANA_PA			(KANA_HA << 8) + KANA_HDTEN
