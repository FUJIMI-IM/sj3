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
 * $SonyRCSfile: addelcmn.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:27 $
 *
 * $Id$
 */


#include <string.h>
#include "sj_euc.h"
#include "sj_kcnv.h"
#include "sj_hinsi.h"
#include "sj_yomi.h"
#include "kanakan.h"


int	sj2cd_str();
void	mvmemi(), mvmemd(), mkidxtbl();



void
set_size (u_char *p, int size, int plen, int nlen)
{
	*p = (u_char)(size >> 8);
	if (nlen & 0x10) *p |= NokoriYomiTop;
	if (plen & 0x10) *p |= AssyukuYomiTop;
	*(p + 1) = (u_char)size;
	*(p + 2) = (u_char)((nlen & 0x0f) | ((plen & 0x0f) << 4));
}



static	int
isvyomi (u_char *yp)
{
	int	len;

	if (!*yp) return FALSE;	

	if (!(Chrtbl[*yp] & DICTOP)) return FALSE;

	for (len = 0 ; *yp ; yp++) {
		if (!(Chrtbl[*yp]) || (*yp >= Y_KUTEN)) return FALSE;
		if (++len > MaxWdYomiLen) return FALSE;
	}

	return TRUE;
}



static	int
isvknj(u_char *kanji)
{
	int	len;

	if (!*kanji) return FALSE; 	

	for (len = 0 ; *kanji ; ) {
		if (*kanji == SS3) {
			kanji += 3;
			len += 2;	
		} else

		if (isknj1(*kanji)) {
			kanji += 2;
			len += 2;
		}
		else {
			kanji++;
			len += 2;	
		}

		if (len > MaxWdKanjiLen) return FALSE;
	}

	return TRUE;
}



static	int
isgrm (TypeGram gram)
{
	int	grm = gram;

	if (MEISI_1    <= grm && grm <= D_MEISI_6)  return TRUE;
	if (MYOUJI     <= grm && grm <= JOSUUSI2)   return TRUE;
	if (KEIYOUSI_1 <= grm && grm <= DO_ZAHEN)   return TRUE;
	if (DO_1DAN_1  <= grm && grm <= DO_WAGO_2)  return TRUE;
	if (DO_KAGO_5  <= grm && grm <= DO_WAGO_5)  return TRUE;
	if (DO_KAGO_6  <= grm && grm <= DO_WAGO_6)  return TRUE;
	if (DO_1DAN_3  <= grm && grm <= DO_WAGO_3)  return TRUE;
	if (DO_1DAN_4  <= grm && grm <= DO_WAGO_4)  return TRUE;
	if (DO_KAGO_7  <= grm && grm <= DO_WAGO_7)  return TRUE;
	if (DO_KAGO_8  <= grm && grm <= DO_WAGO_8)  return TRUE;
	if (SP_SA_MI1  <= grm && grm <= TANKANJI)   return TRUE;

	return FALSE;
}



u_int
addel_arg (u_char *yp, u_char *kp, TypeGram grm, u_char *nyp, int len)
{
	u_short	err = 0;

	if (!sj2cd_str(yp, nyp, len)) err |= AD_BadYomi;
	if (!isvyomi(nyp)) err |= AD_BadYomi;
	if (!isvknj(kp)) err |= AD_BadKanji;
	if (!isgrm(grm)) err |= AD_BadHinsi;
	return err;
}



static	void
yomi2kata (u_char *src, u_char *dst)
{
	int	first;
	int	second;

	while ((first = *src++) != 0) {
		second = *src++;

		if (first != 0xa4) {
			*dst++ = first;
			*dst++ = second;
		}
		else {
			*dst++ = 0xa5;
			*dst++ = second;
		}
	}

	*dst = 0;
}



static	int
top_strcmp (u_char *src, u_char *dst)
{
	int	i;
	for (i = 0 ; *src && (*src == *dst) ; src++, dst++) i++;
	i /= 2;
	return (i > MaxYomiAskNumber) ? MaxYomiAskNumber : i;
}



static	int
last_strcmp (u_char *src, u_char *dst)
{
	int	slen;
	int	dlen;

	slen = strlen(src);
	dlen = strlen(dst);

	if (dlen > MaxYomiAskNumber * 2) return 0;

	if (strcmp(src + (slen - dlen), dst)) return 0;

	return (dlen / 2);
}



int
cvtknj (u_char *yomi, u_char *knj, u_char *dst)
{
	u_char	kana[MaxWdYomiLen * 2 + 1];
	int	len;
	u_char	*keep;

	keep = dst;			
	yomi2kata(yomi, kana);		

	if ((len = top_strcmp(yomi, knj)) != 0) {
                *dst = (u_char)(ZenHiraAssyuku | (len - 1));
	}
	else if ((len = top_strcmp(kana, knj)) != 0) {
                *dst = (u_char)(ZenKataAssyuku | (len - 1));
	}

	if (len) {
		knj += len * 2;
		dst++;
	}

	if (*knj) {
		for ( ; ; ) {
			if ((len = last_strcmp(yomi, knj)) != 0) {
				*dst++ = (u_char)(ZenHiraAssyuku | (len - 1));
				break;
			}
			else if ((len = last_strcmp(kana, knj)) != 0) {
				*dst++ = (u_char)(ZenKataAssyuku | (len - 1));
				break;
			}
			else if (*knj == SS2) {
				knj++;
				*dst++ = (u_char)LeadingHankaku;
				*dst++ = *knj++;
			} else if (*knj == SS3) {
				knj++;
				*dst++ = (*knj & NormalKanjiMask); knj++;
				*dst++ = *knj++;
			} else if (*knj & 0x80) {
				*dst++ = (*knj & NormalKanjiMask); knj++;
				*dst++ = (*knj & NormalKanjiMask); knj++;
			}
			else {
				*dst++ = (u_char)LeadingHankaku;
				*dst++ = *knj++;
			}

			if (!*knj) {
				break;
			}
		}
	}

	*dst = 0; 

	return (dst - keep);
}



int
srchkana (u_char **ptr, int *saml)
{
	u_char	*p;
	int	cmp;
	int	cnt = 0;
	u_char	same;
	int	flg = 0;

	same = 0;

	p = segtop();

	while (!segend(p)) {

		if (flg == 0) {
			cmp = yomicmp(cnvstart, p, &same);

			if (cmp == OVER) break;

			if (cmp == MATCH && *(cnvstart + same) == 0) {
				*ptr = p;
				*saml = same;
				flg = 1;
			}
		}

		p = getntag(p);
		cnt++;
	}

	if (flg != 0) return cnt;

	*ptr = p;
	*saml = same;
	return 0;
}



int
srchgram (u_char *ptr, u_char **dst, TypeGram hinsi)
{
	u_char	*endp;
	int	cnt = 0, nlen;
	int	flg = FALSE;

	endp = getntag(ptr);

	nlen = getnlen(ptr);
        ptr += DouonBlkSizeNumber + nlen;
	while (ptr < endp) {
		if (*ptr == hinsi) {
			*dst = ptr;
			flg = TRUE;
		}

		do {
			ptr += codesize(*ptr);
		} while (*ptr != HinsiBlkTerm);
		ptr++;
		cnt++;
	}

	if (flg) return cnt;

	*dst = ptr;
	return 0;
}



int
srchkanji (u_char **dst, u_char *knj, int klen)
{
	u_char	*ptr;
	u_char	*p, *q;
	int	len;
	int	flg = FALSE;
	int	cnt = 0;

	ptr = *dst + 1;

	while (*ptr != HinsiBlkTerm) {
		for (p = ptr, q = knj, len = klen ; len ; len--)
			if (*p++ != *q++) break;

		if (!len) {
			*dst = ptr;
			flg = TRUE;
		}

		while (!(*ptr == KanjiStrEnd)) {
			ptr += codesize(*ptr);
		}
		ptr++; 
		cnt++;
	}

	if (!flg) *dst = ptr;

	return cnt;
}



TypeIdxOfs
count_uidx (void)
{
	u_char	*p;

	p = get_idxptr(curdict->segunit - 1);
	while (*p++) ;

	return (curdict->idxlen - (p - idxbuf));
}



void
chg_uidx (TypeDicSeg seg, u_char *yomi, int len)
{
	u_char		*p, *q;
	int		olen;

	p = get_idxptr(seg);

	olen = strlen(p);

	q = idxbuf + curdict->idxlen;

	if (len > olen) {
		mvmemd(q - (len-olen), q, (int)(q - (len-olen) - p));
	}
	else {
		mvmemi(p + (olen-len), p, (int)(q - (p + (olen-len))));
	}

	while (len--) *p++ = *yomi++;

	(*curdict->putidx)(curdict, 0);

	mkidxtbl(curdict);
}

