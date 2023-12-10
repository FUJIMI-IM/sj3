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
 * $SonyRCSfile: offset.c,v $  
 * $SonyRevision: 1.2 $ 
 * $SonyDate: 1994/08/17 01:50:26 $
 */




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "sj_struct.h"
#include "dicttool.h"

extern	OffsetRec *ofsrec[];		
extern	int	ofsrec_num;		



static OffsetRec*
makeoffset(unsigned char* ptr, int len, int ofs)
{
	OffsetRec *orec;
	unsigned char	*p;

	
	orec = (OffsetRec *)Malloc(sizeof(OffsetRec));

	
	p = (unsigned char *)Malloc(len);
	memcpy(p, ptr, len);

	
	orec -> kptr   = p;
	orec -> klen   = len;
	if (ofs)
		orec -> offset = ofs;
	else
		orec -> offset = ofsrec_num + 1;

	return orec;
}


void
set_ofsrec(unsigned char* ptr, int len, int ofs)
{
	OffsetRec *orec;
	int	low, high, mid;
	int	i;

	
	if (ofsrec_num <= 0) {
		orec = makeoffset(ptr, len, ofs);
		ofsrec[0]= orec;
		ofsrec_num++;

		return;
	}

	
	low = 0; high = ofsrec_num - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		i = string_cmp(ofsrec[mid]->kptr, ofsrec[mid]->klen, ptr, len);
		if (i > 0)
			high = mid - 1;
		else if (i < 0)
			low = ++mid;
		else
			break;
	}

	
	if (i) {
		if (ofsrec_num >= MaxOffsetNumber) {
			fprintf(stderr, "\245\252\245\325\245\273\245\303\245\310\241\246\245\306\241\274\245\326\245\353\244\254\244\242\244\325\244\354\244\336\244\267\244\277\n");
			exit(1);
		}

		
		orec = makeoffset(ptr, len, ofs);

		
		for (i = ofsrec_num ; i > mid ; i--) ofsrec[i] = ofsrec[i - 1];
		ofsrec[mid] = orec;
		ofsrec_num++;
	}
}


int
isknjexist(unsigned char* knj, int len)
{
	int	low, high, mid;
	int	i;

	
	if (ofsrec_num <= 0) return 0;

	
	low = 0; high = ofsrec_num - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		i = string_cmp(ofsrec[mid]->kptr, ofsrec[mid]->klen, knj, len);
		if (i > 0)
			high = mid - 1;
		else if (i < 0)
			low = ++mid;
		else
			return ofsrec[mid] -> offset;
	}

	return 0;
}


void
clear_ofsrec(void)
{
	int	i;

	for (i = 0 ; i < ofsrec_num ; i++) {
		Free(ofsrec[i] -> kptr);
		Free(ofsrec[i]);
	}

	ofsrec_num = 0;
}



OffsetRec*
real_ofsrec(unsigned char* ptr)
{
	int	ofs;
	int	i;
	OffsetRec **p;

	
        ofs = (*ptr++ & KnjAssyukuMask);
        ofs <<= 8;
        ofs += *ptr;

	
	for (i = ofsrec_num, p = ofsrec ; i-- > 0 ; p++)
		if ((*p) -> offset == ofs)
			return *p;

	
	fprintf(stderr, "real_ofsrec() \244\307\260\333\276\357\244\312\276\365\302\326 %x\n", ofs);
	exit(1);
}
