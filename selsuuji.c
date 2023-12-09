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

#include "sj_rename.h"
#include "sj_typedef.h"
#include "const.h"
#include "sj_suuji.h"

static	u_short	selsjAR[] = {
	SelArb0,
	SelArbCma	| JFLAG_NK4 | JFLAG_NN0,
	SelKanSjYm	| JFLAG_NN0,
	SelKanSjYm2	| SelNum1_23_10_1 | JFLAG_NN0,
	SelKanSj0	| JFLAG_NK2,
	SelNumTerm
};

static	u_short	selsjARCM[] = {
	SelArbCma	| JFLAG_NK4 | JFLAG_NN0,
	SelArb0,
	SelKanSjYm	| JFLAG_NN0,
	SelKanSjYm2	| SelNum1_23_10_1 | JFLAG_NN0,
	SelKanSj0	| JFLAG_NK2,
	SelNumTerm
};

static	u_short	selsjKAZU[] = {
	SelKanSjYm	| JFLAG_NN0,
	SelKanSjYm2	| SelNum1_23_10_1 | JFLAG_NN0,
	SelKanSj0	| JFLAG_NK2,
	SelArb0,
	SelArbCma	| JFLAG_NK4 | JFLAG_NN0,
	SelNumTerm
};

static	u_short	selsjSUUJI[] = {
	SelKanSj0,
	SelArb0,
	SelNumTerm
};

static	u_short	selsjNANSUU[] = {
	SelKanSjYm,
	SelNumTerm
};

static	u_short	selsjAR17[] = {
	SelLongArb,
	SelLongArbCma,
	SelNumTerm
};

static	u_short	selsjAR17CM[] = {
	SelLongArbCma,
	SelLongArb,
	SelNumTerm
};

static	u_short	selsjAR17SJ[] = {
	SelLongArb,
	SelLongSj,
	SelNumTerm
};

static	u_short	selsjKANKURA[] = {
	SelKanSjKr,
	SelArbKr,
	SelNumTerm
};

static	u_short	selsjARAKURA[] = {
	SelArbKr,
	SelKanSjKr	| JFLAG_NSN,
	SelNumTerm
};


u_short	*selsjadrs[] = {
		selsjAR,
		selsjARCM,
			selsjKAZU,
			selsjSUUJI,
		selsjNANSUU,
		selsjKANKURA,
		selsjARAKURA,
		selsjAR17,
		selsjAR17CM,
		selsjAR17SJ
};
