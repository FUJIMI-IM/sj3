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
 * $SonyRCSfile: conjunc.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:33 $
 */


#include "sj_rename.h"
#include "sj_typedef.h"
#include "sj_yomi.h"
#include "sj_right.h"

static	Uchar	sahencnj[] = {
		0x00,				R_SGOKAN,	
		0x01,	_SA,			R_MIZEN1,	
		0x01,	_SI,			R_SI1,		
		0x11,	_RO,			R_MEIREI1,	
		0x02,	_SU, _RU,		R_SHUUSI,	
		0x25,   _KA, _SI, _NA, _I, _KA, R_KA1,	
		0x11,	_RE,			R_KATEI2,	
		0x01,	_SE,			R_MIZEN2,	
		0x12,	_NA, _I,		R_NAI,		
		0x31,	_DE,			R_NAIDE,	
		0x22,	_KA, _XTU,		R_ONBIN1,	
		0x32,	_RO, _U,		R_KAROU,	
		0x21,	_KU,			R_NAKU,		
		0x22,	_KE, _RE,		R_KATEI3,	
		0x12,	_RA, _RE,		R_RARE,		
		0x31,	_YO,			R_MEIREI2,	
		0x31,	_RU,			R_SHUUSI,	
		0x31,	_RE,			R_KATEI2,	
		0x31,	_RO,			R_MEIREI1,	
		0x11,	_YO,			R_MEIREI2,	
		0xff
	};

static	Uchar	zahencnj[] = {
		0x00,				R_CUT,		
		0x01,	_ZI,			R_JI,		
		0x11,	_YO,			R_MEIREI2,	
		0x11,	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KATEI2,	
		0x11,	_RO,			R_MEIREI1,	
		0x02,	_ZU, _RU,		R_SHUUSI,	
		0x11,	_RE,			R_KATEI2,	
		0x01,	_ZE,			R_MIZEN3,	
		0x11,	_YO,			R_MEIREI2,	
		0xff
	};

static	Uchar	ka5cnj1[] = {
		0x01,	_I,			R_ONBIN2,	
		0x01,	_KA,			R_MIZEN5,	
		0x01,	_KI,			R_RENYOU5,	
		0x01,	_KU,			R_SHUUSI,	
		0x01,	_KE,			R_KAMEI,	
		0x02,	_KO, _U,		R_AROU,		
		0xff
	};

static	Uchar	ka5cnj3[] = {
		0x01,	_I,			R_ONBIN2,	
		0x01,	_KA,			R_MIZEN5,	
		0x01,	_KI,			R_RENYOU7,	
		0x01,	_KU,			R_SHUUSI,	
		0x01,	_KE,			R_KAMEI,	
		0x02,	_KO, _U,		R_AROU,		
		0xff
	};

static	Uchar	ka5cnj5[] = {
		0x01,	_I,			R_ONBIN2,	
		0x01,	_KA,			R_MIZEN5,	
		0x11,	_SA,			R_MIZEN5,	
		0x11,	_SI,			R_SA5RENYOU3,	
		0x11,	_SU,			R_SHUUSI,	
		0x12,	_SO, _U,		R_AROU,		
		0x01,	_KI,			R_RENYOU5,	
		0x01,	_KU,			R_SHUUSI,	
		0x01,	_KE,			R_EXP,		
		0x11,	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KATEI2,	
		0x02,	_KO, _U,		R_AROU,		
		0xff
	};

static	Uchar	ka5cnj7[] = {
		0x01,	_I,			R_ONBIN2,	
		0x01,	_KA,			R_MIZEN5,	
		0x11,	_SA,			R_MIZEN5,	
		0x11,	_SI,			R_SA5RENYOU3,	
		0x11,	_SU,			R_SHUUSI,	
		0x12,	_SO, _U,		R_AROU,		
		0x01,	_KI,			R_RENYOU7,	
		0x01,	_KU,			R_SHUUSI,	
		0x01,	_KE,			R_EXP,		
		0x11,	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KATEI2,	
		0x02,	_KO, _U,		R_AROU,		
		0xff
	};

static	Uchar	ga5cnj1[] = {
		0x01,	_I,			R_ONBIN3,	
		0x01,	_GA,			R_MIZEN5,	
		0x01,	_GI,			R_RENYOU5,	
		0x01,	_GU,			R_SHUUSI,	
		0x01,	_GE,			R_KAMEI,	
		0x02,	_GO, _U,		R_AROU,		
		0xff
	};

static	Uchar	ga5cnj3[] = {
		0x01,	_I,			R_ONBIN3,	
		0x01,	_GA,			R_MIZEN5,	
		0x01,	_GI,			R_RENYOU7,	
		0x01,	_GU,			R_SHUUSI,	
		0x01,	_GE,			R_KAMEI,	
		0x02,	_GO, _U,		R_AROU,		
		0xff
	};

static	Uchar	ga5cnj5[] = {
		0x01,	_I,			R_ONBIN3,	
		0x01,	_GA,			R_MIZEN5,	
		0x11,	_SA,			R_MIZEN5,	
		0x11,	_SI,			R_SA5RENYOU3,	
		0x11,	_SU,			R_SHUUSI,	
		0x12,	_SO, _U,		R_AROU,		
		0x01,	_GI,			R_RENYOU5,	
		0x01,	_GU,			R_SHUUSI,	
		0x01,	_GE,			R_EXP,		
		0x11,	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KATEI2,	
		0x02,	_GO, _U,		R_AROU,		
		0xff
	};

static	Uchar	ga5cnj7[] = {
		0x01,	_I,			R_ONBIN3,	
		0x01,	_GA,			R_MIZEN5,	
		0x11,	_SA,			R_MIZEN5,	
		0x11,	_SI,			R_SA5RENYOU3,	
		0x11,	_SU,			R_SHUUSI,	
		0x12,	_SO, _U,		R_AROU,		
		0x01,	_GI,			R_RENYOU7,	
		0x01,	_GU,			R_SHUUSI,	
		0x01,	_GE,			R_EXP,		
		0x11,	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KATEI2,	
		0x02,	_GO, _U,		R_AROU,		
		0xff
	};

static	Uchar	sa5cnj1[] = {
		0x01,	_SA,			R_MIZEN5,	
		0x01,	_SI,			R_SA5RENYOU1,	
		0x01,	_SU,			R_SHUUSI,	
		0x01,	_SE,			R_KAMEI,	
		0x02,	_SO, _U,		R_AROU,		
		0xff
	};

static	Uchar	sa5cnj3[] = {
		0x01,	_SA,			R_MIZEN5,	
		0x01,	_SI,			R_SA5RENYOU3,	
		0x01,	_SU,			R_SHUUSI,	
		0x01,	_SE,			R_KAMEI,	
		0x02,	_SO, _U,		R_AROU,		
		0xff
	};

static	Uchar	sa5cnj5[] = {
		0x01,	_SA,			R_MIZEN5,	
		0x01,	_SI,			R_SA5RENYOU1,	
		0x01,	_SU,			R_SHUUSI,	
		0x01,	_SE,			R_EXP,		
		0x11,	_RU,			R_SHUUSI,	
		0x11, 	_RE,			R_KATEI2,	
		0x02,	_SO, _U,		R_AROU,		
		0xff
	};

static	Uchar	sa5cnj7[] = {
		0x01,	_SA,			R_MIZEN5,	
		0x01,	_SI,			R_SA5RENYOU3,	
		0x01,	_SU,			R_SHUUSI,	
		0x01,	_SE,			R_EXP,		
		0x11,	_RU,			R_SHUUSI,	
		0x11, 	_RE,			R_KATEI2,	
		0x02,	_SO, _U,		R_AROU,		
		0xff
	};

static	Uchar	ta5cnj1[] = {
		0x01,	_TA,			R_MIZEN5,	
		0x01,	_TI,			R_RENYOU5,	
		0x01,	_XTU,			R_ONBIN2,	
		0x01,	_TU,			R_SHUUSI,	
		0x01,	_TE,			R_KAMEI,	
		0x02,	_TO, _U,		R_AROU,		
		0xff
	};

static	Uchar	ta5cnj3[] = {
		0x01,	_TA,			R_MIZEN5,	
		0x01,	_TI,			R_RENYOU7,	
		0x01,	_XTU,			R_ONBIN2,	
		0x01,	_TU,			R_SHUUSI,	
		0x01,	_TE,			R_KAMEI,	
		0x02,	_TO, _U,		R_AROU,		
		0xff
	};

static	Uchar	ta5cnj5[] = {
		0x01,	_TA,			R_MIZEN5,	
		0x11,	_SA,			R_MIZEN5,	
		0x11,	_SI,			R_SA5RENYOU3,	
		0x11,	_SU,			R_SHUUSI,	
		0x12,	_SO, _U,		R_AROU,		
		0x01,	_TI,			R_RENYOU5,	
		0x01,	_XTU,			R_ONBIN2,	
		0x01,	_TU,			R_SHUUSI,	
		0x01,	_TE,			R_EXP,		
		0x11,	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KATEI2,	
		0x02,	_TO, _U,		R_AROU,		
		0xff
	};

static	Uchar	ta5cnj7[] = {
		0x01,	_TA,			R_MIZEN5,	
		0x11,	_SA,			R_MIZEN5,	
		0x11,	_SI,			R_SA5RENYOU3,	
		0x11,	_SU,			R_SHUUSI,	
		0x12,	_SO, _U,		R_AROU,		
		0x01,	_TI,			R_RENYOU7,	
		0x01,	_XTU,			R_ONBIN2,	
		0x01,	_TU,			R_SHUUSI,	
		0x01,	_TE,			R_EXP,		
		0x11,	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KATEI2,	
		0x02,	_TO, _U,		R_AROU,		
		0xff
	};
		

static	Uchar	na5cnj[] = {
		0x01,	_NA,			R_MIZEN5,	
		0x01,	_NI,			R_RENYOU5,	
		0x01,	_NU,			R_SHUUSI,	
		0x01,	_NE,			R_KAMEI,	
		0x02,	_NO, _U,		R_AROU,		
		0x01,	_NN,			R_ONBIN3,	
		0xff
	};

static	Uchar	ba5cnj1[] = {
		0x01,	_BA,			R_MIZEN5,	
		0x01,	_BI,			R_RENYOU5,	
		0x01,	_BU,			R_SHUUSI,	
		0x01,	_BE,			R_KAMEI,	
		0x02,	_BO, _U,		R_AROU,		
		0x01,	_NN,			R_ONBIN3,	
		0xff
	};

static	Uchar	ba5cnj3[] = {
		0x01,	_BA,			R_MIZEN5,	
		0x01,	_BI,			R_RENYOU7,	
		0x01,	_BU,			R_SHUUSI,	
		0x01,	_BE,			R_KAMEI,	
		0x02,	_BO, _U,		R_AROU,		
		0x01,	_NN,			R_ONBIN3,	
		0xff
	};

static	Uchar	ba5cnj5[] = {
		0x01,	_BA,			R_MIZEN5,	
		0x11,	_SA,			R_MIZEN5,	
		0x11,	_SI,			R_SA5RENYOU3,	
		0x11,	_SU,			R_SHUUSI,	
		0x12,	_SO, _U,		R_AROU,		
		0x01,	_BI,			R_RENYOU5,	
		0x01,	_BU,			R_SHUUSI,	
		0x01,	_BE,			R_EXP,		
		0x11,	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KATEI2,	
		0x02,	_BO, _U,		R_AROU,		
		0x01,	_NN,			R_ONBIN3,	
		0xff
	};

static	Uchar	ba5cnj7[] = {
		0x01,	_BA,			R_MIZEN5,	
		0x11,	_SA,			R_MIZEN5,	
		0x11,	_SI,			R_SA5RENYOU3,	
		0x11,	_SU,			R_SHUUSI,	
		0x12,	_SO, _U,		R_AROU,		
		0x01,	_BI,			R_RENYOU7,	
		0x01,	_BU,			R_SHUUSI,	
		0x01,	_BE,			R_EXP,		
		0x11,	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KATEI2,	
		0x02,	_BO, _U,		R_AROU,		
		0x01,	_NN,			R_ONBIN3,	
		0xff
	};

static	Uchar	ma5cnj1[] = {
		0x01,	_MA,			R_MIZEN5,	
		0x01,	_MI,			R_RENYOU5,	
		0x01,	_MU,			R_SHUUSI,	
		0x01,	_ME,			R_KAMEI,	
		0x02,	_MO, _U,		R_AROU,		
		0x01,	_NN,			R_ONBIN3,	
		0xff
	};

static	Uchar	ma5cnj3[] = {
		0x01,	_MA,			R_MIZEN5,	
		0x01,	_MI,			R_RENYOU7,	
		0x01,	_MU,			R_SHUUSI,	
		0x01,	_ME,			R_KAMEI,	
		0x02,	_MO, _U,		R_AROU,		
		0x01,	_NN,			R_ONBIN3,	
		0xff
	};

static	Uchar	ma5cnj5[] = {
		0x01,	_MA,			R_MIZEN5,	
		0x11,	_SA,			R_MIZEN5,	
		0x11,	_SI,			R_SA5RENYOU3,	
		0x11,	_SU,			R_SHUUSI,	
		0x12,	_SO, _U,		R_AROU,		
		0x01,	_MI,			R_RENYOU5,	
		0x01,	_MU,			R_SHUUSI,	
		0x01,	_ME,			R_EXP,		
		0x11,	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KATEI2,	
		0x02,	_MO, _U,		R_AROU,		
		0x01,	_NN,			R_ONBIN3,	
		0xff
	};

static	Uchar	ma5cnj7[] = {
		0x01,	_MA,			R_MIZEN5,	
		0x11,	_SA,			R_MIZEN5,	
		0x11,	_SI,			R_SA5RENYOU3,	
		0x11,	_SU,			R_SHUUSI,	
		0x12,	_SO, _U,		R_AROU,		
		0x01,	_MI,			R_RENYOU7,	
		0x01,	_MU,			R_SHUUSI,	
		0x01,	_ME,			R_EXP,		
		0x11,	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KATEI2,	
		0x02,	_MO, _U,		R_AROU,		
		0x01,	_NN,			R_ONBIN3,	
		0xff
	};

static	Uchar	ra5cnj1[] = {
		0x01,	_XTU,			R_ONBIN2,	
		0x01,	_RA,			R_MIZEN5,	
		0x01,	_RI,			R_RENYOU5,	
		0x01,	_RU,			R_SHUUSI,	
		0x01,	_RE,			R_KAMEI,	
		0x02,	_RO, _U,		R_AROU,		
		0xff
	};

static	Uchar	ra5cnj3[] = {
		0x01,	_XTU,			R_ONBIN2,	
		0x01,	_RA,			R_MIZEN5,	
		0x01,	_RI,			R_RENYOU7,	
		0x01,	_RU,			R_SHUUSI,	
		0x01,	_RE,			R_KAMEI,	
		0x02,	_RO, _U,		R_AROU,		
		0xff
	};

static	Uchar	ra5cnj5[] = {
		0x01,	_XTU,			R_ONBIN2,	
		0x01,	_RA,			R_MIZEN5,	
		0x11,	_SA,			R_MIZEN5,	
		0x11,	_SI,			R_SA5RENYOU3,	
		0x11,	_SU,			R_SHUUSI,	
		0x12,	_SO, _U,		R_AROU,		
		0x01,	_RI,			R_RENYOU5,	
		0x01,	_RU,			R_SHUUSI,	
		0x01,	_RE,			R_EXP,		
		0x11,	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KATEI2,	
		0x02,	_RO, _U,		R_AROU,		
		0xff
	};

static	Uchar	ra5cnj7[] = {
		0x01,	_XTU,			R_ONBIN2,	
		0x01,	_RA,			R_MIZEN5,	
		0x11,	_SA,			R_MIZEN5,	
		0x11,	_SI,			R_SA5RENYOU3,	
		0x11,	_SU,			R_SHUUSI,	
		0x12,	_SO, _U,		R_AROU,		
		0x01,	_RI,			R_RENYOU7,	
		0x01,	_RU,			R_SHUUSI,	
		0x01,	_RE,			R_EXP,		
		0x11,	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KATEI2,	
		0x02,	_RO, _U,		R_AROU,		
		0xff
	};

static	Uchar	wa5cnj1[] = {
		0x01,	_I,			R_RENYOU5,	
		0x01,	_U,			R_SHUUSI,	
		0x01,	_E,			R_KAMEI,	
		0x02,	_O, _U,			R_AROU,		
		0x01,	_XTU,			R_ONBIN2,	
		0x01,	_WA,			R_MIZEN5,	
		0xff
	};

static	Uchar	wa5cnj3[] = {
		0x01,	_I,			R_RENYOU7,	
		0x01,	_U,			R_SHUUSI,	
		0x01,	_E,			R_KAMEI,	
		0x02,	_O, _U,			R_AROU,		
		0x01,	_XTU,			R_ONBIN2,	
		0x01,	_WA,			R_MIZEN5,	
		0xff
	};

static	Uchar	wa5cnj5[] = {
		0x01,	_I,			R_RENYOU5,	
		0x01,	_U,			R_SHUUSI,	
		0x01,	_E,			R_EXP,		
		0x11,	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KATEI2,	
		0x02,	_O, _U,			R_AROU,		
		0x01,	_XTU,			R_ONBIN2,	
		0x01,	_WA,			R_MIZEN5,	
		0x11,	_SA,			R_MIZEN5,	
		0x11,	_SI,			R_SA5RENYOU3,	
		0x11,	_SU,			R_SHUUSI,	
		0x12,	_SO, _U,		R_AROU,		
		0xff
	};

static	Uchar	wa5cnj7[] = {
		0x01,	_I,			R_RENYOU7,	
		0x01,	_U,			R_SHUUSI,	
		0x01,	_E,			R_EXP,		
		0x11,	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KATEI2,	
		0x02,	_O, _U,			R_AROU,		
		0x01,	_XTU,			R_ONBIN2,	
		0x01,	_WA,			R_MIZEN5,	
		0x11,	_SA,			R_MIZEN5,	
		0x11,	_SI,			R_SA5RENYOU3,	
		0x11,	_SU,			R_SHUUSI,	
		0x12,	_SO, _U,		R_AROU,		
		0xff
	};

static	Uchar	ichicnj1[] = {
		0x00,				R_RENYOU1,	
		0x01,	_YO,			R_MEIREI2,	
		0x01,	_RU,			R_SHUUSI,	
		0x01,	_RE,			R_KATEI2,	
		0x01,	_RO,			R_MEIREI1,	
		0xff
	};

static	Uchar	ichicnj3[] = {
		0x00,				R_RENYOU3,	
		0x01,	_YO,			R_MEIREI2,	
		0x01,	_RU,			R_SHUUSI,	
		0x01,	_RE,			R_KATEI2,	
		0x01,	_RO,			R_MEIREI1,	
		0xff
	};

static	Uchar	keiyou1[] = {
		0x80,				R_KGOKAN,	
		0x01,	_I,			R_KSHUUSI,	
		0x02,	_KA, _XTU,		R_ONBIN1,	
		0x12,	_RO, _U,		R_KAROU,	
		0x01,	_KI,			R_KRENTAI,	
		0x01,	_KU,			R_KRENYOU,	
		0x02,	_KE, _RE,		R_KATEI2,	
		0xff
	};

static	Uchar	keiyou2[] = {
		0x80,				R_KGOKAN,	
		0x01,	_I,			R_KSHUUSI,	
		0x02,	_KA, _XTU,		R_ONBIN1,	
		0x12,	_RO, _U,		R_KAROU,	
		0x01,	_KI,			R_KRENTAI,	
		0x01,	_KU,			R_KRENYOU,	
		0x02,	_KE, _RE,		R_KATEI2,	
		0x01,	_SA,			R_FZKMEISI,	
		0xff
	};

static	Uchar	keiyou3[] = {
		0x80,				R_KGOKAN,	
		0x01,	_I,			R_KSHUUSI,	
		0x02,	_KA, _XTU,		R_ONBIN1,	
		0x12,	_RO, _U,		R_KAROU,	
		0x01,	_KI,			R_KRENTAI,	
		0x01,	_KU,			R_KRENYOU,	
		0x02,	_KE, _RE,		R_KATEI2,	
		0x01,	_GE,			R_DMEISI,	
		0x11,	_DA,			R_DSHUUSI1,	
		0x21,	_XTU,			R_ONBIN1,	
		0x22,	_RO, _U,		R_DAROU,	
		0x11,	_DE,			R_DRENYOU1,	
		0x11,	_NA,			R_DRENTAI1,	
		0x21,	_RA,			R_KATEI1,	
		0x21,	_RI,			R_DSHUUSI2,	
		0x21,	_RU,			R_DRENTAI3,	
		0x21,	_RE,			R_KAMEI,	
		0x11,	_NI,			R_DRENYOU2,	
		0x01,	_SA,			R_FZKMEISI,	
		0xff
	};

static	Uchar	keiyou4[] = {
		0x80,				R_KGOKAN,	
		0x01,	_I,			R_KSHUUSI,	
		0x02,	_KA, _XTU,		R_ONBIN1,	
		0x12,	_RO, _U,		R_KAROU,	
		0x02,	_GA, _XTU,		R_ONBIN2,	
		0x11,	_RA,			R_MIZEN5,	
		0x11,	_RI,			R_RENYOU5,	
		0x11, 	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KAMEI,	
		0x12,	_RO, _U,		R_AROU,		
		0x01,	_KI,			R_KRENTAI,	
		0x01,	_KU,			R_KRENYOU,	
		0x02,	_KE, _RE,		R_KATEI2,	
		0x01,	_SA,			R_FZKMEISI,	
		0xff
	};

static	Uchar	keiyou5[] = {
		0x80,				R_KGOKAN,	
		0x01,	_I,			R_KSHUUSI,	
		0x02,	_KA, _XTU,		R_ONBIN1,	
		0x12,	_RO, _U,		R_KAROU,	
		0x02,	_GA, _XTU,		R_ONBIN2,	
		0x11,	_RA,			R_MIZEN5,	
		0x11,	_RI,			R_RENYOU5,	
		0x11, 	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KAMEI,	
		0x12,	_RO, _U,		R_AROU,		
		0x01,	_KI,			R_KRENTAI,	
		0x01,	_KU,			R_KRENYOU,	
		0x02,	_KE, _RE,		R_KATEI2,	
		0x01,	_GE,			R_DMEISI,	
		0x11,	_DA,			R_DSHUUSI1,	
		0x21,	_XTU,			R_ONBIN1,	
		0x22,	_RO, _U,		R_DAROU,	
		0x11,	_DE,			R_DRENYOU1,	
		0x11,	_NA,			R_DRENTAI1,	
		0x21,	_RA,			R_KATEI1,	
		0x21,	_RI,			R_DSHUUSI2,	
		0x21,	_RU,			R_DRENTAI3,	
		0x21,	_RE,			R_KAMEI,	
		0x11,	_NI,			R_DRENYOU2,	
		0x01,	_SA,			R_FZKMEISI,	
		0xff
	};

static	Uchar	keiyou6[] = {
		0x80,				R_KGOKAN,	
		0x01,	_I,			R_KSHUUSI,	
		0x02,	_KA, _XTU,		R_ONBIN1,	
		0x12,	_RO, _U,		R_KAROU,	
		0x01,	_KI,			R_KRENTAI,	
		0x01,	_KU,			R_KRENYOU,	
		0x02,	_KE, _RE,		R_KATEI2,	
		0x01,	_GE,			R_DMEISI,	
		0x11,	_DA,			R_DSHUUSI1,	
		0x21,	_XTU,			R_ONBIN1,	
		0x22,	_RO, _U,		R_DAROU,	
		0x11,	_DE,			R_DRENYOU1,	
		0x11,	_NA,			R_DRENTAI1,	
		0x21,	_RA,			R_KATEI1,	
		0x21,	_RI,			R_DSHUUSI2,	
		0x21,	_RU,			R_DRENTAI3,	
		0x21,	_RE,			R_KAMEI,	
		0x11,	_NI,			R_DRENYOU2,	
		0x01,	_SA,			R_FZKMEISI,	
		0x01,	_ME,			R_DMEISI,	
		0x11,	_DA,			R_DSHUUSI1,	
		0x21,	_XTU,			R_ONBIN1,	
		0x22,	_RO, _U,		R_DAROU,	
		0x11,	_DE,			R_DRENYOU1,	
		0x11,	_NA,			R_DRENTAI1,	
		0x21,	_RA,			R_KATEI1,	
		0x21,	_RI,			R_DSHUUSI2,	
		0x21,	_RU,			R_DRENTAI3,	
		0x21,	_RE,			R_KAMEI,	
		0x11,	_NI,			R_DRENYOU2,	
		0xff
	};

static	Uchar	keiyou7[] = {
		0x80,				R_KGOKAN,	
		0x01,	_I,			R_KSHUUSI,	
		0x02,	_KA, _XTU,		R_ONBIN1,	
		0x12,	_RO, _U,		R_KAROU,	
		0x02,	_GA, _XTU,		R_ONBIN2,	
		0x11,	_RA,			R_MIZEN5,	
		0x11,	_RI,			R_RENYOU5,	
		0x11, 	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KAMEI,	
		0x12,	_RO, _U,		R_AROU,		
		0x01,	_KI,			R_KRENTAI,	
		0x01,	_KU,			R_KRENYOU,	
		0x02,	_KE, _RE,		R_KATEI2,	
		0x01,	_GE,			R_DMEISI,	
		0x11,	_DA,			R_DSHUUSI1,	
		0x21,	_XTU,			R_ONBIN1,	
		0x22,	_RO, _U,		R_DAROU,	
		0x11,	_DE,			R_DRENYOU1,	
		0x11,	_NA,			R_DRENTAI1,	
		0x21,	_RA,			R_KATEI1,	
		0x21,	_RI,			R_DSHUUSI2,	
		0x21,	_RU,			R_DRENTAI3,	
		0x21,	_RE,			R_KAMEI,	
		0x11,	_NI,			R_DRENYOU2,	
		0x01,	_SA,			R_FZKMEISI,	
		0x01,	_ME,			R_DMEISI,	
		0x11,	_DA,			R_DSHUUSI1,	
		0x21,	_XTU,			R_ONBIN1,	
		0x22,	_RO, _U,		R_DAROU,	
		0x11,	_DE,			R_DRENYOU1,	
		0x11,	_NA,			R_DRENTAI1,	
		0x21,	_RA,			R_KATEI1,	
		0x21,	_RI,			R_DSHUUSI2,	
		0x21,	_RU,			R_DRENTAI3,	
		0x21,	_RE,			R_KAMEI,	
		0x11,	_NI,			R_DRENYOU2,	
		0xff
	};

static	Uchar	keiyou8[] = {
		0x80,				R_KGOKAN,	
		0x01,	_I,			R_KSHUUSI,	
		0x02,	_KA, _XTU,		R_ONBIN1,	
		0x12,	_RO, _U,		R_KAROU,	
		0x02,	_GA, _XTU,		R_ONBIN2,	
		0x11,	_RA,			R_MIZEN5,	
		0x11,	_RI,			R_RENYOU5,	
		0x11, 	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KAMEI,	
		0x12,	_RO, _U,		R_AROU,		
		0x01,	_KI,			R_KRENTAI,	
		0x01,	_KU,			R_KRENYOU,	
		0x02,	_KE, _RE,		R_KATEI2,	
		0x01,	_GE,			R_DMEISI,	
		0x11,	_DA,			R_DSHUUSI1,	
		0x21,	_XTU,			R_ONBIN1,	
		0x22,	_RO, _U,		R_DAROU,	
		0x11,	_DE,			R_DRENYOU1,	
		0x11,	_NA,			R_DRENTAI1,	
		0x21,	_RA,			R_KATEI1,	
		0x21,	_RI,			R_DSHUUSI2,	
		0x21,	_RU,			R_DRENTAI3,	
		0x21,	_RE,			R_KAMEI,	
		0x11,	_NI,			R_DRENYOU2,	
		0x01,	_SA,			R_FZKMEISI,	
		0x01,	_MI,			R_FZKMEISI,	
		0x01,	_ME,			R_DMEISI,	
		0x11,	_DA,			R_DSHUUSI1,	
		0x21,	_XTU,			R_ONBIN1,	
		0x22,	_RO, _U,		R_DAROU,	
		0x11,	_DE,			R_DRENYOU1,	
		0x11,	_NA,			R_DRENTAI1,	
		0x21,	_RA,			R_KATEI1,	
		0x21,	_RI,			R_DSHUUSI2,	
		0x21,	_RU,			R_DRENTAI3,	
		0x21,	_RE,			R_KAMEI,	
		0x11,	_NI,			R_DRENYOU2,	
		0xff
	};

static	Uchar	keiyou9[] = {
		0x80,				R_KGOKAN,	
		0x01,	_I,			R_KSHUUSI,	
		0x02,	_KA, _XTU,		R_ONBIN1,	
		0x12,	_RO, _U,		R_KAROU,	
		0x02,	_GA, _XTU,		R_ONBIN2,	
		0x11,	_RA,			R_MIZEN5,	
		0x11,	_RI,			R_RENYOU5,	
		0x11, 	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KAMEI,	
		0x12,	_RO, _U,		R_AROU,		
		0x01,	_KI,			R_KRENTAI,	
		0x01,	_KU,			R_KRENYOU,	
		0x02,	_KE, _RE,		R_KATEI2,	
		0x01,	_GE,			R_DMEISI,	
		0x11,	_DA,			R_DSHUUSI1,	
		0x21,	_XTU,			R_ONBIN1,	
		0x22,	_RO, _U,		R_DAROU,	
		0x11,	_DE,			R_DRENYOU1,	
		0x11,	_NA,			R_DRENTAI1,	
		0x21,	_RA,			R_KATEI1,	
		0x21,	_RI,			R_DSHUUSI2,	
		0x21,	_RU,			R_DRENTAI3,	
		0x21,	_RE,			R_KAMEI,	
		0x11,	_NI,			R_DRENYOU2,	
		0x01,	_SA,			R_FZKMEISI,	
		0xff
	};

static	Uchar	keiyou10[] = {
		0x80,				R_KGOKAN,	
		0x01,	_I,			R_KSHUUSI,	
		0x02,	_KA, _XTU,		R_ONBIN1,	
		0x12,	_RO, _U,		R_KAROU,	
		0x02,	_GA, _XTU,		R_ONBIN2,	
		0x11,	_RA,			R_MIZEN5,	
		0x11,	_RI,			R_RENYOU5,	
		0x11, 	_RU,			R_SHUUSI,	
		0x11,	_RE,			R_KAMEI,	
		0x12,	_RO, _U,		R_AROU,		
		0x01,	_KI,			R_KRENTAI,	
		0x01,	_KU,			R_KRENYOU,	
		0x02,	_KE, _RE,		R_KATEI2,	
		0x01,	_GE,			R_DMEISI,	
		0x11,	_DA,			R_DSHUUSI1,	
		0x21,	_XTU,			R_ONBIN1,	
		0x22,	_RO, _U,		R_DAROU,	
		0x11,	_DE,			R_DRENYOU1,	
		0x11,	_NA,			R_DRENTAI1,	
		0x21,	_RA,			R_KATEI1,	
		0x21,	_RI,			R_DSHUUSI2,	
		0x21,	_RU,			R_DRENTAI3,	
		0x21,	_RE,			R_KAMEI,	
		0x11,	_NI,			R_DRENYOU2,	
		0x01,	_SA,			R_FZKMEISI,	
		0x01,	_ME,			R_DMEISI,	
		0x11,	_DA,			R_DSHUUSI1,	
		0x21,	_XTU,			R_ONBIN1,	
		0x22,	_RO, _U,		R_DAROU,	
		0x11,	_DE,			R_DRENYOU1,	
		0x11,	_NA,			R_DRENTAI1,	
		0x21,	_RA,			R_KATEI1,	
		0x21,	_RI,			R_DSHUUSI2,	
		0x21,	_RU,			R_DRENTAI3,	
		0x21,	_RE,			R_KAMEI,	
		0x11,	_NI,			R_DRENYOU2,	
		0xff
	};

static	Uchar	keiyou11[] = {
		0x80,				R_KGOKAN,	
		0x01,	_I,			R_KSHUUSI,	
		0x02,	_KA, _XTU,		R_ONBIN1,	
		0x12,	_RO, _U,		R_KAROU,	
		0x01,	_KI,			R_KRENTAI,	
		0x01,	_KU,			R_KRENYOU,	
		0x02,	_KE, _RE,		R_KATEI2,	
		0x01,	_SA,			R_FZKMEISI,	
		0x01,	_MI,			R_FZKMEISI,	
		0x01,	_ME,			R_DMEISI,	
		0x11,	_DA,			R_DSHUUSI1,	
		0x21,	_XTU,			R_ONBIN1,	
		0x22,	_RO, _U,		R_DAROU,	
		0x11,	_DE,			R_DRENYOU1,	
		0x11,	_NA,			R_DRENTAI1,	
		0x21,	_RA,			R_KATEI1,	
		0x21,	_RI,			R_DSHUUSI2,	
		0x21,	_RU,			R_DRENTAI3,	
		0x21,	_RE,			R_KAMEI,	
		0x11,	_NI,			R_DRENYOU2,	
		0xff
	};

static	Uchar	keidou1[] = {
		0x80,				R_DGOKAN,	
		0x01,	_DA,			R_DSHUUSI1,	
		0x11,	_XTU,			R_ONBIN1,	
		0x12,	_RO, _U,		R_DAROU,	
		0x01,	_DE,			R_DRENYOU1,	
		0x01,	_NA,			R_DRENTAI1,	
		0x11,	_RA,			R_KATEI1,	
		0x11,	_RI,			R_DSHUUSI2,	
		0x11,	_RU,			R_DRENTAI3,	
		0x11,	_RE,			R_KAMEI,	
		0x01,	_NI,			R_DRENYOU2,	
		0xff
	};

static	Uchar	keidou2[] = {
		0x80,				R_DGOKAN,	
		0x02,	_TA, _RU,		R_DRENTAI2,	
		0x11,	_RE,			R_KATEI2,	
		0x01,	_TO,			R_DRENYOU3,	
		0xff
	};

static	Uchar	keidou4[] = {
		0x80,				R_DGOKAN,	
		0x01,	_SA,			R_FZKMEISI,	
		0x01,	_DA,			R_DSHUUSI1,	
		0x11,	_XTU,			R_ONBIN1,	
		0x12,	_RO, _U,		R_DAROU,	
		0x01,	_DE,			R_DRENYOU1,	
		0x01,	_NA,			R_DRENTAI1,	
		0x11,	_RA,			R_KATEI1,	
		0x11,	_RI,			R_DSHUUSI2,	
		0x11,	_RU,			R_DRENTAI3,	
		0x11,	_RE,			R_KAMEI,	
		0x01,	_NI,			R_DRENYOU2,	
		0xff
	};

static	Uchar	keidou9[] = {
		0x80,				R_DGOKAN,	
		0x01,	_DA,			R_DSHUUSI1,	
		0x11,	_XTU,			R_ONBIN1,	
		0x12,	_RO, _U,		R_DAROU,	
		0x01,	_DE,			R_DRENYOU1,	
		0x01,	_NA,			R_DRENTAI1,	
		0x11,	_RA,			R_KATEI1,	
		0x11,	_RI,			R_DSHUUSI2,	
		0x11,	_RU,			R_DRENTAI3,	
		0x11,	_RE,			R_KAMEI,	
		0x01,	_NI,			R_DRENYOU2,	
		0x01,	_NO,			R_CUT,		
		0x12,	_KO, _TO,		R_KOTO,		
		0xff
	};

static	Uchar	sahenmeisi1[] = {
		0x00,				R_SMEISI1,	
		0x01,	_SA,			R_MIZEN1,	
		0x01,	_SI,			R_SI1,		
		0x11,	_RO,			R_MEIREI1,	
		0x02,	_SU, _RU,		R_SHUUSI,	
		0x25,   _KA, _SI, _NA, _I, _KA, R_KA1,	
		0x11,	_RE,			R_KATEI2,	
		0x01,	_SE,			R_MIZEN2,	
		0x11,	_YO,			R_MEIREI2,	
		0xff
	};

static	Uchar	sahenmeisi2[] = {
		0x00,				R_SMEISI1,	
		0x01,	_SA,			R_MIZEN1,	
		0x01,	_SI,			R_SI1,		
		0x11,	_RO,			R_MEIREI1,	
		0x02,	_SU, _RU,		R_SHUUSI,	
		0x25,   _KA, _SI, _NA, _I, _KA, R_KA1,	
		0x11,	_RE,			R_KATEI2,	
		0x01,	_SE,			R_MIZEN2,	
		0x11,	_YO,			R_MEIREI2,	
		0xff
	};

static	Uchar	keidoumeisi[] = {
		0x00,				R_DMEISI,	
		0x01,	_SA,			R_FZKMEISI,	
		0x01,	_DA,			R_DSHUUSI1,	
		0x11,	_XTU,			R_ONBIN1,	
		0x12,	_RO, _U,		R_DAROU,	
		0x01,	_DE,			R_DRENYOU1,	
		0x01,	_NA,			R_DRENTAI1,	
		0x11,	_RA,			R_KATEI1,	
		0x11,	_RI,			R_DSHUUSI2,	
		0x11,	_RU,			R_DRENTAI3,	
		0x11,	_RE,			R_KAMEI,	
		0x01,	_NI,			R_DRENYOU2,	
		0xff
	};

static	Uchar	keiyoustb[] = {
		0x00,				R_KSTB,		
		0x01,	_I,			R_KSHUUSI,	
		0x02,	_KA, _XTU,		R_ONBIN1,	
		0x12,	_RO, _U,		R_KAROU,	
		0x01,	_KI,			R_KRENTAI,	
		0x01,	_KU,			R_KRENYOU,	
		0x02,	_KE, _RE,		R_KATEI2,	
		0xff
	};

static	Uchar	ichistb[] = {
		0x00,				R_ICHISETUBI,	
		0x01,	_YO,			R_MEIREI2,	
		0x01,	_RU,			R_SHUUSI,	
		0x01,	_RE,			R_KATEI2,	
		0x01,	_RO,			R_MEIREI1,	
		0xff
	};

static	Uchar	fukusi2[] = {
		0x00,				R_FUKUSI,	
		0x01,	_TO,			R_F_FUKUSI,	
		0x11,	_SI,			R_SI1,		
		0x11,	_HA,			R_F_FUKUSI,	
		0xff
	};

static	Uchar	fukusi3[] = {
		0x00,				R_FUKUSI,	
		0x01,	_SA,			R_MIZEN1,	
		0x01,	_SI,			R_SI1,		
		0x11,	_RO,			R_MEIREI1,	
		0x02,	_SU, _RU,		R_SHUUSI,	
		0x25,   _KA, _SI, _NA, _I, _KA, R_KA1,	
		0x11,	_RE,			R_KATEI2,	
		0x01,	_SE,			R_MIZEN2,	
		0x11,	_YO,			R_MEIREI2,	
		0x01,	_TO,			R_F_FUKUSI,	
		0x11,	_SI,			R_SI1,		
		0x11,	_HA,			R_F_FUKUSI,	
		0xff
	};

static	Uchar	fukusi4[] = {
		0x00,				R_FUKUSI,	
		0x01,	_SA,			R_MIZEN1,	
		0x01,	_SI,			R_SI1,		
		0x11,	_RO,			R_MEIREI1,	
		0x02,	_SU, _RU,		R_SHUUSI,	
		0x25,   _KA, _SI, _NA, _I, _KA, R_KA1,	
		0x11,	_RE,			R_KATEI2,	
		0x01,	_SE,			R_MIZEN2,	
		0x11,	_YO,			R_MEIREI2,	
		0xff
	};

static	Uchar	fukusi5[] = {
		0x00,				R_FUKUSI,	
		0x01,	_NI,			R_F_FUKUSI,	
		0xff
	};

static	Uchar	fukusi6[] = {
		0x00,				R_FUKUSI,	
		0x01,	_MO,			R_F_FUKUSI,	
		0xff
	};

static	Uchar	fukusi7[] = {
		0x00,				R_FUKUSI,	
		0x01,	_NO,			R_F_FUKUSI,	
		0xff
	};

static	Uchar	fukusi8[] = {
		0x00,				R_FUKUSI,	
		0x01,	_DA,			R_DSHUUSI1,	
		0x11,	_XTU,			R_ONBIN1,	
		0x02,	_DE, _SI,		R_DESHI,	
		0x21,	_XYO,			R_SHO,		
		0x31,	_U,			R_DESHOU,	
		0x11,	_SU,			R_DESU,		
		0x01,	_TO,			R_F_FUKUSI,	
		0x11,	_SI,			R_SI1,		
		0x11,	_HA,			R_F_FUKUSI,	
		0xff
	};

static	Uchar	fukusi9[] = {
		0x00,				R_FUKUSI,	
		0x01,	_SA,			R_MIZEN1,	
		0x01,	_SI,			R_SI1,		
		0x11,	_RO,			R_MEIREI1,	
		0x02,	_SU, _RU,		R_SHUUSI,	
		0x25,   _KA, _SI, _NA, _I, _KA, R_KA1,	
		0x11,	_RE,			R_KATEI2,	
		0x01,	_SE,			R_MIZEN2,	
		0x11,	_YO,			R_MEIREI2,	
		0x01,	_DA,			R_DSHUUSI1,	
		0x11,	_XTU,			R_ONBIN1,	
		0x02,	_DE, _SI,		R_DESHI,	
		0x21,	_XYO,			R_SHO,		
		0x31,	_U,			R_DESHOU,	
		0x11,	_SU,			R_DESU,		
		0x01,	_TO,			R_F_FUKUSI,	
		0x11,	_SI,			R_SI1,		
		0x11,	_HA,			R_F_FUKUSI,	
		0x01,	_NO,			R_F_FUKUSI,	
		0xff
	};

Uchar	*conjadr[] = {
		0,			
		0,			
		0,			
		0,			
		0,			
		sahenmeisi1,		
		sahenmeisi1,		
		sahenmeisi2,		
		keidoumeisi,		
		keidoumeisi,		
		keidoumeisi,		
		0,			

		0,			
		0,			
		0,			
		sahenmeisi1,		
		keidoumeisi,		
		0,			

		0,			
		0,			
		0, 			

		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		
		0,			
		0,			
		0,			
		0,			
		0,			

		0,			
		0,			
		0,			
		0,			
		keiyoustb,		
		keidoumeisi,		
		sahenmeisi1,		
		ichistb,		
		0,			

		0,			
		fukusi2,		
		fukusi3,		
		fukusi4,		
		fukusi5,		
		fukusi6,		
		fukusi7,		
		fukusi8,		
		fukusi9,		
		0,			
		0,			
		0,			
		0,			
		0,			
		0, 			

		keiyou1,		
		keiyou2,		
		keiyou3,		
		keiyou4,		
		keiyou5,		
		keiyou6,		
		keiyou7,		
		keiyou8,		
		keiyou9,		
		keiyou10,		
		keiyou11,		

		keidou1,		
		keidou2,		
		keidou1,		
		keidou4,		
		keidou4,		
		keidou1,		
		keidou4,		
		keidou1,		
		keidou9,		

		sahencnj,		
		zahencnj,		
		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		0,			

		ichicnj1,		
		ka5cnj1,		
		ga5cnj1,		
		sa5cnj1,		
		ta5cnj1,		
		na5cnj,			
		ba5cnj1,		
		ma5cnj1,		
		ra5cnj1,		
		wa5cnj1,		

		ichicnj1,		
		ka5cnj1,		
		ga5cnj1,		
		sa5cnj1,		
		ta5cnj1,		
		ba5cnj1,		
		ma5cnj1,		
		ra5cnj1,		
		wa5cnj1,		
		0,			
		0,			

		ka5cnj5,		
		ga5cnj5,		
		sa5cnj5,		
		ta5cnj5,		
		ba5cnj5,		
		ma5cnj5,		
		ra5cnj5,		
		wa5cnj5,		
		0,			
		0,			

		ka5cnj5,		
		ga5cnj5,		
		sa5cnj5,		
		ta5cnj5,		
		ba5cnj5,		
		ma5cnj5,		
		ra5cnj5,		
		wa5cnj5,		
		0,			

		ichicnj3,		
		ka5cnj3,		
		ga5cnj3,		
		sa5cnj3,		
		ta5cnj3,		
		ba5cnj3,		
		ma5cnj3,		
		ra5cnj3,		
		wa5cnj3,		
		0,			

		ichicnj3,		
		ka5cnj3,		
		ga5cnj3,		
		sa5cnj3,		
		ta5cnj3,		
		ba5cnj3,		
		ma5cnj3,		
		ra5cnj3,		
		wa5cnj3,		
		0,			
		0,			

		ka5cnj7,		
		ga5cnj7,		
		sa5cnj7,		
		ta5cnj7,		
		ba5cnj7,		
		ma5cnj7,		
		ra5cnj7,		
		wa5cnj7,		
		0,			
		0,			

		ka5cnj7,		
		ga5cnj7,		
		sa5cnj7,		
		ta5cnj7,		
		ba5cnj7,		
		ma5cnj7,		
		ra5cnj7,		
		wa5cnj7,		
		0,			
		0,			

		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		0,			

		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		0,			
		0, 			
		0,			
		0,			
		0,			
		0,			
		0,			
		0			
};
