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
 * $SonyRCSfile: srchdict.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:26 $
 */



#include "sj_kcnv.h"

Uchar	*get_idxptr();
Int	yomicmp(), sstrlen();
Void	get_askknj();
TypeDicSeg	srchidx();

Int	yomicmp(ptr1, ptr2, saml)
Reg1	Uchar	*ptr1;
Reg2	Uchar	*ptr2;
Uchar	*saml;
{
	Int	i, j;
	Int	same;

	same = *saml;

	i = getplen(ptr2);
	j = getnlen(ptr2);

	if (j == 0) {
		ptr2 = get_idxptr(prevseg);

		for (i = same ; i ; i--)
			if (*ptr1++ != *ptr2++) return OVER;

		j = sstrlen(ptr2);
	}

	else if (i > same)
		return CONT;

	else {
		if (i < same) same = i;
		ptr1 += same;
		ptr2 += DouonBlkSizeNumber;
	}

	while (j-- > 0) {
		if (*ptr1 > *ptr2) {
			*saml = same;
			return CONT;
		}
		else if (*ptr1++ < *ptr2++) {
			return OVER;
		}
		same++;
	}

	*saml = same;
	return MATCH;
}


Uchar	*srchdict(tagp)
Uchar	*tagp;
{
	TypeDicSeg	segno;
	Int		cmp;
	Int		maxlen;

	maxlen = (cnvlen > MaxWdYomiLen) ? MaxWdYomiLen : cnvlen;

	while ((int)dicinl <= maxlen) {


		segno = srchidx(prevseg, (Int)dicinl);

		(*curdict->getdic)(curdict, segno);

		if (prevseg != segno) {

			prevseg = segno;

			tagp = dicbuf;
		}

		else {

			tagp = getntag(tagp);

			if (segend(tagp)) {

				prevseg = ++segno;

				if (segno >= curdict->segunit) return NULL;

				(*curdict->getdic)(curdict, segno);

				tagp = dicbuf;
			}
		}

		if (tagp == dicbuf) {
			get_askknj();
			tagp += *tagp + 1;
		}

		while (!segend(tagp)) {

			cmp = yomicmp(cnvstart, tagp, &dicsaml);

			if ((int)dicsaml > maxlen) return NULL;

			if (cmp == OVER)
				return NULL;

			else if (cmp == MATCH) {

				if (isdpnd(*(cnvstart + dicsaml))) {
					break;
				}

				else {
					dicinl = dicsaml + 1;
					return tagp;
				}
			}
			tagp = getntag(tagp);
		}
		dicinl = dicsaml + 1;
	}

	return NULL;
}

