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
 * $SonyRCSfile: cvtdict.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:40 $
 */




#include "sj_euc.h"
#include "sj_kcnv.h"



Int	getkanji(), seldict();
Uchar	TFar	*getstb();
Void	get_askknj();



Void	cvtdict(krec, clrec, flg)
KHREC	*krec;
CLREC	*clrec;
Int	flg;		
{
	TypeDicOfs	ofs;		
	Uchar		*yptr;		
	Int		jlen;		
	Int		stblen;		
	Int		sttlen;		
	Uchar		TFar	*fptr;
	Int		cllen;
	JREC		*jrec;
	Int		i, j;

	
	ofs   = krec -> offs;
	jrec  = clrec -> jnode;
	cllen = clrec -> cllen;

	yptr = inputyomi;

	
	jlen = jrec -> jlen;

	
	if (i = jrec -> sttofs) {
		
		if (i == SETTOU_KIGOU) {
			sttlen = 1;
			fptr = yptr;
		}

		
		else {
			fptr = Settou_ptr(i);
			sttlen = SttYomiLen(fptr);
			fptr++;	
			if (!krec -> sttkj) fptr += 2;
		}

		*kanjitmp++ = *fptr++;
		*kanjitmp++ = *fptr++;
		jlen -= sttlen;
		yptr += sttlen * 2; 		
	}

	
	else
		sttlen = 0;

	
	if (flg) {
		
		(*Cvtnum_func[krec->mode - 1])(ystart + sttlen, yptr, jrec);

		
		yptr += (i = jrec -> numlen) * 2;
		jlen -= i;

		
		if (jrec -> stbofs) {
			*kanjitmp++ = *yptr++;
			*kanjitmp++ = *yptr++;
			jlen--;
		}

		stblen = 0;
	}

	
	else if ((i = jrec -> stbofs) && (fptr = getstb(jrec -> hinsi))) {
		fptr += i - 1;
		stblen = StbYomiLen(fptr);
		jlen -= stblen;
	}

	
	else {
		stblen = 0;
	}

	
	if (ofs) {
		if (seldict(jrec -> dicid)) {
			(*curdict->getdic)(curdict, jrec -> jseg);
			get_askknj();
			kanjitmp +=
				getkanji(yptr, jlen, dicbuf + ofs, kanjitmp);
			yptr += jlen * 2;
		}
		else {
			while (jlen-- > 0) {
				for ( i = 0; i < euc_codesize(*yptr); i++)
				  *kanjitmp++ = yptr[i];
				yptr += euc_codesize(*yptr);
			}
		}
	}

	
	if (stblen) {
		i = StbKnjLen(fptr);	
		fptr += stblen + 2;
		while (i-- > 0) *kanjitmp++ = *fptr++;
		yptr += stblen * 2;
	}

	
	i = cllen - (jrec -> jlen);
	while (i-- > 0) {
		for ( j = 0; j < euc_codesize(*yptr); j++)
		  *kanjitmp++ = yptr[j];
		yptr += euc_codesize(*yptr);
	}
}



Void	cvtminasi(len)
Int	len;		
{
	Uchar	*ptr;		

	ptr = inputyomi;	

	while (len-- > 0) {

		
		if (isknj1(*ptr)) {
			*kanjitmp++ = *ptr++;
			*kanjitmp++ = *ptr++;
		}
		
		else if (*ptr == SS3) {
			*kanjitmp++ = *ptr++;
			*kanjitmp++ = *ptr++;
			*kanjitmp++ = *ptr++;
		} else {
			*kanjitmp++ = *ptr++;
		}
	}
}



Void	cvtwakachi(clrec)
CLREC	*clrec;		
{
	Int	jlen;	
	Uchar	*ym;
	Int	cnt;
	Uchar	ch;

	ym = inputyomi;		

	
	cnt = jlen = clrec -> jnode -> jlen;
	while (cnt-- > 0) {
		if (isknj1(*ym)) {
			
			if (*ym == 0xa4) {
				ch = *++ym;
				if (0xa1 <= ch && ch <= 0xf3) {
					*kanjitmp++ = 0xa5;
				}
				else {
					*kanjitmp++ = 0xa4;
				}
				*kanjitmp++ = ch;
				ym++;
			}
			else {
				*kanjitmp++ = *ym++;
				*kanjitmp++ = *ym++;
			}
		}
		else if (*ym == SS3) {
			*kanjitmp++ = *ym++;
			*kanjitmp++ = *ym++;
			*kanjitmp++ = *ym++;
		} else {
			*kanjitmp++ = *ym++;
		}
	}

	
	cnt = clrec -> cllen - jlen;
	while (cnt-- > 0) {

		
		if (isknj1(*ym)) {
			*kanjitmp++ = *ym++;
			*kanjitmp++ = *ym++;
		}
		
		else if (*ym == SS3) {
			*kanjitmp++ = *ym++;
                        *kanjitmp++ = *ym++;
                        *kanjitmp++ = *ym++;
		} else {
			*kanjitmp++ = *ym++;
		}
	}
}
