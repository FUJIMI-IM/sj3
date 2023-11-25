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
 * $SonyRCSfile: setconj.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:16 $
 */



#include "sj_kcnv.h"
#include "sj_right.h"
#include "sj_hinsi.h"
#include "sj_yomi.h"

Static	Int	cnjstrcmp(yptr, cnjp, saml)
Uchar	*yptr;
Uchar	TFar	*cnjp;
Int	*saml;
{
	Int	asklen;
	Int	nkrlen;

	asklen = CnjAskLen(cnjp);

	if (*saml < asklen) return CONT;

	if (*saml > asklen) return OVER;

	nkrlen = CnjNkrLen(cnjp);

	yptr += asklen;
	cnjp ++;

	while (nkrlen--) {
		if (*yptr > *cnjp)
			return CONT;

		else if (*yptr < *cnjp) {
			if (!*yptr) return PARTLY;
			return OVER;
		}

		else{
			yptr++;
			cnjp++;
			(*saml)++;
		}
	}

	return MATCH;
}

Static	TypeCnct	cnvrow(rec, row)
JREC		*rec;
TypeCnct	row;
{
	TypeGram	hinsi;
	Int		stt;

	hinsi = rec -> hinsi;

	if ((stt = rec -> sttofs) == SETTOU_O) {

		if (R_RENYOU1 <= row && row <= R_SA5RENYOU1)
			row += 6;

		else if (R_RENYOU3 <= row && row <= R_SA5RENYOU3)
			row += 3;

		else if((hinsi>=KEIYOUSI_1 && hinsi<=KE_DOUSI_9)||
			(hinsi == MEISI_5) || (hinsi == MEISI_9))
			;

		else
			row = 0;
	}

	else if (stt == SETTOU_GO) {

		if (hinsi == MEISI_7 && row == R_SMEISI1)
			row++;
	}

	return row;
}

Int	setconj(hinsi, jrec, crec)
TypeGram	hinsi;
JREC		*jrec;
CREC		*crec;
{
	Uchar	*yptr;
	Int	cmp;
	TypeCnct	right;
	Int	saml;
	Int	ofslen;
	int	count = 0;
	Uchar	TFar	*cnj;

	if (cnj  = Conjadr(hinsi)) {
		saml = ofslen = 0;

		yptr = cnvstart + jrec -> jlen;

		if ((*cnj) & TYOUONFLG) {
			while (*yptr == _TYOUON) {
				yptr++;
				ofslen++;
			}
		}

		for ( ; *cnj != CNJEND ; cnj += CnjRecLen(cnj)) {
			cmp = cnjstrcmp(yptr, cnj, &saml);

			if (cmp == OVER) break;

			if (cmp == PARTLY) break;

			if (cmp != MATCH) continue;

			if (isdpnd(*(yptr + saml))) continue;

			if (right = cnvrow(jrec, (TypeCnct)CnjRight(cnj))) {
				crec -> len   = (Uchar)(ofslen + saml);
				crec -> right = right;
				count++;
				crec++;
			}
		}
	}

	return count;
}

