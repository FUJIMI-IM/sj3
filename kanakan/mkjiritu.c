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
 * $SonyRCSfile: mkjiritu.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:03 $
 */



#include "sj_kcnv.h"

Uchar	*srchdict();
Uchar	TFar	*getstb();
Int	srchhead(), sstrncmp();
JREC	*alloc_jrec();
Void	srchnum(), setubi(), setjrec();
Void	setcrec(), memcpy(), memset();

Static Void	dic_mu(), dic_cl();

Void	mkjiritu(mode)
Int	mode;
{
	Uchar	chkind1;
	Uchar	chkind2;
	JREC	*jrec;
	Uchar	TFar	*stb;

	headcode = headlen = 0;

	maxjptr = NULL;

	chkind1 = Chrtbl[*cnvstart];

	if (chkind1 & DICTOP) {
		dic_mu(mode);
	}

	if (mode & DO_CLSTUDY) dic_cl();

	srchnum();

	if (srchhead() && cnvlen != headlen) {
		cnvstart += headlen;
		cnvlen -= headlen;

		chkind2 = Chrtbl[*cnvstart];

		if (chkind2 & DICTOP) {
			dic_mu(mode);
		}

		if (headcode == SETTOU_DAI) {
			srchnum();
		}

		cnvstart -= headlen;
		cnvlen += headlen;
	}

	for (jrec = maxjptr ; jrec ; jrec = jrec -> jsort) {
		if (stb = getstb(jrec -> hinsi))
			setubi(jrec, stb);
	}
}

Static	Void	dic_mu(mode)
Int	mode;
{
	Uchar	*tagp;
	DICTL	*dp;

	for (dp = dictlist ; dp ; dp = dp -> next) {
		curdict = dp -> dict;
		dicinl  = 1;
		dicsaml = 0;
		prevseg = -1;

		while (tagp = srchdict(tagp)) setjrec(tagp, mode);
	}
}

JREC	*argjrec(len, rec)
Int	len;
JREC	*rec;
{
	Reg1	JREC	*ptr;
	Reg2	JREC	*jrec;
	Reg3	JREC	*child;

	jrec = alloc_jrec();

	if (!jrec) {
		if (!maxjptr) return NULL;

		ptr = NULL;
		jrec = maxjptr;
		while (child = jrec -> jsort) {
			ptr  = jrec;
			jrec = child;
		}

		if (len <= (Int) jrec -> jlen) return NULL;

		if (ptr)
			ptr -> jsort = NULL;
		else
			maxjptr = NULL;
	}

	if (rec)
		memcpy((Uchar *)rec, (Uchar *)jrec, sizeof(JREC));
	else
		memset((Uchar *)jrec, 0, sizeof(*jrec));
	jrec -> jlen = (Uchar)len;

	if (!maxjptr) {
		maxjptr = jrec;
		jrec -> jsort = NULL;
		return jrec;
	}

	ptr = maxjptr;

	if ((Int)ptr -> jlen < len) {
		jrec -> jsort = maxjptr;
		maxjptr = jrec;
		return jrec;
	}

	while (child = ptr -> jsort) {
		if ((Int)child -> jlen < len) break;

		ptr = child;
	}

	ptr -> jsort = jrec;
	jrec -> jsort = child;
	return jrec;
}

Static	Void	dic_cl()
{
	Uchar	*p;
	Ushort	pos;
	Int	len;
	Int	cmp;

	if (!StudyExist()) return;

	if ((pos = ClStudyIdx[(Short) *cnvstart / ClStudyStep]) != (Ushort)-1) {

		for (p = ClStudyDict + pos ; !iseocl(p) ; p = ClNextTag(p)) {
			cmp = sstrncmp(cnvstart, (Uchar TFar *)ClYomiPos(p),
					len = ClYomiLen(p));

			if (cmp == MATCH) {
				if (!isdpnd(*(cnvstart + len))) {
					setcrec(p);
				}
			}

			else if (cmp == OVER)
				break;
		}
	}
}

