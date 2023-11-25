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
 * $SonyRCSfile: srchidx.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:29 $
 */



#include "sj_kcnv.h"

Uchar	*get_idxptr();
Int	sstrlen(), istrcmp();

TypeDicSeg	srchidx(low, len)
TypeDicSeg	low;
Int		len;
{
	TypeDicSeg	high;
	TypeDicSeg	mid;
	Int		cmp;
	Uchar		*target;

	if ((high = curdict->segunit - 1) < 1) return 0;

	if (low == (TypeDicSeg)-1) low = 0;

	while (low <= high) {

		mid = (low + high) >> 1;

		target = get_idxptr(mid);

		cmp = istrcmp(cnvstart, target, len, sstrlen(target));

		if (cmp == OVER) {
			high = mid - 1;
		}

		else if(cmp != MATCH){
			low = mid + 1;
		}

		else {
			return mid;
		}
	}

	return (cmp == OVER && mid != DicSegBase) ? mid - 1 : mid;
}

