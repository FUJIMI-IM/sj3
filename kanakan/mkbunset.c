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
 * $SonyRCSfile: mkbunset.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:02 $
 */



#include "sj_kcnv.h"
#include "sj_hinsi.h"

CLREC	*alloc_clrec();
Uchar	TFar	*getstb();
Int	terminate(), setconj();
Void	setclrec(), srchfzk(), memset();

Void	mkbunsetu()
{
	Reg1	JREC	*jrec;
	TypeGram	hinsi;
	Uchar	TFar	*cnj;
	CREC	crec[4];
	Int	count;
	TypeCnct	right;
	Uchar	*next;
	int	i;

	fzkcount = 0;

	maxclptr = NULL;

	jrec = maxjptr;

	while (jrec) {

		if (jrec -> stbofs && (cnj = getstb(jrec -> hinsi)))
			hinsi = StbHinsi(cnj + jrec -> stbofs - 1);
		else
			hinsi = jrec -> hinsi;

		if (right = Connadr[hinsi]) {
			gobilen = 0;
			next = cnvstart + jrec->jlen;

			if (terminate(right, next)) {
				fzk_ka_flg = 0;
				setclrec(jrec, next, right);
			}

			srchfzk(jrec, next, right, 0);
		}

		else if (count = setconj(hinsi, jrec, crec)) {

			for (i = count ; i-- > 0 ; ) {

				right = crec[i].right;
				gobilen = crec[i].len;
				next = cnvstart + jrec->jlen + gobilen;

				if (terminate(right, next)) {
					fzk_ka_flg = 0;
					setclrec(jrec, next, right);
				}

				srchfzk(jrec, next, right, 0);
			}
		}

		jrec = jrec -> jsort;
	}
}


CLREC	*argclrec(len)
Int	len;
{
	Reg1	CLREC	*ptr;
	Reg2	CLREC	*rec;
	Reg3	CLREC	*child;

	rec = alloc_clrec();

	if (!rec) {
		if (!maxclptr) return NULL;

		ptr = NULL;
		rec = maxclptr;
		while (child = rec -> clsort) {
			ptr = rec;
			rec = child;
		}

		if (len <= (Int) rec -> cllen) return NULL;

		if (ptr)
			ptr -> clsort = NULL;
		else
			maxclptr = NULL;
		if (rec -> jnode) (rec -> jnode)--;
	}

	memset((Uchar *)rec, 0, sizeof(*rec));
	rec -> cllen = rec -> cl2len = (Uchar)len;

	if (!maxclptr) {
		maxclptr = rec;
		return rec;
	}

	ptr = maxclptr;

	if (((Int)ptr -> cllen < len) ||
	    (((Int)ptr -> cllen == len) &&
	     (ptr -> jnode -> hinsi == TANKANJI))) {
		rec -> clsort = maxclptr;
		maxclptr = rec;
		return rec;
	}

	while (child = ptr -> clsort) {
		if ((Int)child -> cllen < len ||
		    ((Int)child -> cllen == len &&
		     child->jnode->hinsi == TANKANJI)){
			ptr -> clsort = rec;
			rec -> clsort = child;
			return rec;
		}
		else
			ptr = child;
	}


	ptr -> clsort = rec;
	return rec;
}

