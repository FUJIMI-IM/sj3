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
 * $SonyRCSfile: setubi.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:22 $
 */



#include "sj_kcnv.h"
#include "sj_hinsi.h"

JREC	*argjrec();
Void	memcpy();
Int	sstrncmp();

Uchar	TFar	*getstb(hinsi)
TypeGram	hinsi;
{
	if (hinsi < MEISI_1 || hinsi > TIMEI) return NULL;

	return Stbadr(hinsi);
}


Void	setubi(rec, stbtbl)
JREC	*rec;
Uchar	TFar	*stbtbl;
{
	Uchar	TFar	*stb;
	Uchar	*yptr;
	Uchar	TFar	*sptr;
	Int	slen;
	JREC	*new;
	Int	cmp;

	stb = stbtbl--;

	yptr = cnvstart + rec -> jlen;

	for ( ; *stb ; stb = sptr + slen + StbKnjLen(stb)) {

		slen = StbYomiLen(stb);
		sptr = StbYomiTop(stb);

		cmp = sstrncmp(yptr, sptr, slen);

		if (cmp == OVER || cmp == PARTLY) break;

		if ((cmp != MATCH) || isdpnd(*(yptr + slen))) continue;

		if (!(new = argjrec((Int)(rec -> jlen + slen), rec))) continue;

		new -> stbofs = (Uchar)(stb - stbtbl);
		new -> flags  |= StbBakeru(stb) ? JFLAG_KA : 0;
	}
}

