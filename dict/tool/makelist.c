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
 * $SonyRCSfile: makelist.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:00:39 $
 */


#include <stdio.h>
#include <sys/types.h>
#include "sj_struct.h"

#ifndef SS2
#define SS2 0x8e
#endif

extern	DouonRec *douon_ptr;		
extern	int	douon_num;		
extern	int	yomi_len;		
extern	int	hinsi_num;		
extern	int	kanji_len;		
extern	HindoRec *hindo[];		
extern	int	hindo_num;		
extern	HindoRec *askknj[];		
extern	int	askknj_num;		
extern	HindoRec *assyuku;		

static	DouonRec *drectmp = NULL;



static	clearklist(krec)
register KanjiRec *krec;
{
	register KanjiRec *p;

	
	while (krec) {
		
	        Free(krec -> kptr);
		Free(krec -> aptr);

		
		p = krec;
		krec = krec -> knext;

		
		Free(p);
	}
}



static	clearhlist(hrec)
register HinsiRec *hrec;
{
	register HinsiRec *p;

	
	while (hrec) {
		
		clearklist(hrec -> krec);

		
		p = hrec;
		hrec = hrec -> hnext;

		
		Free(p);
	}
}



clear_list()
{
	register DouonRec *p;
	register DouonRec *drec = douon_ptr;

	
	while (drec) {
		
		Free(drec -> yptr);

		
		clearhlist(drec -> hrec);

		
		p = drec;
		drec = drec -> dnext;

		
		Free(p);
	}

	douon_ptr = NULL;
}



static	u_char	*makekanji(yomi, kanji, atr, len)
int	*yomi;		
int	*kanji;		
int	*atr;		
int	*len;		
{
	int	kana[MaxYomiLength + 1];
	u_char	ktmp[MaxKanjiLength * 3 + MaxAtrNumber * 2 + 1];
	register int	i;
	register int	pos = 0;
	int	*p;
	u_char	*q;

	
	for (p = yomi, i = 0 ; *p ; )
		kana[i++] = h2kcode(*p++);
	kana[i] = 0;
	
	while (*atr) {
		i = *atr;
		atr++;
#ifndef	NO_ATR
		ktmp[pos++] = (AiAttribute | ((i >> 8) & 0xff));
                ktmp[pos++] = (i & 0xff);
#endif
	}

	
	if (i = top_strcmp(yomi, kanji)) {
		kanji += i;
		ktmp[pos++] = (ZenHiraAssyuku | (i - 1));
	}
	
	else if (i = top_strcmp(kana, kanji)) {
		kanji += i;
		ktmp[pos++] = (ZenKataAssyuku | (i - 1));
	}

	
	while (*kanji) {
		
		if (i = last_strcmp(yomi, kanji)) {
			kanji += i;
			ktmp[pos++] = (ZenHiraAssyuku | (i - 1));
		}
		
		else if (i = last_strcmp(kana, kanji)) {
			kanji += i;
			ktmp[pos++] = (ZenKataAssyuku | (i - 1));
		}
		
		else if (((*kanji >> 8) & 0xff) == SS2 ||  *kanji < 0x100) {
			ktmp[pos] = LeadingHankaku;
			ktmp[pos + 1] = (*kanji & 0xff);
			kanji++;
			pos += 2;
		}
		
		else {
			if (*kanji < 0x10000) {
				ktmp[pos] = ((*kanji >> 8) & NormalKanjiMask);
				ktmp[pos + 1] = (*kanji & NormalKanjiMask);
				kanji++;
				pos += 2;
			} else if (*kanji < 0x1000000) {
				ktmp[pos] = ((*kanji >> 8) & NormalKanjiMask);
				ktmp[pos + 1] = (*kanji & 0xff);
				kanji++;
				pos += 2;
			} else {
				fprintf(stderr, "Error: 4 byte code is found\n");
				exit(0);
			}
		}
	}
	*len = pos;

	
	q = (u_char *)Malloc(pos);
	if (!q) {
		fprintf(stderr, "\245\341\245\342\245\352\244\254\311\324\302\255\244\267\244\336\244\267\244\277");
		exit(1);
	}
	memcpy(q, ktmp, pos);

	return q;
}



static	u_char	*makeyomi(yomi)
int	*yomi;
{
	u_char	tmp[MaxYomiLength + 1];
	register int	i;
	register int	j;
	register int	*y = yomi;
	register u_char	*p;

	
	for (i = 0 ; *y ; ) {
		j = cnvyomi(*y++);
		if (j == 0) {
			fprintf(stderr, "\311\324\300\265\244\312\312\270\273\372\244\254\306\311\244\337\244\313\273\310\244\357\244\354\244\306\244\244\244\336\244\271\n");
			output_int(stderr, yomi); fputc('\n', stderr);
			exit(1);
		}
		tmp[i++] = j;
	}
	tmp[i++] = 0;

	
	p = (u_char *)Malloc(i);
	if (!p) {
		fprintf(stderr, "\245\341\245\342\245\352\244\254\311\324\302\255\244\267\244\336\244\267\244\277");
		exit(1);
	}
	memcpy(p, tmp, i);

	return p;
}



static	KanjiRec *make_krec(kcode, klen)
u_char	*kcode;
int	klen;
{
	register KanjiRec *krec;

	
	krec = (KanjiRec *)Malloc(sizeof(KanjiRec));
	if (!krec) {
		fprintf(stderr, "\245\341\245\342\245\352\244\254\311\324\302\255\244\267\244\336\244\267\244\277");
		exit(1);
	}

	
	krec -> klen = klen;
	krec -> kptr = kcode;
	krec -> alen = 0;
	krec -> aptr = NULL;
	krec -> knext = NULL;

	return krec;
}



static	HinsiRec *make_hrec(hinsi)
int	hinsi;
{
	register HinsiRec *hrec;

	
	hrec = (HinsiRec *)Malloc(sizeof(HinsiRec));
	if (!hrec) {
		fprintf(stderr, "\245\341\245\342\245\352\244\254\311\324\302\255\244\267\244\336\244\267\244\277");
		exit(1);
	}

	
	hrec -> hinsi = hinsi;
	hrec -> krec  = NULL;
	hrec -> hnext = NULL;

	return hrec;
}



static	DouonRec *make_drec(ycode)
u_char	*ycode;
{
	register DouonRec *drec;

	
	drec = (DouonRec *)Malloc(sizeof(DouonRec));
	if (!drec) {
		fprintf(stderr, "\245\341\245\342\245\352\244\254\311\324\302\255\244\267\244\336\244\267\244\277");
		exit(1);
	}

	
	drec -> yptr = ycode;
	drec -> hrec_num = 0;
	drec -> hrec = NULL;
	drec -> dnext = NULL;

	return drec;
}



static	diff_ylen(drec)
DouonRec *drec;
{
	register DouonRec *dptr;
	register DouonRec *dprev;
	register u_char	*p1;
	register u_char	*p2;
	register int	ylen = 0;

	
	dptr = douon_ptr; dprev = NULL;
	while (dptr) {
		dprev = dptr;
		dptr = dptr -> dnext;
	}

	if (dprev) {
		
		p1 = dprev -> yptr; p2 = drec  -> yptr;
		while (*p1 && (*p1 == *p2)) { p1++; p2++; }

		
		if (*p1 == *p2) {
			fprintf(stderr, "\305\371\244\267\244\244\306\311\244\337\244\316\306\261\262\273\270\354\245\326\245\355\245\303\245\257\244\254\244\242\244\353\n");
			output_yomi(stderr, dprev -> yptr); fputc('\n', stderr);
			exit(1);
		}
		
		else if (*p1 > *p2) {
			fprintf(stderr, "\306\311\244\337\244\316\275\347\275\370\244\254\244\252\244\253\244\267\244\244\n");
			output_yomi(stderr, dprev->yptr); fputc('\n', stderr);
			output_yomi(stderr, drec->yptr); fputc('\n', stderr);
			exit(1);
		}

		
		while (*p2++) ylen++;
	}

        return ylen;
}



static	douon_knj(drec)
DouonRec *drec;
{
	int	i;
	int	len = 0;
	u_char	*p;
	u_char	*knjofscvt();
	HinsiRec *hrec;
	KanjiRec *krec;

	
	for (hrec = drec -> hrec ; hrec ; hrec = hrec -> hnext) {
		
		for (krec = hrec -> krec ; krec ; krec = krec -> knext) {
			
			p = knjofscvt(krec -> kptr, krec -> klen, &i);

			
			len += i + 1;

			
			knjhnd_set(p, i);

			
			krec -> alen = i;
			krec -> aptr = p;

			
			set_ofsrec(krec -> kptr, krec -> klen, 0);
		}
	}

	
	return len;
}



static void make_d_list(drec)
DouonRec *drec;
{
	DouonRec *dptr, *dprev;
	HinsiRec *hptr;
	KanjiRec *kptr;
	int	ylen;
	int	klen;
	int	hnum;
	int	len;
	int	i;

start:
	
	if (douon_ptr == NULL) {
		douon_num = 0;
		yomi_len  = 0;
		hinsi_num = 0;
		kanji_len = 0;
		assyuku = NULL;
	}

	
	klen = douon_knj(drec);

	
	ylen = diff_ylen(drec);

	
	hnum = drec -> hrec_num;

	
	i = 1 +				
	    MaxKnjAskNumber +       
					
	    douon_num * DouonBlkSizeNumber + DouonBlkSizeNumber +
	    yomi_len + ylen  +	
	    hinsi_num + hnum +		
	    hinsi_num + hnum +		
	    kanji_len + klen;		

	
	len = (i <= MainSegmentLength) ? 0 : decide_knjask();

	
	if ((i - len) <= MainSegmentLength) {
		
		if (douon_ptr) {
			
			dptr = douon_ptr; dprev = NULL;
			while (dptr) {
				dprev = dptr;
				dptr = dptr -> dnext;
			}

			
			dprev -> dnext = drec;
		}
		
		else {
			
			douon_ptr = drec;
		}
		drec  -> dnext = NULL;

		
		drec -> dlen = ylen;

		
		douon_num += 1;

		
	        yomi_len += ylen;

		
		hinsi_num += hnum;

		
		kanji_len += klen;

		return;
	}
	
	else if (douon_ptr == NULL) {
		fprintf(stderr, "\243\261\306\261\262\273\270\354\245\326\245\355\245\303\245\257\244\254\302\347\244\255\244\271\244\256\244\336\244\271\n");
		exit(1);
	}

	
	for (hptr = drec -> hrec ; hptr ; hptr = hptr -> hnext) {
		for (kptr = hptr -> krec ; kptr ; kptr = kptr -> knext) {

			
			knjhnd_reset(kptr -> aptr, kptr -> alen);

			
			Free(kptr -> aptr);
			kptr -> aptr = NULL;
		}
	}

	
	len = decide_knjask();

	
	makeseg();

	
	clear_list();

	
	clear_hindo();

	
	clear_ofsrec();

	
	askknj_num = 0;

	
	clear_assyuku();

	goto start;
}



flush_douon()
{
	if (drectmp) {
		
		make_d_list(drectmp);

		drectmp = NULL;
	}

	if (douon_ptr) {
		
		decide_knjask();

		
		makeseg();

		
		clear_list();

		
		clear_hindo();

		
		clear_ofsrec();

		
		askknj_num = 0;
	}
}


void
makelist(yomi, kanji, hinsi, atr)
int	*yomi;
int	*kanji;
int	hinsi;
int	*atr;
{
	u_char	*ycode;
	u_char	*kcode;
	int	klen;
	HinsiRec *hrec;
	KanjiRec *krec, *kprev;

	
	ycode = makeyomi(yomi);

	
	kcode = makekanji(yomi, kanji, atr, &klen);

	
	if (drectmp && strcmp(ycode, drectmp -> yptr)) {
		
		make_d_list(drectmp);

		drectmp = NULL;
	}

	
	if (!drectmp) {
		
		drectmp = make_drec(ycode);

		
		hrec = make_hrec(hinsi);
		drectmp -> hrec = hrec;
		drectmp -> hrec_num = 1;

		
		krec = make_krec(kcode, klen);
		hrec -> krec      = krec;

		return;
	}

	
	for (hrec = drectmp -> hrec ; hrec -> hnext ; hrec = hrec -> hnext)
		if (hrec -> hinsi == hinsi) break;

	
	if (hrec -> hinsi == hinsi) {
		
		for (krec=hrec->krec ; krec ; krec=krec->knext) {
			
			if ((krec -> klen == klen) &&
			    !memcmp(krec -> kptr, kcode, klen)) {
				fprintf(stderr, "\306\261\260\354\244\316\275\317\270\354\244\254\302\270\272\337\244\267\244\277\n");
				fprintf(stderr, "\t\306\311\244\337:");
				output_int(stderr, yomi);
				fprintf(stderr, "\n");
				fprintf(stderr, "\t\264\301\273\372:");
				output_int(stderr, kanji);
				fprintf(stderr, "\n");

				
				Free(kcode);

				return;
			}
			kprev = krec;
		}

		
		kprev -> knext = make_krec(kcode, klen);

		return;
	}

	
	hrec -> hnext = make_hrec(hinsi);
	hrec = hrec -> hnext;
	drectmp -> hrec_num += 1;

	
	krec = make_krec(kcode, klen);
	hrec -> krec = krec;
}
