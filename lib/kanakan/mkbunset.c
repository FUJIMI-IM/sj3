/*-
 * SPDX-License-Identifier: MIT-open-group
 *
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
 */



#include <sys/types.h>
#include <string.h>
#include "sj_kcnv.h"
#include "sj_hinsi.h"
#include "kanakan.h"

Int	terminate();
Void	setclrec(), srchfzk();

void
mkbunsetu(void)
{
	JREC	*jrec;
	TypeGram	hinsi;
	u_char	*cnj;
	CREC	crec[4];
	int	count;
	TypeCnct	right;
	u_char	*next;
	int	i;

	fzkcount = 0;

	maxclptr = NULL;

	jrec = maxjptr;

	while (jrec) {

		if (jrec -> stbofs && (cnj = getstb(jrec -> hinsi)))
			hinsi = StbHinsi(cnj + jrec -> stbofs - 1);
		else
			hinsi = jrec -> hinsi;

		if ((right = Connadr[hinsi]) != '\0') {
			gobilen = 0;
			next = cnvstart + jrec->jlen;

			if (terminate(right, next)) {
				fzk_ka_flg = 0;
				setclrec(jrec, next, right);
			}

			srchfzk(jrec, next, right, 0);
		}

		else if ((count = setconj(hinsi, jrec, crec)) != 0) {

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


CLREC*
argclrec(int len)
{
	CLREC	*ptr;
	CLREC	*rec;
	CLREC	*child;

	rec = alloc_clrec();

	if (!rec) {
		if (maxclptr == NULL) return NULL;

		ptr = NULL;
		rec = maxclptr;
		while ((child = rec -> clsort) != NULL) {
			ptr = rec;
			rec = child;
		}

		if (len <= (int) rec -> cllen) return NULL;

		if (ptr)
			ptr -> clsort = NULL;
		else
			maxclptr = NULL;
		if (rec -> jnode) (rec -> jnode)--;
	}

	memset((u_char*)rec, 0, sizeof(*rec));
	rec -> cllen = rec -> cl2len = (u_char)len;

	if (!maxclptr) {
		maxclptr = rec;
		return rec;
	}

	ptr = maxclptr;

	if (((int)ptr -> cllen < len) ||
	    (((int)ptr -> cllen == len) &&
	     (ptr -> jnode -> hinsi == TANKANJI))) {
		rec -> clsort = maxclptr;
		maxclptr = rec;
		return rec;
	}

	while ((child = ptr -> clsort) != NULL) {
		if ((int)child -> cllen < len ||
		    ((int)child -> cllen == len &&
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

