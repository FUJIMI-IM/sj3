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
 *
 * $Id$
 */


#include <sys/types.h>
#include <string.h>
#include "sj_kcnv.h"
#include "kanakan.h"

int	srchhead();

static void dic_mu(int mode);
static void dic_cl(void);

void
mkjiritu(int mode)
{
	u_char	chkind1;
	u_char	chkind2;
	JREC	*jrec;
	u_char	*stb;

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
		if ((stb = getstb(jrec -> hinsi)) != NULL)
			setubi(jrec, stb);
	}
}

static void
dic_mu(int mode)
{
	u_char	*tagp;
	DICTL	*dp;

	for (dp = dictlist ; dp ; dp = dp -> next) {
		curdict = dp -> dict;
		dicinl  = 1;
		dicsaml = 0;
		prevseg = -1;

		while ((tagp = srchdict(tagp)) != NULL) setjrec(tagp, mode);
	}
}

JREC*
argjrec(int len, JREC* rec)
{
	JREC	*ptr;
	JREC	*jrec;
	JREC	*child;

	jrec = alloc_jrec();

	if (!jrec) {
		if (!maxjptr) return NULL;

		ptr = NULL;
		jrec = maxjptr;
		while ((child = jrec -> jsort) != NULL) {
			ptr  = jrec;
			jrec = child;
		}

		if (len <= (int) jrec -> jlen) return NULL;

		if (ptr)
			ptr -> jsort = NULL;
		else
			maxjptr = NULL;
	}

	if (rec)
		memcpy((u_char*)rec, (u_char*)jrec, sizeof(JREC));
	else
		memset((u_char*)jrec, 0, sizeof(*jrec));
	jrec -> jlen = (u_char)len;

	if (!maxjptr) {
		maxjptr = jrec;
		jrec -> jsort = NULL;
		return jrec;
	}

	ptr = maxjptr;

	if ((int)ptr -> jlen < len) {
		jrec -> jsort = maxjptr;
		maxjptr = jrec;
		return jrec;
	}

	while ((child = ptr -> jsort) != NULL) {
		if ((int)child -> jlen < len) break;

		ptr = child;
	}

	ptr -> jsort = jrec;
	jrec -> jsort = child;
	return jrec;
}

static void
dic_cl(void)
{
	u_char	*p;
	u_short	pos;
	int	len;
	int	cmp;

	if (!StudyExist()) return;

	if ((pos = ClStudyIdx[(short) *cnvstart / ClStudyStep]) != (u_short)-1) {

		for (p = ClStudyDict + pos ; !iseocl(p) ; p = ClNextTag(p)) {
			cmp = strncmp(cnvstart, (u_char*)ClYomiPos(p),
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

