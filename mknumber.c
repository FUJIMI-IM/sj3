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
 * $SonyRCSfile: mknumber.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:05 $
 *
 * $Id$
 */


#include <sys/types.h>
#include "sj_kcnv.h"
#include "sj_yomi.h"
#include "sj_suuji.h"
#include "kanakan.h"


static int
word2char(unsigned short wd, unsigned char* tbl, int keta, int flg)
{
	int	mask;
	int	num;
	unsigned char*	src;

	for (mask = 0x1000 ; mask ; mask >>= 4) {
		
		switch (num = (unsigned int)wd / mask) {
		case 0:
			
			break;

		case _Num0:
			
			if (!flg) break;

		default:
			src = tbl + (num - _Num0) * 2;
			*kanjitmp++ = *src++;
			*kanjitmp++ = *src;
			flg = TRUE;

			if (keta && !(keta % 3)) {
				*kanjitmp++ = CommaHigh;
				*kanjitmp++ = CommaLow;
			}
		}
		if (keta) keta--;

		
		wd &= (mask - 1);
	}

	return flg;
}


static void
words2num(unsigned short* wd, unsigned char* tbl, int flgc)
{
	int	i;
	int	keta;
	int	flg;
	unsigned char	*kp = kanjitmp;

	if (flgc) {
		keta = NumKetaLength - 1;
		flg = FALSE;
	}
	else {
		keta = 0;
		flg = TRUE;
	}

	for (i = NumWordBuf ; i-- > 0 ; ) {
		flg = word2char(*(wd + i), tbl, keta, flg);
		if (keta) keta -= 4;
	}

	
	if (kp == kanjitmp) {
		
		*kanjitmp++ = *tbl++;
		*kanjitmp++ = *tbl;
	}
}


void
num_type00(unsigned char* s1, unsigned char* s2, JREC* jrec)
{
	unsigned short	num[NumWordBuf];

	
	setwdnum(s1, (int)jrec -> numlen, num);

	
	words2num(num, Num1tbl, FALSE);
}


void
num_type01(unsigned char* s1, unsigned char* s2, JREC* jrec)
{
	unsigned short	num[NumWordBuf];

	
	setwdnum(s1, (int)jrec -> numlen, num);

	
	words2num(num, Num1tbl, FALSE);
}


void
num_type02(unsigned char* s1, unsigned char* s2, JREC* jrec)
{
	unsigned short	num[NumWordBuf];

	
	setwdnum(s1, (int)jrec -> numlen, num);

	
	words2num(num, Num1tbl, TRUE);
}


void
num_type03(unsigned char* s1, unsigned char* s2, JREC* jrec)
{
	unsigned short	num[NumWordBuf];

	
	setwdnum(s1, (int)jrec -> numlen, num);

	
	words2num(num, Num2tbl, FALSE);
}


void
num_type04(unsigned char* s1, unsigned char* s2, JREC* jrec)
{
	unsigned short	num[NumWordBuf];

	
	setwdnum(s1, (int)jrec -> numlen, num);

	
	words2num(num, Num2tbl, FALSE);
}


void
num_type05(unsigned char* s1, unsigned char* s2, JREC* jrec)
{
	unsigned short	num[NumWordBuf];

	
	setwdnum(s1, (int)jrec -> numlen, num);

	
	words2num(num, Num2tbl, TRUE);
}


void
num_type06(unsigned char* s1, unsigned char* s2, JREC* jrec)
{
	unsigned short	num[NumWordBuf];

	
	setwdnum(s1, (int)jrec -> numlen, num);

	
	words2num(num, Num3tbl, FALSE);
}


void
num_type07(unsigned char* s1, unsigned char* s2, JREC* jrec)
{
	unsigned short	num[NumWordBuf];

	
	setwdnum(s1, (int)jrec -> numlen, num);

	
	words2num(num, Num3tbl, FALSE);
}


static void
kan_num(unsigned short* wd, unsigned char* tbl1, unsigned char* tbl2)
{
	int	ii;
	unsigned short	tmp;
	int	mask;
	int	num;
	int	flg;
	unsigned char*	kurai1;
	unsigned char*	kurai2;
	unsigned char*	src;
	unsigned char*	keep = kanjitmp;

	
	kurai2 = Num6tbl - 2;
	mask = 0;
	wd += NumWordBuf - 1;
	flg = FALSE;

	
	for (ii = NumKetaLength ; ii-- > 0 ; ) {
		
		if (!mask) {
			
			if (flg) {
				*kanjitmp++ = *kurai2++;
				*kanjitmp++ = *kurai2++;
			}
			else
				kurai2 += 2;

			mask = 0x1000;
			tmp = *wd--;
			flg = FALSE;
			kurai1 = tbl2;
		}

		
		if ((num = (unsigned int)tmp / mask) > _Num0) {
			
			if ((num != _Num1) || !*kurai1) {
				src = tbl1 + (num - _Num0) * 2;
				*kanjitmp++ = *src++;
				*kanjitmp++ = *src;
			}

			
			if (*kurai1) {
				*kanjitmp++ = *kurai1++;
				*kanjitmp++ = *kurai1++;
			}
			flg = TRUE;
		}
		
		else
			kurai1 += 2;

		
		tmp &= (mask - 1);
		mask >>= 4;
	}

	
	if (kanjitmp == keep) {
		*kanjitmp++ = *tbl1++;
		*kanjitmp++ = *tbl1;
	}
}


void
num_type08(unsigned char* s1, unsigned char* s2, JREC* jrec)
{
	unsigned short	num[NumWordBuf];

	
	setwdnum(s1, (int)jrec -> numlen, num);

	
	kan_num(num, Num2tbl, Num4tbl);
}


void
num_type09(unsigned char* s1, unsigned char* s2, JREC* jrec)
{
	unsigned short	num[NumWordBuf];

	
	setwdnum(s1, (int)jrec -> numlen, num);

	
	kan_num(num, Num3tbl, Num5tbl);
}


void
num_type10(unsigned char* s1, unsigned char* s2, JREC* jrec)
{
	int	i;

	
	for (i = jrec -> numlen ; i > 0 ; i--) {
		
		if (Chrtbl[*s1++] & NUMBER) {
			*kanjitmp++ = *s2++;
			*kanjitmp++ = *s2++;
		}
		
		else
			s2 += euc_codesize(*s2);
	}
}


void
num_type11(unsigned char* s1, unsigned char* s2, JREC* jrec)
{
	int	i;
	int	j;

	
	i = j = jrec -> numlen;
	if (!(Chrtbl[*(s1 + j - 4)] & NUMBER)) j -= (j - 1) / 4;

	
	for ( ; i > 0 ; i--) {
		
		if (Chrtbl[*s1++] & NUMBER) {
			*kanjitmp++ = *s2++;
			*kanjitmp++ = *s2++;
			if (--j > 0) {
				if ((j % 3) == 0) {
					*kanjitmp++ = CommaHigh;
					*kanjitmp++ = CommaLow;
				}
			}
		}
		
		else
			s2 += euc_codesize(*s2);
	}
}


void
num_type12(unsigned char* s1, unsigned char* s2, JREC* jrec)
{
	int	i;
	int	j;

	
	for (i = jrec -> numlen ; i > 0 ; i--) {
		
		if (Chrtbl[j = *s1++] & NUMBER) {
			j = (j - N_0) * 2;
			*kanjitmp++ = Num2tbl[j];
			*kanjitmp++ = Num2tbl[j + 1];
		}
	}
}


static void
num_kurai(unsigned char* p, int len, unsigned char* tbl)
{
	int	i;
	int	j;
	unsigned char	tmp;

	for (i = 0 ; i < len ; i++) {
		tmp = *p++;

		*kanjitmp++ = tbl[j = ((tmp & 0x0f) - _Num0) * 2];
		*kanjitmp++ = tbl[j + 1];
		if ((j = ((tmp >> 4) & 3)) != 0) {
			*kanjitmp++ = Num4tbl[j = (3 - j) * 2];
			*kanjitmp++ = Num4tbl[j + 1];
		}
		if ((j = ((tmp >> 6) & 3)) != 0) {
			*kanjitmp++ = Num6tbl[j = (3 - j) * 2];
			*kanjitmp++ = Num6tbl[j + 1];
		}
	}
}


void
num_type13(unsigned char* s1, unsigned char* s2, JREC* jrec)
{
	unsigned char	num[NumKetaLength];
	int	len;

	
	len = setucnum(s1, (int)jrec -> numlen, num);

	
	num_kurai(num, len, Num1tbl);
}


void
num_type14(unsigned char* s1, unsigned char* s2, JREC* jrec)
{
	unsigned char	num[NumKetaLength];
	int	len;

	
	len = setucnum(s1, (int)jrec -> numlen, num);

	
	num_kurai(num, len, Num2tbl);
}
