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
 * $SonyRCSfile: fuzoku.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:47 $
 */





#include "sj_kcnv.h"
#include "sj_hinsi.h"
#include "sj_right.h"
#include "sj_yomi.h"



CLREC	*argclrec();
Int	terminate();



Static	Int	fzkstrcmp(yptr, fzkp, saml)
Uchar	*yptr;			
Uchar	TFar	*fzkp;		
Int	*saml;			
{
	Int	asklen, nkrlen;

	asklen = FzkAskLen(fzkp);

	
	if (*saml < asklen) return CONT;

	
	if (*saml > asklen) return OVER;

	
	nkrlen = FzkNkrLen(fzkp);

	
	yptr += asklen;
	fzkp ++;

	
	while (nkrlen--) {
		if (*yptr > *fzkp)
			return CONT;

		else if (*yptr < *fzkp) {
			if (!*yptr) return PARTLY;
			return OVER;
		}

		else{
			yptr++;
			fzkp++;
			(*saml)++;
		}
	}

	return MATCH;
}



Void	setclrec(jrec, yptr, right)
JREC		*jrec;		
Uchar		*yptr;		
TypeCnct	right;		
{
	CLREC		*new;
	TypeGram	hinsi;
	Int		len;

	
	if (!(len = yptr - cnvstart)) return;

	
	if (len > MaxClInputLen) return;

	
	if (!(new = argclrec(len))) return;

	
	new -> jnode    = jrec;
	new -> gobiln   = gobilen;
	new -> right    = right;
	new -> fzk_ka   = fzk_ka_flg;
	(jrec -> count)++;

	
	hinsi = jrec -> hinsi;

	if (hinsi >= DO_SAHEN && hinsi <= SP_KA_ONB)
		new -> kubun = K_DOUSHI;

	else if ((hinsi >= KEIYOUSI_1 && hinsi <= KE_DOUSI_9) ||
		 (hinsi == SP_KEIYOUSI)){
		if(right == R_FZKMEISI)
			new -> kubun = K_TAIGEN;
		else
			new -> kubun = K_YOUGEN;
	}

	else if (hinsi == RENTAISI)
		new -> kubun = K_RENTAISI;

	else if ((hinsi >= FUKUSI_1 && hinsi <= FUKUSI_9) ||
		 (hinsi == SP_FUKUSI))
		new -> kubun = K_FUKUSI;

	else if (hinsi == SETUZOKU || hinsi==KANDOUSI ||
		hinsi == TANKANJI || hinsi==AISATU)
		new -> kubun = K_SONOTA;

	else if (gobilen) {
		if (hinsi >= MEISI_5 && hinsi <= MEISI_7)
			new -> kubun = K_DOUSHI;

		else if (hinsi == D_MEISI_4)
			new -> kubun = K_DOUSHI;

		else if (hinsi == SETUBI_7 || hinsi == SETUBI_8)
			new -> kubun = K_DOUSHI;


		else if (hinsi == D_MEISI_5)
			new -> kubun = K_YOUGEN;

		else if (hinsi == SETUBI_5 || hinsi == SETUBI_6)
			new -> kubun = K_YOUGEN;
		else
			new -> kubun = K_TAIGEN;
	}
	else if (hinsi >= MEISI_5 && hinsi <= MEISI_7) {
		if (right >= R_I && right <= R_ITADAKE)
			new -> kubun = K_DOUSHI;

		else if (right >= R_DESHI && right <= R_TAI)
			new -> kubun = K_DOUSHI;

		else if (right >= R_NAI && right <= R_MEIREI2)
			new -> kubun = K_DOUSHI;

		else if (right == R_TE2 || right == R_TERU)
			new -> kubun = K_DOUSHI;
		else
			new -> kubun = K_TAIGEN;
	}
	else
		new -> kubun = K_TAIGEN;
}

Static	Int	fzkcnct(right, left)
TypeCnct	right;
TypeCnct	left;
{
	Uchar	TFar	*cncttbl;

	if (right && left) {
		cncttbl = Rigtadr(right);
		left--;
		if (cncttbl[left >> 3] << (left & 0x07) & 0x80)
			return TRUE;
	}

	return FALSE;
}

RECURS
Void	srchfzk(jrec, yptr, right, level)
JREC		*jrec;			
Reg1		Uchar	*yptr;		
TypeCnct	right;			
Int		level;			
{
	Uchar	TFar	*fzk;	
	Int	len = 0;	
	Int	cmp;		
	Uchar	*next;		
	FREC	*fzkrec;	
	Int	i;		
	Int	nlen;
	TypeCnct	rt;

	if ((level >= MAXFLVL) && !(Chrtbl[*yptr] & FZK_KGU)) return;
	
	if (terminate(right, yptr) && Chrtbl[*yptr] & FZK_KGU){
		if (!level) fzk_ka_flg = 0;
		yptr++;
		while (Chrtbl[*yptr] & FZK_KGU) yptr++;
		setclrec(jrec, yptr, (TypeCnct)R_FZKKGU);
		return;
	}

	if (right == R_CUT) return;
	
	if (!*yptr) return;
	
	if (!(*yptr >= _XA && *yptr <= _NN)) return;
	
	fzkrec = fzktbl;
	
	for (i = fzkcount ; i ; i--, fzkrec++) {
		
		if (yptr != fzkrec -> yomip) continue;
		
		fzk = fzkrec -> fzkp;
		
		len = FzkAskLen(fzk) + (nlen = FzkNkrLen(fzk)) + 1;
		
		if (!fzkcnct(right, (TypeCnct)FzkLeft(fzk))) continue;
		
		if (!level) fzk_ka_flg = FzkStrKa(fzk);
		
		next = yptr + len;
		
		if (terminate(rt = FzkRight(fzk), next)) {
			setclrec(jrec, next, rt);
		}
		
		srchfzk(jrec, next, rt, level + 1);
	}

	if (len) return;

	
	if (!(fzk = Fzkadr(*yptr - _XA))) return;
	
	yptr++;
	
	for ( ; *fzk != FZKEND ; fzk += nlen + (i ? 4 : 3)) {

		nlen = FzkNkrLen(fzk);	
		i    = FzkHasCode(fzk);	

		cmp = fzkstrcmp(yptr, fzk, &len);

		if (cmp == OVER)
			break;

		else if (cmp == PARTLY)
			break;

		else if ((cmp != MATCH) || isdpnd(*(yptr + len)))
			continue;

		
		fzktbl[fzkcount].yomip   = yptr - 1; 
		fzktbl[fzkcount].fzkp    = fzk;
		fzkcount++;

		if (!fzkcnct(right, (TypeCnct)FzkLeft(fzk))) continue;

		if (!level) fzk_ka_flg = FzkStrKa(fzk);
		
		next = yptr + len;
		
		if (terminate(rt = FzkRight(fzk), next)) {
			setclrec(jrec, next, rt);
		}
		
		srchfzk(jrec, next, rt, level + 1);
	}
}
