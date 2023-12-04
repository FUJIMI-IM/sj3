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
 * $SonyRCSfile: peepdic.c,v $  
 * $SonyRevision: 1.2 $ 
 * $SonyDate: 1995/07/21 05:22:51 $
 *
 * $Id$
 */


#include "sj_kcnv.h"
#include "sj_yomi.h"
#include "kanakan.h"


Uchar	*skiphblk();

static	void	add_yomi (void);
static	void	cd2sjh_chr (Uchar ch, Uchar *dst);
static	Int	next_douon (void);
static	Int	next_hinsi (void);
static	Int	next_kanji (void);
static	Int	prev_douon (void);
static	Int	prev_hinsi (void);
static	Int	prev_kanji (void);
static	void	set_buf (Uchar *buf);
static	void	set_idxyomi (void);
static	void	set_kanji (void);



Int
getusr (Uchar *buf)
{
	Int nlen;

	peepyomi[0] = peepknj[0] = peepgrm = 0;

	(*curdict->getdic)(curdict, peepidx = DicSegBase);
	get_askknj();

	peepdptr = segtop();

	if (segend(peepdptr)) return 0;

	add_yomi();

	nlen = getnlen(peepdptr);
        peephptr = peepdptr + DouonBlkSizeNumber + nlen;

	peepgrm = *peephptr;

	peepkptr = peephptr + 1;

	set_kanji();

	set_buf(buf);

	return -1;
}



Int
nextusr (Uchar *buf)
{
	(*curdict->getdic)(curdict, peepidx);
	get_askknj();

	if (next_kanji()) {
		set_kanji();
		set_buf(buf);
		return -1;
	}

	return 0;
}



Int
prevusr (Uchar *buf)
{
	(*curdict->getdic)(curdict, peepidx);
	get_askknj();

	if (prev_kanji()) {
		set_kanji();
		set_buf(buf);
		return -1;
	}

	return 0;
}



static	void
set_kanji (void)
{
	Int	len;

	len = getkanji(peepyomi, getnlen(peepdptr) + getplen(peepdptr),
			peepkptr, peepknj);
	*(peepknj + len) = 0;
}



static	void
set_buf (Uchar *buf)
{
	Uchar	*p;
	Int      i, csize;

	for (p = peepyomi ; *p ; ) *buf++ = *p++;
	*buf++ = 0;

	for (p = peepknj ; *p ; ) {
		csize = euc_codesize(*p);
		for (i = 0; i < csize; i++ ) *buf++ = *p++;
	}
	*buf++ = 0;

	*buf++ = peepgrm;
	*buf++ = 0;
}



static	Int
prev_kanji (void)
{
	Uchar	*p1;
	Uchar	*p2;

	p1 = peephptr + 1;
	if (peepkptr <= p1) return prev_hinsi();

	p2 = peepkptr;
	while (p1 < p2) {
		peepkptr = p1;
		p1 = skipkstr(p1);
	}

	return -1;
}



static	Int
prev_hinsi (void)
{
	Uchar	*p1;
	Uchar	*p2;
	Int     nlen;

	nlen = getnlen(peepdptr);
        p1 = peepdptr + DouonBlkSizeNumber + nlen;

	if (peephptr <= p1) return prev_douon();

	p2 = peephptr;
	while (p1 < p2) {
		peephptr = p1;
		p1 = skiphblk(p1);
	}

	peepgrm = *peephptr;

	p1 = peephptr + 1;
	while (*p1 != HinsiBlkTerm) {
		peepkptr = p1;
		p1 = skipkstr(p1);
	}

	return -1;
}



static	Int
prev_douon (void)
{
	Uchar	*p1;
	Uchar	*p2;
	Int     nlen;

	if (peepdptr <= segtop()) {
		if (peepidx <= DicSegBase) return 0;

		(*curdict->getdic)(curdict, --peepidx);
		get_askknj();

		set_idxyomi();

		p1 = segtop();
		do {
			peepdptr = p1;
			add_yomi();
			p1 = getntag(p1);
		} while (!segend(p1));
	}
	else {
		set_idxyomi();

		p1 = segtop();
		p2 = peepdptr;
		do {
			peepdptr = p1;
			add_yomi();
			p1 = getntag(p1);
		} while (p1 < p2);
	}

	nlen = getnlen(peepdptr);
        p1 = peepdptr + DouonBlkSizeNumber + nlen;

	p2 = getntag(peepdptr);
	do {
		peephptr = p1;
		p1 = skiphblk(p1);
	} while (p1 < p2);

	peepgrm = *peephptr;

	p1 = peephptr + 1;
	do {
		peepkptr = p1;
		p1 = skipkstr(p1);
	} while (*p1 != HinsiBlkTerm);

	return -1;
}



static	Int
next_kanji (void)
{
	Uchar	*p1;

	p1 = skipkstr(peepkptr);
	if (*p1 == HinsiBlkTerm) return next_hinsi();
	peepkptr = p1;
	return -1;
}



static	Int
next_hinsi (void)
{
	Uchar	*p1;

	p1 = skiphblk(peephptr);
	if (p1 >= getntag(peepdptr)) return next_douon();
	peephptr = p1;
	peepgrm = *p1;
	peepkptr = p1 + 1;
	return -1;
}



static	Int
next_douon (void)
{
	Uchar	*p1;
        Int      nlen;

	p1 = getntag(peepdptr);

	if (segend(p1)) {
		if (peepidx + 1 < curdict->segunit) {
			(*curdict->getdic)(curdict, ++peepidx);
			get_askknj();
			peepdptr = segtop();

			set_idxyomi();
		}
		else
	                return 0;
	}
	else {
		peepdptr = p1;
	}

	add_yomi();

        nlen = getnlen(peepdptr);
        peephptr = peepdptr + DouonBlkSizeNumber + nlen;

	peepgrm = *peephptr;

	peepkptr = peephptr + 1;
	
	return -1;
}



static	void
set_idxyomi (void)
{
	Uchar	*p1, *p2;

	if ((p2 = get_idxptr(peepidx)) != NULL) {
		p1 = peepyomi;
		while (*p2) {
			cd2sjh_chr(*p2++, p1);
			p1 += 2;
		}
		*p1 = 0;
	}
}



static	void
add_yomi (void)
{
	Int	nlen;
	Uchar	*p1, *p2;

	nlen = getnlen(peepdptr);
	p1 = peepyomi + getplen(peepdptr) * 2;
	p2 = peepdptr + DouonBlkSizeNumber;

	while (nlen--) {
		cd2sjh_chr(*p2++, p1);
		p1 += 2;
	}

	*p1 = 0;
}



static	void
cd2sjh_chr (Uchar ch, Uchar *dst)
{
	if (ch == _TYOUON) {			
		*dst++ = 0xa1;
		*dst++ = 0xbc; 
	}
	else if (ch == _IGETA) {		
		*dst++ = 0xa1;
		*dst++ = 0xf4;
	}
	else if (ch == _ATTO)  {		
		*dst++ = 0xa1;
		*dst++ = 0xf7;
	}
	else if (ch == _YUUBIN) {		
		*dst++ = 0xa2;
		*dst++ = 0xa9;
	}
	else if (ch < N_0) {			
	}
	else if (ch <= N_9) {			
		*dst++ = 0xa3;
		*dst++ = ch + 0xa0; 
	}
	else if (ch <= A_Z) {			
		*dst++ = 0xa3;
		*dst++ = ch + 0xa7; 
	}
	else if (ch <= A_z) {			
		*dst++ = 0xa3;
		*dst++ = ch + 0xad; 
	}
	else if (ch <= _NN) {			
		*dst++ = 0xa4;
		*dst++ = ch + 0x53; 
	}
	else if (ch <= _XKE) {			
		*dst++ = 0xa5;
		*dst++ = ch + 0x53;
	}
	else {					
	}
}

