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
 * $SonyRCSfile: setkouho.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:20 $
 *
 * $Id$
 */


#include "sj_euc.h"
#include "sj_kcnv.h"
#include "sj_hinsi.h"
#include "kanakan.h"

static int hiraknj(unsigned char *p);


void
setkouho(CLREC* clrec, TypeDicOfs offs, int mode)
{
	JREC	*jrec;
	KHREC	*kptr;		
	unsigned char	*fptr;
	int	flg;
	int	i;

	
	if (khcount >= MaxKouhoNumber) return;

	jrec = clrec -> jnode;

	
	kptr = &kouhotbl[khcount++];

	
	if (mode && !offs) nkhcount++;

	kptr -> clrec  = clrec;			
	kptr -> offs   = offs;			
	kptr -> rank   = 0;			
	kptr -> styno  = 0;			
	kptr -> mode   = (unsigned char)mode;

	
	if (jrec -> flags & JFLAG_KA) {
		kptr -> ka_fg = 1;
		kptr -> ka_kj = 1;
		flg = 1;
	}
	
	else if (clrec -> fzk_ka) {
		kptr -> ka_fg = 1;
		kptr -> ka_kj = 0;
		flg = 2;
	}
	
	else {
		kptr -> ka_fg = 0;
		kptr -> ka_kj = 0;
		flg = 0;
	}

	
	kptr -> sttfg = kptr -> sttkj = 0;

	
	if (!(fptr = Settou_ptr(jrec -> sttofs))) return;

	
	if (!SttHiraKnj(fptr)) return;

	
	kptr -> sttfg = 1;
	kptr -> sttkj = 0;

	
	i = hiraknj(dicbuf + offs);
	if (offs > 1 && (i == 2 || i == 3)) return;

	

	
	if (khcount >= MaxKouhoNumber) return;

	
	kptr++;
	khcount++;

	
	if (mode && !offs) nkhcount++;

	kptr -> clrec  = clrec;			
	kptr -> offs   = offs;			
	kptr -> rank   = 0;			
	kptr -> styno  = 0;			
	kptr -> mode   = (unsigned char)mode;

	kptr -> sttfg  = 1;
	kptr -> sttkj  = 1;

	
	switch (flg) {
	case 1:		kptr -> ka_fg = kptr -> ka_kj = 1; break;
	case 2:		kptr -> ka_fg = 1; kptr -> ka_kj = 0; break;
	default:	kptr -> ka_fg = kptr -> ka_kj = 0; break;
	}
}


void
ph_setkouho(CLREC* clrec, TypeDicOfs offs, STDYIN* sptr)
{
	JREC	*jrec;
	unsigned char	*p;

	
	if (sptr) {
		stdytop = sptr;
		kouhotbl[0].styno = sptr -> styno;	
		kouhotbl[0].sttkj = sptr -> sttkj;
	}
	
	else {
		kouhotbl[0].styno = -1;			
		kouhotbl[0].sttkj = 0;			
	}

	
	kouhotbl[0].clrec = clrec;		
	kouhotbl[0].offs  = offs;		
	kouhotbl[0].rank  = 0;			

	
	kouhotbl[0].mode = sel_sjmode(jrec = clrec -> jnode);

	
	kouhotbl[0].sttfg = (unsigned char)
		((p = Settou_ptr(jrec->sttofs)) ? (SttHiraKnj(p)?1:0) : 0);

	
	if (jrec -> flags & JFLAG_KA) {
		kouhotbl[0].ka_fg =
		kouhotbl[0].ka_kj = 1;
	}
	
	else if (clrec -> fzk_ka) {
		kouhotbl[0].ka_fg = 1;
		kouhotbl[0].ka_kj = 0;
	}
	
	else {
		kouhotbl[0].ka_fg =
		kouhotbl[0].ka_kj = 0;
	}

	khcount = 1;		
}


static int
hiraknj(unsigned char *p)
{
	char	flg = TRUE;
	int	i;
	int	result;

	do {
		if (p[codesize(*p)] == KanjiStrEnd) flg = FALSE;
		switch(*p & KanjiModeMask) {
		      case ZenHiraAssyuku:
			result = hiraknj_hask(p, &i); break;
			
		      case ZenKataAssyuku:
			result = hiraknj_kask(p, &i); break;
			
		      case OffsetAssyuku:
                        result = hiraknj_ofs(p, &i); break;
			
		      case AiAttribute:
			result = hiraknj_atrb(p, &i); break;
		
		      case LeadingHankaku:
			result = hiraknj_norm(p, &i); break;

		      case KanjiAssyuku:
			result = hiraknj_knj(p, &i); break;			

		      default:
			result = hiraknj_hira(p, &i);
		}

		if (result) return result;

		p += i; 
	} while (flg);

	return 0;
}


int
hiraknj_atrb(unsigned char* p, int* len)
{
	*len = 2;
	return 0;
}


int
hiraknj_ofs(unsigned char* p, int* len)
{
	*len = 2;
        return hiraknj(dicbuf + ((*p & KanjiCodeMask) << 8) + *(p + 1));
}


int
hiraknj_knj(unsigned char* p, int* len)
{
	*len = 1;
	return hiraknj(askknj[*p & KnjAssyukuMask]);
}


int
hiraknj_hask(unsigned char* p, int* len)
{
	*len = 1;
	return 2;
}


int
hiraknj_kask(unsigned char* p, int* len)
{
	*len = 1;
	return 3;
}


int
hiraknj_norm(unsigned char* p, int* len)
{
	*len = codesize(*p);
	return 1;
}


int
hiraknj_hira(unsigned char* p, int* len)
{
	unsigned char	ch;

	*len = codesize(*p);
	ch = *(p + 1);

	switch (*p) {
	case 0x24:
		if (ch >= 0x21 && ch <= 0x73) return 2;
		return 1;

	case 0x25:
		if (ch >= 0x21 && ch <= 0x76) return 3;
		return 1;

	default:
		break;
	}
	return 1;
}
