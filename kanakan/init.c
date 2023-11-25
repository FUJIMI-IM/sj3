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
 * $SonyRCSfile: init.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:54 $
 */



#include "sj_kcnv.h"

Void	seg_count(dict)
DICT	*dict;
{
	Uchar	*p;
	Uchar	*q;
	TypeDicSeg	segcnt = 0;

	if (dict-> getidx) {
		(*dict->getidx)(dict);

		p = idxbuf;
		q = p + dict->idxlen;
		while (p < q && *p) {
			segcnt++;
			while (*p++) ;
		}
	}

	dict->segunit = (segcnt == 0) ? 1 : segcnt;
}

Void	mkidxtbl(dict)
DICT	*dict;
{
	Uchar	*p;
	TypeDicSeg	seg;

	if (!dict->getidx || !dict->getofs) return;

	seg = 0;

	(*dict->getidx)(dict);
	(*dict->getofs)(dict);

	idxofs[0] = 0;
	for (p = idxbuf ; p < idxbuf + dict->idxlen && seg < dict->segunit ; ) {
		idxofs[seg++] = p - idxbuf;
		while (*p++) ;
	}
}

Void	initwork()
{
	jrt1st = jrt2nd = maxjptr = (JREC *)0;
	clt1st = clt2nd = maxclptr = (CLREC *)0;
}

