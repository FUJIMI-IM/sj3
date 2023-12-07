/*
 * Copyright (c) 1994  Sony Corporation
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
 * $SonyRCSfile: sj3_rkcv.c,v $  
 * $SonyRevision: 1.3 $ 
 * $SonyDate: 1995/02/10 06:02:25 $
 */

#include <string.h>

#include "wchar16.h"
#include "sj.h"
#include "kana.h"
#include "rk.h"
#include "kctype.h"

#if defined(__sony_news) && (SVR4)
#define wscmp sj3_wscmp16
#define wsncmp sj3_wsncmp16
#define wsncpy sj3_wsncpy16
#define wscpy sj3_wscpy16
#define wslen sj3_wslen16
#define mbstowcs sj3_mbstowcs16
#define wcstombs sj3_wcstombs16
#endif 

extern int current_locale;

#define sj_iskata(c) (c >= 0xa5a1 && c <= 0xa5f6)
#define sj_ishira(c) (c >= 0xa4a1 && c <= 0xa4f3)
static wchar16_t intmp[MAXLLEN];
static wchar16_t outtmp[MAXLLEN];
static u_char  mtmp[MAXLLEN];

static u_short HKtable[ZKATAn] = {
	KANA_XA,	KANA_A,
	KANA_XA + 1,	KANA_A	+ 1,
	KANA_XA + 2,	KANA_A	+ 2,
	KANA_XA + 3,	KANA_A	+ 3,
	KANA_XA + 4,	KANA_A  + 4,

			KANA_KA,	KANA_GA,
			KANA_KA + 1,	KANA_GA + (1<<8),
			KANA_KA + 2,	KANA_GA + (2<<8),
			KANA_KA + 3,	KANA_GA + (3<<8),
			KANA_KA + 4,	KANA_GA + (4<<8),

			KANA_SA,	KANA_ZA,
			KANA_SA + 1,	KANA_ZA + (1<<8),
			KANA_SA + 2,	KANA_ZA + (2<<8),
			KANA_SA + 3,	KANA_ZA + (3<<8),
			KANA_SA + 4,	KANA_ZA + (4<<8),

			KANA_TA,	KANA_DA,
			KANA_TA + 1,	KANA_DA + (1<<8),
	KANA_XTSU,	KANA_TA + 2,	KANA_DA + (2<<8),
			KANA_TA + 3,	KANA_DA + (3<<8),
			KANA_TA + 4,	KANA_DA + (4<<8),
	
			KANA_NA,
			KANA_NA + 1,
			KANA_NA + 2,
			KANA_NA + 3,
			KANA_NA + 4,

			KANA_HA,	KANA_BA,	  KANA_PA,
			KANA_HA + 1,	KANA_BA + (1<<8), KANA_PA + (1<<8),
			KANA_HA + 2,	KANA_BA + (2<<8), KANA_PA + (2<<8),
			KANA_HA + 3,	KANA_BA + (3<<8), KANA_PA + (3<<8),
			KANA_HA + 4,	KANA_BA + (4<<8), KANA_PA + (4<<8),

			KANA_MA,
			KANA_MA + 1,
			KANA_MA + 2,
			KANA_MA + 3,
			KANA_MA + 4,

	KANA_XYA,	KANA_YA,
	KANA_XYA + 1,	KANA_YA + 1,
	KANA_XYA + 2,	KANA_YA + 2,

			KANA_RA,
			KANA_RA + 1,
			KANA_RA + 2,
			KANA_RA + 3,
			KANA_RA + 4,

	KANA_WA, 	KANA_WA,
			KANA_A  + 1,
			KANA_A  + 3,
			KANA_WO,
			KANA_WA + 1,

					((KANA_A+2)<<8) + KANA_DTEN,
			KANA_KA,
			KANA_KA + 3
};

static struct ztohkktbl {
	u_short zen;
	u_short han;
} HKKtbl[8] = {
	ZEN_KTEN, 	KANA_KTEN,
	ZEN_FKAKKO, 	KANA_FKAKKO,
	ZEN_EKAKKO, 	KANA_EKAKKO,
	ZEN_TTEN, 	KANA_TTEN,
	ZEN_CTEN, 	KANA_CTEN,
	ZEN_CHOUON, 	KANA_CHOUON,
	ZEN_DTEN, 	KANA_DTEN,
	ZEN_HDTEN, 	KANA_HDTEN
};


static struct hzkigoutbl {
	wchar16_t han;
	u_short zen;
} HZKtbl[6] = {
	',',	ZEN_TTEN,
	'.',	ZEN_KTEN,
	'-',	ZEN_CHOUON,
	'@',	ZEN_CTEN,
	'[',	ZEN_FKAKKO,
	']',	ZEN_EKAKKO
};

static u_short HZtbl[95] = {
	0xa1a1, 0xa1aa, 0xa1c9, 0xa1f4, 0xa1f0, 0xa1f3, 0xa1f5, 0xa1c7,
	0xa1ca, 0xa1cb, 0xa1f6, 0xa1dc, 0xa1a4, 0xa1dd, 0xa1a5, 0xa1bf,
	0xa3b0, 0xa3b1, 0xa3b2, 0xa3b3, 0xa3b4, 0xa3b5, 0xa3b6, 0xa3b7,
	0xa3b8, 0xa3b9, 0xa1a7, 0xa1a8, 0xa1e3, 0xa1e1, 0xa1e4, 0xa1a9,
	0xa1f7, 0xa3c1, 0xa3c2, 0xa3c3, 0xa3c4, 0xa3c5, 0xa3c6, 0xa3c7,
	0xa3c8, 0xa3c9, 0xa3ca, 0xa3cb, 0xa3cc, 0xa3cd, 0xa3ce, 0xa3cf,
	0xa3d0, 0xa3d1, 0xa3d2, 0xa3d3, 0xa3d4, 0xa3d5, 0xa3d6, 0xa3d7,
	0xa3d8, 0xa3d9, 0xa3da, 0xa1ce, 0xa1ef, 0xa1cf, 0xa1b0, 0xa1b2,
	0xa1c6, 0xa3e1, 0xa3e2, 0xa3e3, 0xa3e4, 0xa3e5, 0xa3e6, 0xa3e7,
	0xa3e8, 0xa3e9, 0xa3ea, 0xa3eb, 0xa3ec, 0xa3ed, 0xa3ee, 0xa3ef,
	0xa3f0, 0xa3f1, 0xa3f2, 0xa3f3, 0xa3f4, 0xa3f5, 0xa3f6, 0xa3f7,
	0xa3f8, 0xa3f9, 0xa3fa, 0xa1d0, 0xa1c3, 0xa1d1, 0xa1b1
};

#ifdef ADDHK
setl_hktozh()
{
	register int i;
	register u_short c, zen1;
	wchar16_t rstr[2];
	wchar16_t kstr[2];
	RkTablW16 *rktp, *mktable();

	zen1 = (ZHIRA1 << 8);
	rstr[1] = '\0';
	kstr[1] = 0;
	for(i = 0; i < ZKATAn - 3; i++) {
		c = HKtable[i];
		if (!(c & RKZEN)) {
			rstr[0] = c;
			if ((rktp = mktable(rstr, 1)) == NULL)
				break;
			if (rktp->k_yomi != NULL)
				continue;
			kstr[0] = zen1 + (ZHIRAF2 + i);
			if (kstradd(&(rktp->k_yomi), kstr, 2) == -1)
				break;
		}
	}
	for (i = 0; i < 8; i++) {
		rstr[0] = HKKtbl[i].han;
		if ((rktp = mktable(rstr, 1)) == NULL)
			break;
		if (rktp->k_yomi != NULL)
			continue;
		kstr[0] = HKKtbl[i].zen;
		if (kstradd(&(rktp->k_yomi), kstr, 2) == -1)
			break;
	}
}
#endif


mkkigou()
{
	register int i;
	wchar16_t rstr[2];
	wchar16_t kstr[2];
	RkTablW16 *rktp, *mktable();

	rstr[1] = '\0';
	kstr[1] = 0;
	for (i = 0; i < 6; i++) {
		rstr[0] = HZKtbl[i].han;
		if ((rktp = mktable(rstr, 1)) == NULL)
			break;
		if (rktp->k_yomi != NULL)
			continue;
		kstr[0] = HZKtbl[i].zen;
		if (kstradd(&(rktp->k_yomi), kstr, 2) == -1)
			break;
	}
}
			
sj_addten(prefix, c)
u_short prefix, c;
{
	register u_short c1, cc;

	cc = 0;
	if ((c == ZEN_DTEN || c == ZEN_HDTEN) &&
	    (sj_ishira(prefix) || sj_iskata(prefix))) {
		c1 = sj_zen2han(prefix);
		if ((c1 >= KANA_KA && c1 < KANA_NA) ||
			 	(c1 >= KANA_HA && c1 < KANA_MA)) {
			if (c == ZEN_DTEN)
				cc = prefix + 1;
			else if (c == ZEN_HDTEN && c1 >= KANA_HA)
				cc = prefix + 2;
		} else if (c1 == KANA_A+2 && sj_iskata(prefix)&& c == ZEN_DTEN)
			cc = ZEN_VU;
	}
	return(cc);
}

sj_han2zen(c)
u_short c;
{
	register int i;
	register u_short cc;

	if (WcIsHANKAKU(c) && iskana2(c)) {
		for (i = 0; i < ZKATAn; i++) {
			if (c == HKtable[i]) {
				if (i >= ZKATAn - 3) {
					cc = (ZKATA1 << 8) + (ZKATAF2 + i + 1);
				}
				else {
					cc = (ZHIRA1 << 8) + (ZHIRAF2 + i);
					if (c == KANA_WA)
						cc++;
				}
				break;
			}
		}
		if (i >= ZKATAn) {
			for (i = 0; i < 8; i++) {
				if (c == HKKtbl[i].han) {
					cc = HKKtbl[i].zen;
					break;
				}
			}
			if (i >= 8)
				cc = ZEN_SPACE;
		}
	} else {
		if (c >= 0x20 && c <= 0x7e)
			cc = HZtbl[c - 0x20];
		else
			cc = c;
	}
	return(cc);
}


extern int sj3_hantozen_w16();

sj3_hantozen(out, in)
u_char *out, *in;
{
	wchar16_t *winstr, *woutstr;
	u_char *mstr;
	int inlen, outlen, imflag = 0, omflag = 0, mmflag = 0, ret;

	inlen = strlen(in) + 1;
	if (current_locale == LC_CTYPE_EUC) {
		if (inlen > sizeof(mtmp)) {
			mstr = (u_char *) malloc(inlen);
			mmflag = 1;
		} else {
			mstr = mtmp;
		}
		if (sjistoeuc(mstr, inlen, in, inlen) < 0) {
			if (mmflag) free(mstr);
			return -1;
		}
	} else {
		mstr = in;
	}
	if ((inlen * sizeof(wchar16_t)) > sizeof(intmp)) {
		winstr = (wchar16_t *) malloc(inlen * sizeof(wchar16_t));
		if (!winstr) {
			if (mmflag) free(mstr);
			return -1;
		}
		imflag = 1;
	} else {
		winstr = (wchar16_t *) intmp;
	}
	if (mbstowcs(winstr, (char *)mstr, inlen) == -1) {
		if (mmflag) free(mstr);
		if (imflag) free(winstr);
		return -1;
	}
	outlen = inlen;
	if ((outlen * sizeof(wchar16_t)) > sizeof(outtmp)) {
		woutstr = (wchar16_t *) malloc(outlen * sizeof(wchar16_t));
		if (!woutstr) {
			if (mmflag) free(mstr);
			if (imflag) free(winstr);
			return -1;
		}
                omflag = 1;
	} else {
                woutstr = (wchar16_t *) outtmp;
	}
	ret = sj3_hantozen_w16(woutstr, winstr);
	if (mmflag) {
		free(mstr);
		mmflag = 0;
	}
	if (ret == 0 || ret == -1) {
		if (imflag) free(winstr);
		if (omflag) free(woutstr);
		return ret;
	}
	outlen = ret * 3;
	if (outlen > sizeof(mtmp)) {
		mstr = (u_char *) malloc(outlen);
		if (!mstr) {
			if (imflag) free(winstr);
			if (omflag) free(woutstr);
			return -1;
		}
		mmflag = 1;
	} else {
		mstr = mtmp;
	}
	if (wcstombs((char *)mstr, woutstr, outlen) == -1) {
		if (imflag) free(winstr);
		if (omflag) free(woutstr);
		if (mmflag) free(mstr);
		return -1;
	}
	if (current_locale == LC_CTYPE_EUC) {
		inlen = euctosjis(out, outlen, mstr, outlen);
	} else {
		(void) memcpy(out, mstr, strlen(mstr) + 1);
	}
	if (imflag) free(winstr);
	if (omflag) free(woutstr);
	if (mmflag) free(mstr);

	if (inlen > 0) {
		return ret;
	} else {
		return inlen;
	}
}

sj3_hantozen_euc(out, in)
u_char *out, *in;
{
	wchar16_t *winstr, *woutstr;
	u_char *mstr;
	int inlen, outlen, imflag = 0, omflag = 0, mmflag = 0, ret;

	inlen = strlen(in) + 1;
	if (current_locale == LC_CTYPE_EUC) {
		mstr = in;
	} else {
		if (inlen > sizeof(mtmp)) {
			mstr = (u_char *) malloc(inlen);
			mmflag = 1;
		} else {
			mstr = mtmp;
		}
		if (euctosjis(mstr, inlen, in, inlen) < 0) {
			if (mmflag) free(mstr);
			return -1;
		}
	}
	if ((inlen * sizeof(wchar16_t)) > sizeof(intmp)) {
		winstr = (wchar16_t *) malloc(inlen * sizeof(wchar16_t));
		if (!winstr) {
			if (mmflag) free(mstr);
			return -1;
		}
		imflag = 1;
	} else {
		winstr = (wchar16_t *) intmp;
	}
	if (mbstowcs(winstr, (char *)mstr, inlen) == -1) {
		if (mmflag) free(mstr);
		if (imflag) free(winstr);
		return -1;
	}
	outlen = inlen;
	if ((outlen * sizeof(wchar16_t)) > sizeof(outtmp)) {
		woutstr = (wchar16_t *) malloc(outlen * sizeof(wchar16_t));
		if (!woutstr) {
			if (mmflag) free(mstr);
			if (imflag) free(winstr);
			return -1;
		}
                omflag = 1;
	} else {
                woutstr = (wchar16_t *) outtmp;
	}
	ret = sj3_hantozen_w16(woutstr, winstr);
	if (mmflag) {
		free(mstr);
		mmflag = 0;
	}
	if (ret == 0 || ret == -1) {
		if (imflag) free(winstr);
		if (omflag) free(woutstr);
		return ret;
	}
	outlen = ret * 3;
	if (outlen > sizeof(mtmp)) {
		mstr = (u_char *) malloc(outlen);
		if (!mstr) {
			if (imflag) free(winstr);
			if (omflag) free(woutstr);
			return -1;
		}
		mmflag = 1;
	} else {
		mstr = mtmp;
	}
	if (wcstombs((char *)mstr, woutstr, outlen) == -1) {
		if (imflag) free(winstr);
		if (omflag) free(woutstr);
		if (mmflag) free(mstr);
		return -1;
	}
	if (current_locale == LC_CTYPE_EUC) {
		(void) memcpy(out, mstr, strlen(mstr) + 1);
	} else {
		inlen = sjistoeuc(out, outlen, mstr, outlen);
	}
	if (imflag) free(winstr);
	if (omflag) free(woutstr);
	if (mmflag) free(mstr);

	if (inlen > 0) {
		return ret;
	} else {
		return inlen;
	}
}

sj3_hantozen_mb(s1, s2)
u_char *s1, *s2;
{
	if (current_locale == LC_CTYPE_EUC) 
	  return sj3_hantozen_euc(s1, s2);
	else
  	  return sj3_hantozen(s1, s2);
}

sj3_hantozen_w16(s1, s2)
wchar16_t *s1, *s2;
{
	return sj_hantozen(s1, s2, wslen(s2));
}


sj_hantozen(s1, s2, len)
wchar16_t *s1, *s2;
int len;
{
	register u_short c1, cc, prefix;
	register wchar16_t c;
	register int i;
	int rlen;

	prefix = 0;
	rlen = 0;
	for (i = 0; i < len && *s2 != '\0'; i++) {
		c = *s2++;
		cc = sj_han2zen(c);
		if (cc & RKZEN) {
			if (prefix != 0 && (c1 = sj_addten(prefix, cc)) != 0) {
				s1 -= 1;
				rlen -= 1;
				cc = c1;
				prefix = 0;
			} else 
				prefix = cc;
			*s1++ = cc;
			rlen ++;
		} else {
			*s1++ = cc;
			rlen++;
			prefix = 0;
		}
	}
	*s1 = '\0';
	return(rlen);
}

sj_zen2han(c)
u_short c;
{
	register int i;
	register u_short cc;
	
	i = ZKATAn;
	cc = c & MASK;
	if (sj_ishira(c))
		i = cc - ZHIRAF2;
	else if (sj_iskata(c)) {
		i = cc - ZKATAF2;
	}
	if (i < ZKATAn)
		cc = HKtable[i];
	else {
		for (i = 0; i < 8; i++) {
			if (c == HKKtbl[i].zen) {
				cc = HKKtbl[i].han;
				break;
			}
		}
		if (i >= 8) {
			cc = c;
			for (i = 0; i < 95; i++) {
				if (c == HZtbl[i]) {
					cc = i + 0x20;
					break;
				}
			}
		}
	}
	return(cc);
}

extern int sj3_zentohan_w16();

sj3_zentohan(out, in)
u_char *out, *in;
{
	wchar16_t *winstr, *woutstr;
	u_char *mstr;
	int inlen, outlen, imflag = 0, omflag = 0, mmflag = 0, ret;

	inlen = strlen(in) + 1;
	if (inlen > sizeof(mtmp)) {
		mstr = (u_char *) malloc(inlen);
		if (!mstr) {
			return -1;
		}
		mmflag = 1;
	} else {
		mstr = mtmp;
	}
	if (current_locale == LC_CTYPE_EUC) {
		(void) euctosjis(mstr, inlen, in, inlen);
	} else {
		(void) memcpy(mstr, in, inlen);
	}
	if (inlen > sizeof(intmp)) {
		winstr = (wchar16_t *) malloc(((inlen / 2) + 1) * sizeof(wchar16_t));
		if (!winstr) {
			if (mmflag) free(mstr);
			return -1;
		}
		imflag = 1;
	} else {
		winstr = (wchar16_t *) intmp;
	}
	if (mbstowcs(winstr, (char *)mstr, inlen) == -1) {
		if (mmflag) free(mstr);
		if (imflag) free(winstr);
		return -1;
	}
	if (mmflag) {
		free(mstr);
		mmflag = 0;
	}
	outlen = ((inlen - 1) / 2) + 1;
	if (outlen * sizeof(wchar16_t) > sizeof(outtmp)) {
		woutstr = (wchar16_t *) malloc(outlen * sizeof(wchar16_t));
		if (!woutstr) {
			if (imflag) free(winstr);
			return -1;
		}
                omflag = 1;
	} else {
                woutstr = (wchar16_t *) outtmp;
	}
	ret = sj3_zentohan_w16(woutstr, winstr);
	if (ret == 0 || ret == -1) {
		if (imflag) free(winstr);
		if (omflag) free(woutstr);
		return ret;
	}
	outlen = ret * 3;
	if (current_locale == LC_CTYPE_EUC) {
		if (outlen > sizeof(mtmp)) {
			mstr = (u_char *) malloc(outlen);
			mmflag = 1;
		} else {
			mstr = mtmp;
		}
	} else {
		mstr = out;
	}
	if (wcstombs((char *)mstr, woutstr, outlen) == -1) {
		if (imflag) free(winstr);
		if (omflag) free(woutstr);
		return -1;
	}
	if (current_locale == LC_CTYPE_EUC) {
		if (euctosjis(out, outlen, mstr, outlen) < 0) {
			if (imflag) free(winstr);
			if (omflag) free(woutstr);
			if (mmflag) free(mstr);
			return -1;
		}
	}
			
	if (imflag) free(winstr);
	if (omflag) free(woutstr);
	if (mmflag) free(mstr);

	return ret;
}

sj3_zentohan_euc(out, in)
u_char *out, *in;
{
	wchar16_t *winstr, *woutstr;
	u_char *mstr;
	int inlen, outlen, imflag = 0, omflag = 0, mmflag = 0, ret;

	inlen = strlen(in) + 1;
	if (inlen > sizeof(mtmp)) {
		mstr = (u_char *) malloc(inlen);
		if (!mstr) {
			return -1;
		}
		mmflag = 1;
	} else {
		mstr = mtmp;
	}
	if (current_locale == LC_CTYPE_EUC) {
		(void) euctosjis(mstr, inlen, in, inlen);
	} else {
		(void) memcpy(mstr, in, inlen);
	}
	if (inlen > sizeof(intmp)) {
		winstr = (wchar16_t *) malloc(((inlen / 2) + 1) * sizeof(wchar16_t));
		if (!winstr) {
			if (mmflag) free(mstr);
			return -1;
		}
		imflag = 1;
	} else {
		winstr = (wchar16_t *) intmp;
	}
	if (mbstowcs(winstr, (char *)mstr, inlen) == -1) {
		if (mmflag) free(mstr);
		if (imflag) free(winstr);
		return -1;
	}
	if (mmflag) {
		free(mstr);
		mmflag = 0;
	}
	outlen = ((inlen - 1) / 2) + 1;
	if (outlen * sizeof(wchar16_t) > sizeof(outtmp)) {
		woutstr = (wchar16_t *) malloc(outlen * sizeof(wchar16_t));
		if (!woutstr) {
			if (mmflag) free(mstr);
			if (imflag) free(winstr);
			return -1;
		}
                omflag = 1;
	} else {
                woutstr = (wchar16_t *) outtmp;
	}
	ret = sj3_zentohan_w16(woutstr, winstr);
	if (ret == 0 || ret == -1) {
		if (mmflag) free(mstr);
		if (imflag) free(winstr);
		if (omflag) free(woutstr);
		return ret;
	}
	outlen = ret * 3;
	if (current_locale == LC_CTYPE_EUC) {
		mstr = out;
	} else {
		if (outlen > sizeof(mtmp)) {
			mstr = (u_char *) malloc(outlen);
			mmflag = 1;
		} else {
			mstr = mtmp;
		}
	}
	if (wcstombs((char *)mstr, woutstr, outlen) == -1) {
		if (imflag) free(winstr);
		if (omflag) free(woutstr);
		return -1;
	}
	if (current_locale == LC_CTYPE_SHIFTJIS) {
		if (sjistoeuc(out, outlen, mstr, outlen) < 0) {
			if (imflag) free(winstr);
			if (omflag) free(woutstr);
			if (mmflag) free(mstr);
			return -1;
		}
	}
	if (imflag) free(winstr);
	if (omflag) free(woutstr);
	if (mmflag) free(mstr);

	return ret;
}

sj3_zentohan_mb(s1, s2)
u_char *s1, *s2;
{
	if (current_locale == LC_CTYPE_EUC)
	  return sj3_zentohan_euc(s1, s2);
	else 
	  return sj3_zentohan(s1, s2);
}

sj3_zentohan_w16 (s1, s2)
wchar16_t *s1, *s2;
{
	return sj_zentohan(s1, s2, wslen(s2));
}

sj_zentohan(s1, s2, len)
wchar16_t *s1, *s2;
int len;
{
	register u_short cc;
	register wchar16_t c;
	register int i, rlen;

	rlen = 0;
	for (i = 0; i < len && *s2 != (wchar16_t) '\0'; i++) {
		c = *s2++;
		if (WcIsZENKAKU(c)) {
			cc = sj_zen2han(c);
			if ((cc >> 8) && (cc != c)) {
				*s1++ = (cc >> 8);
				rlen++;
				*s1++ = cc & 0xff;
				rlen++;   
				continue;
			}

		} else 
			cc = c;
		if (cc & RKZEN) {
			*s1++ = (cc >> 8);
			rlen++;
		}
		*s1++ = (wchar16_t)cc;
		rlen++;
	}
	*s1 = (wchar16_t) '\0';
	return(rlen);
}

sj_tokata(c)
wchar16_t c;
{
	if (sj_ishira(c)) {
		c += 0x0100;
	}
	return(c);
}

sj_tohira(c)
wchar16_t c;
{
	if (sj_iskata(c) && (short) c <= (short) 0xa5f3) {
		c -= 0x0100;
	}
	return(c);
}

sj_htok(s1, s2)
wchar16_t *s1, *s2;
{
	register wchar16_t cc;
	register wchar16_t c;

	while ((c = *s2++) != (wchar16_t) '\0') {
		if (WcIsX0208(c)) {
			cc = sj_tokata(c);
			*s1++ = cc;
		} else
			*s1++ = c;
	}
	*s1 = (wchar16_t) '\0';
}

sj_ktoh(s1, s2)
wchar16_t *s1, *s2;
{
	register wchar16_t cc;
	register wchar16_t c;

	while ((c = *s2++) != (wchar16_t) '\0') {
		if (WcIsX0208(c)) {
			cc = sj_tohira(c);
			*s1++ = cc;
		} else
			*s1++ = c;
	}
	*s1 = (wchar16_t) '\0';
}
