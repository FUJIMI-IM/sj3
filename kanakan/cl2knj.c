/*-
 * SPDX-License-Identifier: MIT-open-group
 *
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
 */



#include <string.h>
#include "sj_kcnv.h"
#include "kanakan.h"


Int	sj2cd_chr();
void	cvtclknj(), freework();


static CLREC* nextrecblk(void);
static CLREC* prevrecblk(void);
static void initkbuf(u_char* kouho);


int
cl2knj(u_char* yomi, int len, u_char* kouho)
{
	u_char	*ptr1;
	u_char	*ptr2;
	int	i;

	khcount = nkhcount = 0;

	initkbuf(kouho);

	if (len > MaxClInputLen * 2) len = MaxClInputLen * 2;

	ptr1 = yomi; ptr2 = hyomi;
	for (i = 0 ; i < len ; i++) {
		if (isknj1(*ptr1)) {
			if (++i >= len) break;
		}
		ptr1 += sj2cd_chr(ptr1, ptr2++);
	}
	*ptr2 = 0;

	strlcpy(yomi, orgyomi, (int)(ptr1 - yomi));

	if (!hyomi[0]) return 0;

	freework();

	inputyomi = orgyomi;
	cnvstart = ystart = hyomi;
	cnvlen = strlen(hyomi);

	mkjiritu(0);
	mkbunsetu();
	if (!maxclptr) wakachi();

	jrt1st = maxjptr;
	clt1st = maxclptr;

	mkkouho();
	getrank();

	selectid = 1;

	cvtclknj();

	return hzstrlen(inputyomi, (int)clt1st -> cllen);
}



int
nextcl(u_char* kouho, int mode)
{
	CLREC	*clptr;

	initkbuf(kouho);

	if (!khcount) return 0;

	if (!mode && (selectid < khcount)) {
		selectid++;	
	}
	else if((mode < 2) && (clptr = nextrecblk())) {
		selectid = 1;		
		clt1st = clptr;		
		mkkouho();		
		getrank();		
	}
	else {
		cvtclknj();		
		return 0;
	}

	cvtclknj();			

	return hzstrlen(inputyomi, (int)clt1st -> cllen);
}


static CLREC*
nextrecblk(void)
{
	CLREC	*clptr;
	int	keeplen;

	keeplen = clt1st -> cllen;
	clptr =  clt1st -> clsort;

	while (clptr && (int)clptr -> cllen == keeplen)
		clptr = clptr -> clsort;

	return clptr;
}


int
prevcl(u_char* kouho, int mode)
{
	CLREC	*clptr;

	initkbuf(kouho);

	if (!khcount) return 0;

	if (!mode && (selectid > 1)) {
		selectid--;
	}
	else if ((mode < 2) && (clptr = prevrecblk())) {
		clt1st = clptr;
		mkkouho();		
		getrank();		
		selectid = khcount; 	
	}
	else {
		cvtclknj();		
		return 0;
	}

	cvtclknj();			

	return hzstrlen(inputyomi, (int)clt1st -> cllen);
}


static CLREC*
prevrecblk(void)
{
	CLREC	*clptr;
	CLREC	*keepptr;
	int	keeplen;
	int	cllen;

	clptr = maxclptr;

	if ((cllen = clt1st -> cllen) == clptr -> cllen)
		return NULL;

	while ((int)clptr -> cllen > cllen) {
		keeplen = clptr -> cllen;
		keepptr = clptr;

		while ((int)clptr -> cllen == keeplen)
			clptr = clptr -> clsort;
	}

	return keepptr;
}


static void
initkbuf(u_char* kouho)
{
	kanjitmp = kouho;
	memset(kanjitmp, 0, sizeof(STDYOUT) + 1);
}


int
selectnum(void)
{
	return selectid;
}
