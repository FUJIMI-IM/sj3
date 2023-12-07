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
 * $SonyRCSfile: mk2claus.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:00 $
 */



#include "sj_kcnv.h"
#include "sj_right.h"

Int	terminate(), priority();
Void	mkjiritu(), mkbunsetu(), wakachi(), pritiny();
Void	free_clall(), free_jall();

Static	Int	set2nd(clrec)
CLREC	*clrec;
{

	CLREC	*rec2;

	mkjiritu(DO_IKKATU | DO_CLSTUDY);

	mkbunsetu();

	if (!maxclptr) {
		wakachi();
		return clrec -> cllen;
	}

	for (rec2 = maxclptr ; rec2 ; rec2 = rec2 -> clsort) {
		if (terminate(rec2->right, cnvstart+rec2->cllen) != SHU)
			break;
	}

	return rec2 ? (clrec -> cllen + rec2 -> cllen) : (clrec -> cllen);
}

Void	mk2claus()
{
	Uchar	*keepptr;
	Int	keeplen;
	CLREC	*clrec;
	Int	len;
	Int	save2ln;
	Int	prty;
	Int	maxprty;
	Int	count;
	Int	prty2;

	prty = maxprty = prty2 = 0;

	clt2nd = maxclptr = NULL;
	jrt2nd = maxjptr  = NULL;

	selcl = clt1st;

	if (selcl -> right == R_FZKKGU) {
		pritiny();
		return;
	}

	if (cnvlen == selcl -> cllen) {
		pritiny();
		return;
	}

	keepptr = cnvstart;
	keeplen = cnvlen;

	len = count = 0;
	for (clrec = selcl ; clrec && count < 5 ; clrec = clrec -> clsort) {

		cnvstart = keepptr + clrec -> cllen;
		cnvlen = keeplen - clrec -> cllen;

		if (len != clrec -> cllen) {
			if (clt2nd != maxclptr) {
				free_clall(maxclptr);
				free_jall(maxjptr);
			}

			save2ln = set2nd(clrec);

			count++;
			len = clrec -> cllen;
			prty2 = 0;
		}

		if (Termtbl[clrec -> right] & T_SHUUJO)
			clrec -> cl2len = (Uchar)len;
		else
			clrec -> cl2len = (Uchar)save2ln;

		if (selcl -> cl2len > clrec -> cl2len) continue;

		prty = priority(clrec) - prty2;

		prty2++;

		if (selcl -> cl2len == clrec -> cl2len) {
			if (clrec -> cllen == nextcllen) {
				if (selcl -> cllen != nextcllen)
					;
				else if (prty <= maxprty)
					continue;
			}

			else if (prty <= maxprty)
				continue;
		}

		selcl = clrec;
		maxprty = prty;

		if (clt2nd == maxclptr) continue;

		free_clall(clt2nd);
		free_jall(jrt2nd);
		jrt2nd = maxjptr;
		clt2nd = maxclptr;
	}

	if (clt2nd != maxclptr) {
		free_clall(maxclptr);
	        maxclptr = NULL;
		free_jall(maxjptr);
	        maxjptr = NULL;
	}

	cnvstart = keepptr;
	cnvlen = keeplen;
}

