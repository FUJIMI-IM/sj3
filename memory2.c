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
 * $SonyRCSfile: memory.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:58 $
 */



#include "sj_kcnv.h"

Void	free_jrec(), free_clrec();

JREC	*free_jlst(p)
Reg1	JREC	*p;
{
	JREC	*jpk;
	JREC	*jp;
	JREC	*jn;

	jp = NULL;

	for ( ; p ; p = jn) {

		jn = p -> jsort;

		if (p -> count) {
			if (jp)
				jp -> jsort = p;
			else
				jpk = p;
			jp = p;
		}

		else {
			free_jrec(p);
		}
	}

	if (jp) jp -> jsort = NULL;

	return jpk;
}

CLREC	*free_clst(p, l)
Reg1	CLREC	*p;
Int	l;
{
	CLREC	*cpk;
	CLREC	*cp;
	CLREC	*cn;
	Int	i = FALSE;

	cp = cpk = NULL;

	for ( ; p ; p = cn) {

		cn = p -> clsort;

		if ((Int)p -> cllen == l) {
			if (p == selcl) {
				if (cp) {
					p -> clsort = cpk;
					cpk = p;
				}
				else
					cpk = cp = p;
			}
			else {
				if (cp)
					cp -> clsort = p;
				else
					cpk = p;
				cp = p;
			}
		}

		else {
			(p -> jnode -> count)--;

			free_clrec(p);
		}
	}

	if (cp) cp -> clsort = NULL;

	return cpk;
}

Void	free_clall(p)
Reg1	CLREC	*p;
{
	CLREC	*next;

	while (p) {
		(p -> jnode -> count)--;
		next = p -> clsort;
		free_clrec(p);
		p = next;
	}
}

Void	free_jall(p)
Reg1	JREC	*p;
{
	JREC	*next;

	while (p) {
		next = p -> jsort;
		free_jrec(p);
		p = next;
	}
}

Void	freework()
{
	free_clall(maxclptr);
	clt1st = maxclptr = NULL;

	free_jall(maxjptr);
	jrt1st = maxjptr = NULL;
}

