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
 * $SonyRCSfile: selclrec.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:14 $
 */





#include "sj_kcnv.h"



Void	free_clall(), free_jall(), mkjiritu(), mkbunsetu();
Void	wakachi();



Void	selclrec()
{
	Int	len;
	CLREC	*rec;

	
	nextcllen = 0;

	
	len = selcl -> cllen;
	rec = clt1st;
	while (rec) {
		if (len >= (unsigned int) rec -> cllen) break;
		rec = rec -> clsort;
	}

	
	while (rec) {
		if (len > (unsigned int) rec -> cllen) return;
		if (rec -> jnode -> class == C_BUNSETU) break;
		rec = rec -> clsort;
	}
	if (!rec) return;



	
	len = ClYomi1Len(ClStudyDict + rec->jnode->jofsst);

	
	nextcllen =  rec -> cllen - len;

	
	while (rec) {
                
		if (len == rec -> cllen) {
			
			selcl = rec;
			break;
		}
		
		else if (len > (unsigned int) rec -> cllen) {
			rec = NULL;
			break;
		}
		rec = rec -> clsort;
	}

	
	if (rec == NULL) {
		Int	klen;
		Uchar	ch;

		
		free_clall(clt1st);
		free_jall(jrt1st);

		
		klen = cnvlen;
		cnvlen = len;
		ch = *(cnvstart + len);
		*(cnvstart + len) = 0;

		
		mkjiritu(DO_IKKATU);

		
		mkbunsetu();
		if (!maxclptr) wakachi();

		
		jrt1st = maxjptr;
		selcl = clt1st = maxclptr;

		
		cnvlen = klen;
		*(cnvstart + len) = ch;
	}

	
	free_clall(clt2nd); clt2nd = NULL;
	free_jall(jrt2nd); jrt2nd = NULL;
}
