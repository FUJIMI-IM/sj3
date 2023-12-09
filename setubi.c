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
 *
 * $Id$
 */


#include <string.h>
#include "sj_kcnv.h"
#include "sj_hinsi.h"
#include "kanakan.h"

u_char*
getstb(TypeGram hinsi)
{
	if (hinsi < MEISI_1 || hinsi > TIMEI) return NULL;

	return Stbadr(hinsi);
}


void
setubi(JREC *rec, u_char *stbtbl)
{
	u_char	*stb;
	u_char	*yptr;
	u_char	*sptr;
	int	slen;
	JREC	*new;
	int	cmp;

	stb = stbtbl--;

	yptr = cnvstart + rec -> jlen;

	for ( ; *stb ; stb = sptr + slen + StbKnjLen(stb)) {

		slen = StbYomiLen(stb);
		sptr = StbYomiTop(stb);

		cmp = strncmp(yptr, sptr, slen);

		if (cmp == OVER || cmp == PARTLY) break;

		if ((cmp != MATCH) || isdpnd(*(yptr + slen))) continue;

		if (!(new = argjrec((int)(rec -> jlen + slen), rec))) continue;

		new -> stbofs = (u_char)(stb - stbtbl);
		new -> flags  |= StbBakeru(stb) ? JFLAG_KA : 0;
	}
}

