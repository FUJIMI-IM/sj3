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
 * $SonyRCSfile: wakachi.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:43 $
 */



#include "sj_kcnv.h"
#include "sj_right.h"
#include "sj_hinsi.h"

JREC	*argjrec();
CLREC	*argclrec();
Int	terminate(), setconj();
Void	setclrec(), srchfzk();

Void	wakachi()
{
	JREC		*jrec;
	CLREC		*clrec;

	CREC		crec[4];
	Int		count;
	Uchar		*next;
	TypeCnct	right;
	Int		i;
	Int		maxlen;

	free_jall(maxjptr);
	maxjptr = NULL;

	maxlen = (cnvlen > MaxClInputLen) ? MaxClInputLen : cnvlen;

	next = cnvstart;
	if (Chrtbl[*next] & (TAI_KGU | FZK_KGU)) {

		while (*next && (Chrtbl[*next] & (TAI_KGU | FZK_KGU)))
			next++;

		if ((i = next - cnvstart) > maxlen) {
			next = cnvstart + maxlen;
			i = maxlen;
		}

		if (!(jrec = argjrec(i, (JREC *)NULL))) return;

		jrec -> class  = C_WAKACHI;
		jrec -> hinsi  = D_MEISI_1;

		if (Chrtbl[*(next - 1)] & TAI_KGU)
			srchfzk(jrec, next, R_DAIMEISI, 0);
	}

	else {

		if (!(jrec = argjrec(0, (JREC *)NULL))) return;
		jrec -> class  = C_WAKACHI;
		jrec -> hinsi  = MEISI_6;

		while ((Int)jrec -> jlen < maxlen) {

			count = setconj((TypeGram)MEISI_6, jrec, crec);

			for (i = count ; i-- > 0 ; ) {

				right = crec[i].right;
				gobilen = crec[i].len;
				next = cnvstart + jrec->jlen + gobilen;

				if (gobilen) {
					if (terminate(right, next)) {
						fzk_ka_flg = 0;
						setclrec(jrec, next, right);
					}
				}
				else if ((Chrtbl[*next] & NUMBER)
						&& (jrec -> jlen)) {
					fzk_ka_flg = 0;
					setclrec(jrec, next, right);
					return;
				}

				srchfzk(jrec, next, right, 0);
			}

			if (maxclptr) break;

			(jrec -> jlen)++;
		}
	}

	if ((maxclptr) && (Chrtbl[*(cnvstart + jrec -> jlen)] & FZKTOP))
		return;

	if (jrec -> jlen) {
		if (clrec = argclrec((Int)jrec -> jlen)) {
			clrec -> jnode   = jrec;
			clrec -> right   = R_MEISI;
			clrec -> kubun   = K_TAIGEN;
			(jrec -> count)++;
		}
	}
}

