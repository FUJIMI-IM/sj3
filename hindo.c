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
 * $SonyRCSfile: hindo.c,v $  
 * $SonyRevision: 1.2 $ 
 * $SonyDate: 1994/08/17 01:50:26 $
 *
 * $Id$
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "sj_struct.h"
#include "dicttool.h"

extern	HindoRec *hindo[];		
extern	int	hindo_num;		
extern	AssyukuRec *assyuku;		
extern	HindoRec *askknj[];		
extern	int	askknj_num;		
extern	int	kanji_num;		


int
check_hindo(unsigned char* ptr, int len)
{
	int	low, high, mid;
	int	i;

	
	if (hindo_num <= 0) return 0;

	
	low = 0; high = hindo_num - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		i = string_cmp(hindo[mid]->kptr, hindo[mid]->klen, ptr, len);
		if (i > 0)
			high = mid - 1;
		else if (i < 0)
			low = ++mid;
		else
			return(hindo[mid]->exist + hindo[mid]->hindo);
	}

	return 0;
}



static AssyukuRec*
makeassyuku (int num)
{
	AssyukuRec *arec;

	
	arec = (AssyukuRec *)Malloc(sizeof(AssyukuRec));
	if (!arec) {
		fprintf(stderr, "\245\341\245\342\245\352\244\254\302\255\244\352\244\336\244\273\244\363\n");
		exit(1);
	}

	
	arec -> len = num;
	arec -> nrec = NULL;
	arec -> anext = NULL;

	return arec;
}


static int
assyuku_len(HindoRec* hrec)
{
	int	org;
	int	old;
	int	new;
	OffsetRec *p;

	
	if (((*(hrec->kptr) & KanjiModeMask) == OffsetAssyuku) &&
	    (hrec->klen == 2)) {
		
		p = real_ofsrec(hrec -> kptr);

		
		org = check_hindo(p -> kptr, p -> klen);

		
		
		old = p->klen * org + 2 * (hrec->exist + hrec->hindo);

		
		
		new = p->klen + (hrec->exist + hrec->hindo + org);
	}
	
	else {
		
		old = hrec -> klen * (hrec -> exist + hrec -> hindo);

		
		new = hrec -> klen + (hrec -> exist + hrec -> hindo);
	}

	
	return (old - new);
}



static void
set_assyuku(HindoRec* hrec)
{
	AssyukuRec *arec;
	AssyukuRec *aprev;
	int	len;

	
	len = assyuku_len(hrec);

	
	if (len <= 0) return;

	
	arec = assyuku; aprev = NULL;
	while (arec) {
		
		if (arec -> len == len) {
			
			hrec -> arec = arec;
			hrec -> anext = arec -> nrec;
			arec -> nrec = hrec;
			return;
		}
		
		else if (arec -> len < len)
			break;

		aprev = arec;
		arec = arec -> anext;
	}

	
	arec = makeassyuku(len);

	
	hrec -> anext = NULL;
	hrec -> arec = arec;
	arec -> nrec = hrec;

	
	if (aprev) {
		arec  -> anext = aprev -> anext;
		aprev -> anext = arec;
	}
	else {
		arec  -> anext = assyuku;
		assyuku = arec;
	}
}



static void
reset_assyuku(HindoRec* hrec)
{
	AssyukuRec *arec;
	AssyukuRec *aptr;
	AssyukuRec *aprev;
	HindoRec *hptr;
	HindoRec *hprev;

	arec = hrec -> arec;

	
	if (!arec) return;
	hrec -> arec = NULL;

	
	hptr = arec -> nrec; hprev = NULL;
	while (hptr) {
		if (hptr == hrec) break;
		hprev = hptr;
		hptr = hptr -> anext;
	}

	
	if (!hptr) return;

	
	if (hprev) {
		hprev -> anext = hrec -> anext;
		hrec -> anext = NULL;
		return;
	}

	
	if (hrec -> anext) {
		arec -> nrec = hrec -> anext;
		hrec -> anext = NULL;
		return;
	}

	

	
	if (!assyuku) return;
	aptr = assyuku; aprev = NULL;
	while (aptr) {
		if (aptr == arec) break;
		aprev = aptr;
		aptr = aptr -> anext;
	}

	
	if (!aptr) return;

	
	if (aprev)
		aprev -> anext = arec -> anext;
	
	else
		assyuku = arec -> anext;

	
	Free(arec);

	
	hrec -> anext = NULL;
}



static HindoRec*
makehindo(unsigned char* ptr, int len, int alen)
{
	HindoRec *hrec;
	unsigned char	*p;

	
	hrec = (HindoRec *)Malloc(sizeof(HindoRec));
	if (!hrec) {
		fprintf(stderr, "\245\341\245\342\245\352\244\254\302\255\244\352\244\336\244\273\244\363\n");
		exit(1);
	}
	p = (unsigned char *)Malloc(len);
	if (!p) {
		fprintf(stderr, "\245\341\245\342\245\352\244\254\302\255\244\352\244\336\244\273\244\363\n");
		exit(1);
	}
	
	hrec -> klen = len;
	hrec -> kptr = p;
	if (len == alen && len > 2) {
		hrec -> hindo = 0;
		  hrec -> exist = 1;
		  hrec -> offset = ++kanji_num;
	}
	else {
		hrec -> hindo = 1;
		hrec -> exist = 0;
		hrec -> offset = 0;
	}
	while (len-- > 0) *p++ = *ptr++;
	hrec -> anext  = NULL;
	hrec -> arec   = NULL;

	return hrec;
}



static void
set_hindo(unsigned char* ptr, int len, int alen)
{
	HindoRec *hrec;
	int	low, high, mid;
	int	i;

	
	if (hindo_num <= 0) {
		hrec = makehindo(ptr, len, alen);
		hindo[0]= hrec;
		hindo_num = 1;

		
		set_assyuku(hrec);

		return;
	}

	
	low = 0; high = hindo_num - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		i = string_cmp(hindo[mid]->kptr, hindo[mid]->klen, ptr, len);
		if (i > 0)
			high = mid - 1;
		else if (i < 0)
			low = ++mid;
		else
			break;
	}

	
	if (i) {
		if (hindo_num >= MaxHindoNumber) {
			fprintf(stderr, "\311\321\305\331\245\306\241\274\245\326\245\353\244\254\244\242\244\325\244\354\244\336\244\267\244\277\n");
			exit(1);
		}

		
		hrec = makehindo(ptr, len, alen);

		
		for (i = hindo_num ; i > mid ; i--) hindo[i] = hindo[i - 1];
		hindo[mid] = hrec;
		hindo_num++;
	}
	
	else {
		hrec = hindo[mid];

		
		reset_assyuku(hrec);

		
		if (hrec -> exist > 0)
			hrec -> exist += 1;

		
		else if (len == alen && len > 2) {
			hrec -> exist = 1;
			hrec -> offset = ++kanji_num;
		}

		
		else
			hrec -> hindo += 1;
	}

	
	set_assyuku(hrec);
}



static void
reset_hindo(unsigned char* p, int l)
{
	HindoRec *hrec;
	int	low, high, mid;
	int	i;

	
	if (hindo_num <= 0) return;

	
	low = 0; high = hindo_num - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		i = string_cmp(hindo[mid]->kptr, hindo[mid]->klen, p, l);
		if (i > 0)
			high = mid - 1;
		else if (i < 0)
			low = ++mid;
		
		else {
			hrec = hindo[mid];

			
			reset_assyuku(hrec);

			if (hrec -> exist > 0)
				hrec -> exist -= 1;
			else if (hrec -> hindo > 0)
				hrec -> hindo -= 1;

			
			if (hrec -> exist + hrec -> hindo == 0) {
				while (++mid < hindo_num) {
					hindo[mid - 1] = hindo[mid];
				}
				hindo_num--;
				Free(hrec -> kptr);
				Free(hrec);
			}
			else {
				
				set_assyuku(hrec);
			}

			return;
			break;
		}
	}
}


void
knjhnd_set(unsigned char* p, int l)
{
	unsigned char	*kp1;
	unsigned char	*kp2;
	int	i;
	int	kl1;
	unsigned char	tmp[MaxKanjiLength * 2 + MaxAtrNumber * 2 + 1];

	for (kp1 = p, kl1 = l ; kl1 > 0 ; ) {

		
		for (i = 0, kp2 = kp1 ; i < kl1 ; ) {
			if (codesize(*kp2) == 1) {
				tmp[i++] = *kp2++;
			}
			else if (codesize(*kp2) == 2) {
				tmp[i++] = *kp2++;
				tmp[i++] = *kp2++;
			} else {
                                tmp[i++] = *kp2++;
                                tmp[i++] = *kp2++;
                                tmp[i++] = *kp2++;
			}
			set_hindo(tmp, i, l);
		}

		
		kl1 -= codesize(*kp1);
		kp1 += codesize(*kp1);
	}
}


void
knjhnd_reset(unsigned char* p, int l)
{
	unsigned char	*kp1;
	unsigned char	*kp2;
	int	i;
	int	kl1;
	unsigned char	tmp[MaxKanjiLength * 2 + MaxAtrNumber * 2 + 1];

	for (kp1 = p, kl1 = l ; kl1 > 0 ; ) {

		
		for (i = 0, kp2 = kp1 ; i < kl1 ; ) {
			if (codesize(*kp2) == 1) {
				tmp[i++] = *kp2++;
			}
			else if (codesize(*kp2) == 2) {
				tmp[i++] = *kp2++;
				tmp[i++] = *kp2++;
			} else {
                                tmp[i++] = *kp2++;
                                tmp[i++] = *kp2++;
                                tmp[i++] = *kp2++;
			}
			reset_hindo(tmp, i);
		}

		
		kl1 -= codesize(*kp1);
		kp1 += codesize(*kp1);
	}
}



static int
check_assyuku(HindoRec* p, HindoRec* q)
{
	unsigned char	ptmp[128], qtmp[128];
	int	plen;
	int	qlen;

	
	plen = make_knjstr(p -> kptr, p -> klen, ptmp);

	
	qlen = make_knjstr(q -> kptr, q -> klen, qtmp);

	
	if (bubun_str(ptmp, plen, qtmp, qlen)) {
		return -1;
	}

	
	if (overlap_str(ptmp, plen, qtmp, qlen))
		return -1;

	return 0;
}


int
decide_knjask(void)
{
	int	i, j;
	AssyukuRec *p;
	HindoRec *q;
	unsigned char	*r;
	int	len = 0;

	askknj_num = 0;

	
	for (p = assyuku ; p ; p = p -> anext) {

		
		if (p -> len <= 0) break;

		
		for (q = p -> nrec ; q ; q = q -> anext) {

			
			i = q -> klen;
			r = q -> kptr;
			if (i > 2) {
				while (i > 0) {
					if ((*r & KanjiModeMask)
					    == OffsetAssyuku) {
						break;
					}
					j = codesize(*r);
					i -= j;
					r += j;
				}
				if (i) continue;
			}

			if (askknj_num) {
				
				for (i = 0 ; i < askknj_num ; i++) {
					if (check_assyuku(askknj[i], q))
						break;
				}

				
				if (i >= askknj_num) {
					askknj[askknj_num++] = q;
					len += p -> len;
					if (askknj_num >= MaxKnjAskNumber)
						goto end;
				}
			}
			else {
				askknj[askknj_num++] = q;
				len += p -> len;
			}
		}
	}
end:
{
	int	i, j, len;
	unsigned char	*p;

	for (i = 0 ; i < askknj_num ; i++) {
		len = askknj[i] -> klen;
		p   = askknj[i] -> kptr;
		if (len <= 2) continue;
		while (len > 0) {
			if ((*p & KanjiModeMask) == OffsetAssyuku) {
				printf("\245\252\245\325\245\273\245\303\245\310\260\265\275\314\244\362\264\336\244\340 %d\n", i);
			}
			j = codesize(*p);
			len -= j;
			p += j;
		}
	}
}
	return len;
}


void
clear_hindo(void)
{
	int	i;

	for (i = 0 ; i < hindo_num ; i++) {
		Free(hindo[i]->kptr);
		Free(hindo[i]);
	}
	hindo_num = 0;
	kanji_num = 0;
}


void
clear_assyuku(void)
{
	AssyukuRec *p;

	while (assyuku) {
		p = assyuku;
		assyuku = assyuku -> anext;
		Free(p);
	}
}
