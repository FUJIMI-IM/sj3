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
 * $SonyRCSfile: getrank.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:50 $
 */





#include "sj_kcnv.h"



STDYIN	*srchstdy();



Static  Void	setstynum(), setnspr(), regetrank();



Void	getrank()
{
	
	trank = nrank = 1;

	
	setstynum();

	
	regetrank();
}



Static	Void	setstynum()
{
	KHREC	*kptr;
	STDYIN	*sptr;
	Int	count;

	
	for (kptr = kouhotbl, count = khcount ; count-- ; kptr++) { 
		
		sptr = srchstdy(kptr -> clrec -> jnode -> jseg, kptr -> offs,
				kptr -> clrec -> jnode -> dicid);

		
		if (!sptr) continue;

		
		setnspr(kptr, sptr);	
	}
}



Static	Void	setnspr(kptr, sptr)
KHREC	*kptr;		
STDYIN	*sptr;		
{
	KHREC		*ptr;
	Int		keepnm;
	TypeStyNum	styno;

 	
	styno = kptr -> styno = sptr -> styno;

 	
	keepnm = trank;

	
	for (ptr = kouhotbl ; ptr < kptr ; ptr++) {

		
		if (styno < ptr -> styno) {
			
			ptr -> rank++;

			
			keepnm--;
		}

		
		else if (styno == ptr -> styno) {

			
			if (kptr -> sttfg && kptr -> ka_fg) {
				
				
				if ((sptr -> sttkj == kptr -> sttkj) &&
				    (sptr -> ka_kj == kptr -> ka_kj)) {
					ptr -> rank++;
					keepnm--;
				}
			}
			
			else if (kptr -> sttfg) {
				
				
				if (sptr -> sttkj == kptr -> sttkj) {
					ptr -> rank++;
					keepnm--;
				}
			}
			
			else if (kptr -> ka_fg) {
				
				
				if (sptr -> ka_kj == kptr -> ka_kj) {
					ptr -> rank++;
					keepnm--;
				}
			}
		}
	}

	
	kptr -> rank = (Uchar)keepnm;
	trank++;

	
	if (keepnm <= nrank) nrank++;

	
	if (sptr -> nmflg && (nrank > keepnm)) nrank = keepnm;
}



Static	Void	regetrank()
{
	KHREC	*kptr;		
	Int	count;
	Int	tmp;

	
	if (nrank < trank) {
		
		tmp = nrank;

		
		trank += nkhcount;
	}
	
	else {
		nkhcount = tmp = 0;
	}

	
	for (count = khcount, kptr = kouhotbl ; count-- ; kptr++) {
		
		if (!(kptr -> rank)) {
			if (tmp && kptr -> mode && !(kptr -> offs)) {
				kptr -> rank = (Uchar)tmp++;
			}
			else
				kptr -> rank = (Uchar)trank++;
		}

		
		else if ((Short)kptr -> rank >= nrank)
			kptr -> rank += (Uchar)nkhcount;
	}
}
