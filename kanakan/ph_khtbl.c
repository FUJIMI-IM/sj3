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
 * $SonyRCSfile: ph_khtbl.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:09 $
 */



#include "sj_kcnv.h"

Uchar	*skipkstr();
STDYIN	*srchstdy();
Void	ph_setkouho();
Int	seldict();

Static	Void	ph_setsty(clrec)
CLREC	*clrec;
{
	JREC		*jrec;
	TypeDicOfs	offset;
	Uchar		*ptr;
	STDYIN		*sptr;

	if (!seldict((jrec = clrec -> jnode) -> dicid)) {
		if (!khcount)
			ph_setkouho(clrec, (TypeDicOfs)1, (STDYIN *)NULL);
		return;
	}

	(*curdict->getdic)(curdict, jrec -> jseg);
	ptr = dicbuf + jrec->jofsst + 1;

	for ( ; *ptr != HinsiBlkTerm ; ptr = skipkstr(ptr)) {

		offset = ptr - dicbuf;

		sptr = srchstdy(jrec -> jseg, offset, jrec -> dicid);

		if (!khcount){
			ph_setkouho(clrec, offset, sptr);
			continue;
		}

		if (!sptr) continue;

		if (sptr -> styno < kouhotbl[0].styno) {
			ph_setkouho(clrec, offset, sptr);
		}

		else if (sptr -> styno == kouhotbl -> styno) {
			if (!(kouhotbl[0].ka_fg &&
			   sptr -> ka_kj == kouhotbl[0].ka_kj)) {
				ph_setkouho(clrec, offset, sptr);
			}
		}
	}
}



Int	ph_khtbl(clrec)
CLREC	*clrec;
{
	JREC	*jrec;
	Int	flg = FALSE;

	switch((jrec = clrec -> jnode) -> class) {
	case C_DICT:
		ph_setsty(clrec);
		break;

	case C_N_ARABIA:
	case C_N_ARACMA:
	case C_N_KAZU:
	case C_N_SUUJI:
	case C_N_NANSUU:
	case C_N_KANKURA:
	case C_N_ARAKURA:
	case C_N_KAZULONG:
	case C_N_KAZULCMA:
	case C_N_SUUJILONG:
		if (jrec -> jofsst) {
			ph_setsty(clrec);
			break;
		}
		else
			flg = TRUE;
	case C_BUNSETU:
	case C_MINASI:
	case C_WAKACHI:
		if (!khcount) ph_setkouho(clrec, (TypeDicOfs)0, (STDYIN *)0);
		break;
	}

	return flg;
}

