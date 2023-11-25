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
 * $SonyRCSfile: cl2knj.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:31 $
 */





#include "sj_kcnv.h"



Int	sj2cd_chr(), hzstrlen(), sstrlen();
Void	sstrncpy(), mkjiritu(), mkbunsetu(), wakachi();
Void	mkkouho(), getrank(), cvtclknj(), freework();
Void	memset();



Static  CLREC	*nextrecblk(), *prevrecblk();
Static  Void	initkbuf();



Int	cl2knj(yomi, len, kouho)
Uchar	*yomi;			
Reg4	Int	len;		
Uchar	*kouho;			
{
	Reg1	Uchar	*ptr1;
	Reg2	Uchar	*ptr2;
	Reg3	Int	i;

	
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

	
	sstrncpy(yomi, orgyomi, (Int)(ptr1 - yomi));

	
	if (!hyomi[0]) return 0;

	
	freework();

	
	inputyomi = orgyomi;
	cnvstart = ystart = hyomi;
	cnvlen = sstrlen(hyomi);

	
	mkjiritu(0);

	
	mkbunsetu();

	
	if (!maxclptr) wakachi();

	
	jrt1st = maxjptr;
	clt1st = maxclptr;

	
	mkkouho();

	
	getrank();

	
	selectid = 1;

	
	cvtclknj();

	
	return hzstrlen(inputyomi, (Int)clt1st -> cllen);
}



Int	nextcl(kouho, mode)
Uchar	*kouho;
Int	mode;		
			
{
	CLREC	*clptr;

	
	initkbuf(kouho);

	
	if (!khcount) return 0;

	
	
	if (!mode && (selectid < khcount))
		selectid++;	
	
	
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

	
	return hzstrlen(inputyomi, (Int)clt1st -> cllen);
}




Static	CLREC	*nextrecblk()
{
	Reg1	CLREC	*clptr;
	Reg2	Int	keeplen;

	
	keeplen = clt1st -> cllen;

	
	clptr =  clt1st -> clsort;

	
	while (clptr && (Int)clptr -> cllen == keeplen)
		clptr = clptr -> clsort;

	
	return clptr;
}




Int	prevcl(kouho, mode)
Uchar	*kouho;
Int	mode;		
			
{
	CLREC	*clptr;

	
	initkbuf(kouho);

	
	if (!khcount) return 0;

	 
	if (!mode && (selectid > 1)) 
		selectid--;

	
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

	
	return hzstrlen(inputyomi, (Int)clt1st -> cllen);
}



Static	CLREC	*prevrecblk()
{
	Reg1	CLREC	*clptr;
	CLREC	*keepptr;
	Reg2	Int	keeplen;
	Int	cllen;

	
	clptr = maxclptr;

	
	
	if ((cllen = clt1st -> cllen) == clptr -> cllen)
		return NULL;

	
	while ((Int)clptr -> cllen > cllen) {
		
		keeplen = clptr -> cllen;

		
		keepptr = clptr;

		
		while ((Int)clptr -> cllen == keeplen)
			clptr = clptr -> clsort;
	}

	
	return keepptr;
}



Static	Void	initkbuf(kouho)
Uchar	*kouho;
{
	kanjitmp = kouho;
	memset(kanjitmp, 0, sizeof(STDYOUT) + 1);
}



Int	selectnum()
{
	return selectid;
}
