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
 * $SonyRCSfile: dict.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:45 $
 */




#include "sj_euc.h"
#include "sj_kcnv.h"

Int codesize();



Void	get_askknj()
{
	Reg1	Uchar	*p;
	Reg3	Uchar	*q;
	Reg2	Uchar	*r;
	Int	i = 0, csize;
	Int	flg;

	
	p = q = dicbuf;

	
	q += *p++;

	
	while (p < q) {
		
		askknj[i] = p;

		
		r = NULL;
		flg = TRUE;
		do {
			csize = codesize(*p);
			if (p[csize] == KanjiStrEnd) flg = FALSE;

			if (!r && (*p != AiAttribute))
				r = p;

			p += csize;
		} while (flg);
		
		p++;
		
		askknj_k[i++] = r;
	}
}



Int	seldict(id)
TypeDicID	id;
{
	DICTL	*dp;

	for (dp = dictlist ; dp ; dp = dp -> next) {
		if ((curdict = dp -> dict) -> dicid == id)
			return TRUE;
	}
	curdict = NULL;

	return FALSE;
}



Uchar	*get_idxptr(seg)
TypeDicSeg	seg;
{
	(*curdict->getofs)(curdict);
	(*curdict->getidx)(curdict);

	return idxbuf + idxofs[seg];
}
