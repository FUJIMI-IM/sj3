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
 * $SonyRCSfile: cvtkanji.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:41 $
 */





#include "sj_kcnv.h"



Int	hzstrlen(), ph_khtbl();
Void	cvtdict(), cvtminasi();
Void	cvtwakachi(), setstyrec(), cvtkouho(), mkknjmax();
Void	ph_setkouho(), mvmemi(), free_clall();
Void	free_jall(), memcpy(), cvtphknj();
Uchar	TFar	*getstb();
STDYIN	*srchstdy();



Void	cvtphknj(num)
Int	num;
{
	Int	i, j;
	CLREC	*clrec;
	CLREC	*clkp;

	if (!kanjilen) goto not_enough_memory;

	khcount = 0;
	clkp = NULL;
	stdytop = NULL;

		clrec = clt1st;
		while (clrec) {
			if (ph_khtbl(clrec)) clkp = clrec;
			clrec = clrec -> clsort;
		}

		
		if (clkp && stdytop) {
			
			if (stdytop -> nmflg)
				ph_setkouho(clkp, (TypeDicOfs)0, (STDYIN *)0);
		}

	kouhotbl[0].rank = 1;

	
	kanjitmp = kanjibuf + 1;

	
	setstyrec(kouhotbl);

	
	cvtkouho(kouhotbl);

	
	i = kanjitmp - kanjibuf;
	if (i < kanjilen) {
		
		kanjilen -= i;

		
                ystart += clt1st -> cllen;

                j = hzstrlen(inputyomi, (Int)clt1st -> cllen);
		kanjibuf[0] = (Uchar)j;

		
		inputyomi += j;

		
		memcpy(kanjibuf, kanjipos, i);
		kanjipos += i;
	}
	else
		kanjilen = 0;

not_enough_memory:

	
	free_clall(clt1st);
	free_jall (jrt1st);
}



Void	cvtkouho(krec)
KHREC	*krec;			
{
	CLREC	*clrec;		

	clrec = krec -> clrec; 	

	
	switch (clrec -> jnode -> class) {
	case C_DICT:			
		cvtdict(krec, clrec, FALSE);
		break;

	case C_N_ARABIA:		
	case C_N_ARACMA:		
	case C_N_KAZU:			
	case C_N_SUUJI:			
	case C_N_NANSUU:		
	case C_N_KANKURA:		
	case C_N_ARAKURA:		
	case C_N_KAZULONG:		
	case C_N_KAZULCMA:		
	case C_N_SUUJILONG:		
		cvtdict(krec, clrec, TRUE);
		break;

	case C_BUNSETU :		
	case C_MINASI  :		
		cvtminasi((Int)clrec -> cllen);
		break;

	case C_WAKACHI :		
		cvtwakachi(clrec);
		break;
	}

	
	*kanjitmp++ = 0;
}



Void	setstyrec(krec)
KHREC	*krec;
{
	JREC	*jrec;
	KHREC	*kptr;
	Int	ii;
	Uchar	TFar	*fptr;
	STDYOUT	stdy;

	jrec  = krec -> clrec -> jnode;

	
	if (krec -> mode && !krec -> offs) {
		
		for (ii = khcount, kptr = kouhotbl ; ii--; kptr++)
			if (kptr -> rank == 1) break;

		
		stdy.stdy1.offset = kptr -> offs;
		stdy.stdy1.seg    = kptr -> clrec -> jnode -> jseg;
		stdy.stdy1.styno  = kptr -> styno;
		stdy.stdy1.dicid  = kptr -> clrec -> jnode -> dicid;
		stdy.stdy1.nmflg  = 1;
	}
	
	else {
		stdy.stdy1.offset = krec -> offs;
		stdy.stdy1.seg    = jrec -> jseg;
		stdy.stdy1.styno  = krec -> styno;
		stdy.stdy1.dicid  = jrec -> dicid;
		stdy.stdy1.nmflg  = 0;
	}
	stdy.stdy1.sttkj  = krec -> sttkj;
	stdy.stdy1.ka_kj  = krec -> ka_kj;
	stdy.hinshi       = jrec -> hinsi;
	stdy.len          = jrec -> jlen;
	stdy.sttfg        = krec -> sttfg;
	stdy.ka_fg        = krec -> ka_fg;

	
	if (jrec -> stbofs && (fptr = getstb(jrec -> hinsi)))
		stdy.len -= StbYomiLen(fptr + jrec -> stbofs - 1);

	
	if (fptr = Settou_ptr(jrec -> sttofs))
		stdy.sttlen = SttYomiLen(fptr);
	else
		stdy.sttlen = 0;

	
	memcpy((Uchar *)&stdy, kanjitmp, sizeof(STDYOUT));
	kanjitmp += sizeof(STDYOUT);
}
