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
 * $SonyRCSfile: mkkouho.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:04 $
 */




#include "sj_euc.h"
#include "sj_kcnv.h"
#include "sj_hinsi.h"
#include "sj_suuji.h"
#include "sj_dict.h"



Uchar	*skipkstr();
Void	setkouho(), setwdnum(), get_askknj();
Int	seldict(), codesize();



Static  Void	getkhtbl(), cl_kanji();
Static  Int	diffknj();
Static  Void	cl_numcmn();
Static  Uchar	*makekan();



Void	mkkouho()
{
	CLREC	*clrec;		
	Int	keeplen;	

	
	khcount = nkhcount = 0;

	
	keeplen = clt1st -> cllen;

	
	clrec = clt1st;

	
	do {
		getkhtbl(clrec);		
		clrec = clrec -> clsort;	
	} while (clrec && (keeplen == clrec -> cllen));
}



Static	Void	getkhtbl(clrec)
CLREC	*clrec;
{
	JREC	*jrec;		

	
	jrec = clrec -> jnode;

	
	switch (jrec -> class) {
	case C_DICT:			
 		 
		cl_kanji(jrec, clrec);
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
		cl_numcmn(jrec, clrec);
		break;

	case C_BUNSETU:
		
		break;

	case C_MINASI:			
	case C_WAKACHI:			
		
		setkouho(clrec, (TypeDicOfs)0, 0);
		break;
	}
}



Static	Void 	cl_kanji(jrec, clrec)
JREC  	*jrec;		
CLREC 	*clrec;		
{
	Uchar	*ptr;		
	Int	kcount = khcount;

	
	if (seldict(jrec -> dicid)) {
		(*curdict->getdic)(curdict, jrec -> jseg);
		ptr = dicbuf + jrec->jofsst + 1;

		get_askknj();

		
		while (*ptr != HinsiBlkTerm) {

			
			if (diffknj(jrec, ptr, kcount)) {
				
				setkouho(clrec, (TypeDicOfs)(ptr - dicbuf), 0);
			}

			
			ptr = skipkstr(ptr);
		}
	}
	else {
		
		setkouho(clrec, (TypeDicOfs)1, 0);
	}
}



Uchar	*makekan_none(s, d, flg)
Uchar	*s;
Uchar	*d;
Int	flg;
{
	return d;
}



Uchar	*makekan_1byte(s, d, flg)
Uchar	*s;
Uchar	*d;
Int	flg;
{
	if ((*s & KanjiModeMask) == ZenHiraAssyuku)
	  *d++ = 0x10 | (*s & KnjAssyukuMask); 
	else 
	  *d++ = 0x90 | (*s & KnjAssyukuMask); 

	if (flg) *d++ = KanjiStrEnd;
	return d;
}



Uchar	*makekan_knj(s, d, flg)
Uchar	*s;
Uchar	*d;
Int	flg;
{
	
	return makekan(askknj[*s & KnjAssyukuMask], d, flg);
}



Uchar	*makekan_ofs(s, d, flg)
Uchar	*s;
Uchar	*d;
Int	flg;
{
	
        return makekan(dicbuf + ((*s & KanjiCodeMask) << 8) + *(s + 1),
                                d, flg);
}



Uchar	*makekan_norm(s, d, flg)
Uchar	*s;
Uchar	*d;
Int	flg;
{
	Reg1 Int  i, csize;
	Reg1 Uchar c;

	if (*s != 0) {
		c = s[1];
		if (c & 0x80)
		  *d++ = SS3;
		*d++ = *s | 0x80;
		*d++ =  c | 0x80;
	} else {
		*d++ = *s;
	}
	if (flg) *d++ = KanjiStrEnd;
		
	return d;
}



Uchar	*makekan_ascii(s, d, flg)
Uchar	*s;
Uchar	*d;
Int	flg;
{
	Reg1 Uchar c;

	c = s[1];
	if (c & 0x80)
	  *d++ = SS2;
	*d++ = c;

	if (flg) *d++ = KanjiStrEnd;

	return d;
}



Static	Uchar	*makekan(s, d, flg)
Uchar	*s;
Uchar	*d;
Int	flg;
{
	Reg1 Int csize;

	for ( ; ; ) {
		csize = codesize(*s);
		if (s[csize] == KanjiStrEnd) {
			switch(*s & KanjiModeMask) {
			      case ZenHiraAssyuku:
                                d = makekan_1byte(s, d, flg); break;
         
			      case ZenKataAssyuku:
                                d = makekan_1byte(s, d, flg); break;
 
			      case OffsetAssyuku:
                                d = makekan_ofs(s, d, flg); break;
 
			      case AiAttribute:
                                d = makekan_none(s, d, flg); break;

			      case LeadingHankaku:
				d = makekan_ascii(s, d, flg); break;
                                        
			      case KanjiAssyuku:
				d = makekan_knj(s, d, flg); break;
				
			      default:
				d = makekan_norm(s, d, flg);
			}
			break;
		}
		else
			switch(*s & KanjiModeMask) {
			      case ZenHiraAssyuku:
                                d = makekan_1byte(s, d, FALSE); break;
         
			      case ZenKataAssyuku:
                                d = makekan_1byte(s, d, FALSE); break;
 
			      case OffsetAssyuku:
                                d = makekan_ofs(s, d, FALSE); break;
 
			      case AiAttribute:
                                d = makekan_none(s, d, FALSE); break;

			      case LeadingHankaku:
				d = makekan_ascii(s, d, FALSE); break;

			      case KanjiAssyuku:
				d = makekan_knj(s, d, FALSE); break;

			      default:
				d = makekan_norm(s, d, FALSE);
			}
		
		s += codesize(*s);
	}

	return d;
}



#define	PEND	2
#define	QEND	1
Static	Int	sameknj(p, plen, q, qlen)
Reg1	Uchar	*p;
Uchar	plen;
Reg2	Uchar	*q;
Uchar	qlen;
{
	Int	i;
	Int	j;
	Int	endf = 0;
	Int     brkf;
	extern Int     euc_codesize();

	do {
		
		if ((i = euc_codesize(*p)) != euc_codesize(*q)) return FALSE;

		
		brkf = 0;
		for (j = 0; j < i; j++) {
			if (p[j] != q[j] ) {
				brkf = 1;
				break;
			}
		}
		if (brkf) break;

		
		if (p[i] == KanjiStrEnd) endf += PEND;
		if (q[i] == KanjiStrEnd) endf += QEND;

		
		p += i;
		q += i;
	} while (!endf);

	switch (endf) {
	
	case (PEND | QEND):
		
		if (plen == qlen) return TRUE;
		break;

	
	case PEND:
		i = plen;
		plen = qlen;
		qlen = i;
		p = q;

	
	case QEND:
		if (p[euc_codesize(*p)] != KanjiStrEnd) break;
		if ((*p & KanjiModeMask) != 0x10 ) break;
		if (qlen != plen - (*p & KanjiCodeMask) - 1) break;
		return TRUE;

	
	default:
		if (q[euc_codesize(*q)] != KanjiStrEnd) break;
		if (p[euc_codesize(*p)] != KanjiStrEnd) break;
		if ((*q & KanjiModeMask) != 0x10 ) break;
		if ((*p & KanjiModeMask) != 0x10 ) break;
		if (plen - (*p & KanjiCodeMask) != qlen - (*q & KanjiCodeMask))
			break;
		return TRUE;
	}

	return FALSE;
}
#undef	PEND
#undef	QEND



Static	Int	diffknj(jrec, ptr, num)
JREC	*jrec;			
Uchar	*ptr;			
Int	num;			
{
	KHREC	*kptr;
	JREC	*jptr;
	Int	i;
	Uchar	kbuf1[MaxWdKanjiLen];
	Uchar	kbuf2[MaxWdKanjiLen];

	
	if (jrec -> hinsi == TANKANJI) return TRUE;

	
	makekan(ptr, kbuf1, TRUE);

	
	for (i = 0, kptr = kouhotbl ; i < num ; i++, kptr++) {
		
		if ((jptr = kptr -> clrec -> jnode) == jrec)
			
			return FALSE;

		
		if (jptr -> hinsi == TANKANJI) continue;

		
		if (jptr -> class != jrec -> class) continue;

		
		if (jptr -> dicid != jrec -> dicid) continue;

		
		if (jptr -> jseg != jrec -> jseg) continue;

		
		if (jptr -> sttofs != jrec -> sttofs) continue;

		
		if (jptr -> stbofs != jrec -> stbofs) continue;

		
		makekan(dicbuf + kptr -> offs, kbuf2, TRUE);

		
		if (sameknj(kbuf1, jrec -> jlen, kbuf2, jptr -> jlen))
			return FALSE;
	}

	
	return TRUE;
}




Static	Int	chrck_numtbl(flg, cond)
Ushort	flg;
Ushort	cond;
{
	Ushort	must;

	if (cond = SelNumCond(cond)) {
		if (must = (cond & SelNumMust)) {
			if ((flg & must) != must) return FALSE;
			cond &= ~must;
		}
		if (cond && !(cond & flg)) return FALSE;
	}

	return TRUE;
}



Int	sel_sjmode(jrec)
JREC	*jrec;
{
	Ushort	i;
	Ushort	TFar	*p;
	Ushort	TFar	*q;

	switch (jrec -> class) {
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
		p = q = Selsjadrs(jrec->class - C_N_ARABIA);
		break;

	default:
		return SelArb;
	}

	
	while ((i = *p++) != SelNumTerm) {
		
		if (chrck_numtbl(jrec -> flags, i)) break;
	}

	if (i == SelNumTerm) i = *q;

	return SelNumFunc(i);
}



Static	Void	cl_numcmn(jrec, clrec)
JREC	*jrec;			
CLREC	*clrec;			
{
	Uchar	*p;
	Int	i;
	Ushort	j;
	Ushort	TFar	*tbl;

	
	p = cnvstart;
	if ((i = jrec -> sttofs) == SETTOU_DAI)		
		p += SttYomiLen(Settou_ptr(i));
	else if (i == SETTOU_KIGOU)			
		p++;

	
	if (jrec -> jofsst) {
		
		i = sel_sjmode(jrec);

		
		if (seldict(jrec -> dicid)) {
			(*curdict->getdic)(curdict, jrec -> jseg);

			
			p = dicbuf + jrec->jofsst + 1;

			
			while (*p != HinsiBlkTerm) {
				
				setkouho(clrec, (TypeDicOfs)(p - dicbuf), i);

				
				p = skipkstr(p);
			}
		}
		else {
			
			setkouho(clrec, (TypeDicOfs)1, i);
		}
	}
	
	else {
		
		tbl = Selsjadrs(jrec->class - C_N_ARABIA);
		while ((j = *tbl++) != SelNumTerm) {
			
			if (!chrck_numtbl(jrec -> flags, j)) continue;

			
			setkouho(clrec, (TypeDicOfs)0, (Int)SelNumFunc(j));
		}
	}
}
