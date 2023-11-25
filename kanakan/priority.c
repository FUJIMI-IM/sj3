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
 * $SonyRCSfile: priority.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:10 $
 */



#include "sj_kcnv.h"
#include "sj_hinsi.h"
#include "sj_right.h"
#include "sj_prty.h"

Static	Int	isfukusi(hinsi)
TypeGram	hinsi;
{
	if (FUKUSI_1 <= hinsi && hinsi <= FUKUSI_7)
		return TRUE;
	else if (hinsi == MEISI_11 || hinsi == D_MEISI_6)
		return TRUE;
	return FALSE;
}

Static	Int	istaigen(right)
TypeCnct	right;
{
	if (right == R_MEISI || right == R_DMEISI || right == R_SMEISI1)
		return TRUE;
	if (right == R_WO_MEISI || right == R_DAIMEISI || right == R_JOSUUSI)
		return TRUE;
	if (right == R_RENYOU1 || (R_RENYOU3 <= right && right <= R_SA5RENYOU4))
		return TRUE;
	return FALSE;
}

Static	Int	taicnt(hinsi1, hinsi2)
TypeGram	hinsi1;
TypeGram	hinsi2;
{
	if (hinsi2 == TANKANJI) return 0;

	if (MYOUJI <= hinsi1 && hinsi1 <= KEN_KU) {
		if (MEISI_1 <= hinsi2 && hinsi2 <= SETUBI_9)
			return Taipri(hinsi1 - MYOUJI, hinsi2);
		else
			return 3;
	}
	else if ((MEISI_1 <= hinsi1 && hinsi1 <= MEISI_11) ||
	         hinsi1 == D_MEISI_2) {
		if (MEISI_1 <= hinsi2 && hinsi2 <= SETUBI_9) {
			return Taipri(5, hinsi2);
		}
		else
			return 5;
	}

	else {
		if (MEISI_1 <= hinsi2 && hinsi2 <= SETUBI_9)
			return Taipri(6, hinsi2);
		else
			return 4;
	}
}

Static	Int	sttcnt(hinsi1, hinsi2)
TypeGram	hinsi1;
TypeGram	hinsi2;
{
	if (SETTOU_1 <= hinsi1 && hinsi1 <= SETTOU_5) {
		if (MEISI_1 <= hinsi2 && hinsi2 <= SUUSI)
			return Sttpri(hinsi1 - SETTOU_1, hinsi2);
	}
	return 4;
}

Int	priority(clrec)
CLREC	*clrec;
{
	int		prty;
	CLREC		*cl2rec;
	Int		keeplen;
	TypeGram	hinsi1;
	Int		i;
	Int		prev = 0;

	if (clrec->cllen == clrec->cl2len) return 0xff;

	if (clrec->jnode->class == C_BUNSETU) {
		prty = Termtbl[clrec->right] & GETPRI;
		return(0xf0 + prty);
	}


	hinsi1 = clrec->jnode->hinsi;

	if (prevclrow == R_WO  || prevclrow == R_WO_S ||
	    prevclrow == R_NI1 || prevclrow == R_DRENYOU2) {
		if (clrec -> kubun == K_DOUSHI || isfukusi(hinsi1))
			prev = 5;
	}

	else if (istaigen(prevclrow)) {

		if (clrec -> kubun == K_TAIGEN)
			prev = taicnt(prevclgrm, hinsi1);

		else
			prev = 0;
	}

	else if (prevclrow == R_SETTOU) {
		if (clrec -> jnode -> sttofs)
			prty = 2;
		else
			prty = sttcnt(prevclgrm, hinsi1);
	}


	if (hinsi1 >= SETUBI_1 && hinsi1 <= SETUBI_9 && (!prev)) {
		prty = 1;
		goto finish;
	}

	cl2rec = maxclptr;
	keeplen = cl2rec -> cllen;

	if (clrec->right == R_WO  || clrec->right == R_WO_S ||
	    clrec->right == R_NI1 || clrec->right == R_DRENYOU2) {
		while (cl2rec && (Int)cl2rec->cllen == keeplen) {
			if (cl2rec->kubun == K_DOUSHI) {
				prty = 14;
				goto finish;
			}
			cl2rec = cl2rec->clsort;
		}
	}

	else if (clrec->right == R_SETTOU) {
		prty = 0;
		while (cl2rec && (Int)cl2rec -> cllen == keeplen) {
			if (cl2rec -> jnode -> sttofs )
				i = 2;
			else
				i = sttcnt(hinsi1, cl2rec->jnode->hinsi);

			if (i > prty)
				prty = i;
			cl2rec = cl2rec -> clsort;
		}
		goto finish;
	}

	else if (istaigen(clrec->right)) {
		prty = 0;
		while (cl2rec && (Int)cl2rec->cllen == keeplen) {
			i = taicnt(hinsi1, cl2rec->jnode->hinsi);
			if (i > prty)
				prty = i;
			cl2rec = cl2rec->clsort;
		}
		if (clrec->jnode->stbofs) {
			prty -= 3;
		}
		goto finish;
	}


	prty = Termtbl[clrec->right];


	if (prty & RENTAI) {
		while (cl2rec && (Int)cl2rec->cllen == keeplen) {
			if (cl2rec -> kubun == K_TAIGEN) {
				prty &= GETPRI;
				goto finish;
			}
			cl2rec = cl2rec->clsort;
		}
		prty = 4;
	}

	else {
		prty &= GETPRI;

		if (prty == P_RENYOU_J || prty == P_RENYOU_M ) {
			while (clrec->cllen != 1 && cl2rec &&
			       (Int)cl2rec->cllen == keeplen ) {
				if (cl2rec->kubun == K_DOUSHI) {
					prty = 6;
					goto finish;
				}
				else if (cl2rec->jnode->hinsi == SETUBI_1 ||
					 cl2rec->jnode->hinsi == SETUBI_2) {
					prty = 7;
					goto finish;
				}
				cl2rec = cl2rec->clsort;
			}
			prty = 4;
		}
	}


finish:
	cl2rec = maxclptr;

	if (hinsi1 == SUUSI) prty -= 4;

	if (cl2rec -> jnode -> hinsi == SUUSI) prty -= 3;

	if (cl2rec->jnode->class == C_DICT &&
	    cl2rec->jnode->jlen == 1 && (cl2rec->kubun != K_DOUSHI)) {
		prty -= 2;
	}

	if ((!istaigen(clrec->right)) && (cl2rec->jnode->hinsi == SETUBI_2)) {
		prty -= 4;
	}

	prty += 20 - clrec->cllen + clrec->jnode->jlen + clrec->gobiln;
	return(prty + prev);
}


Void	pritiny()
{
	CLREC	*clrec;
	Int	keeplen;
	int	i, prty = 0;

	clrec = selcl;
	keeplen = clrec -> cllen;

	if (istaigen(prevclrow)) {
		while (clrec && (Int)clrec -> cllen == keeplen) {
			i = taicnt(prevclgrm, clrec -> jnode -> hinsi);
			if (i > prty) {
				selcl = clrec;
				prty = i;
			}
			clrec = clrec -> clsort;
		}
	}

	else {

		while (clrec && (Int)clrec -> cllen == keeplen) {
			if (clrec -> jnode -> hinsi >= MYOUJI &&
			    clrec -> jnode -> hinsi <= KEN_KU)
				clrec = clrec -> clsort;

			else if (clrec -> jnode -> hinsi == TANKANJI)
				return;
			else {
				selcl = clrec;
				return;
			}
		}
	}
}

