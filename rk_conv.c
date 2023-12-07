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
 * $SonyRCSfile: rk_conv.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:59 $
 */


#ifndef lint
static char rcsid[] = "$Header: /export/work/contrib/sj3/sj3rkcv/RCS/rk_conv.c,v 1.14 1994/06/03 07:42:16 notanaka Exp $";
#endif


#include "sj_sysvdef.h"
#include "wchar16.h"
#include <stdio.h>
#include <ctype.h>
#ifdef SVR4
#include <string.h>
#else
#include <strings.h>
#endif
#ifdef __sony_news
#include <jctype.h>
#endif
#include "kctype.h"
#include "rk.h"

#if defined(__sony_news) && defined(SVR4)
#define wscmp sj3_wscmp16
#define wsncmp sj3_wsncmp16
#define wsncpy sj3_wsncpy16
#define wscpy sj3_wscpy16
#define wslen sj3_wslen16
#define mbstowcs sj3_mbstowcs16
#define wcstombs sj3_wcstombs16
#endif

extern int current_locale;

#define	ENDMARK(c) (c == SPACE || c == TAB)	   
#define	SKLINE(c)  (c == EOL || c == NSTR)	   
#define	TOUPPER(c) (islower(c) ? toupper(c) : (c)) 
                    
#define	WTOUPPER(c) ((wchar16_t) (islower(((c) & 0xff)) ? toupper(((c) & 0xff)) : ((c) & 0xff))) 

#define RKSIZE		256
#define RKINCZ		32
typedef struct rkbufrec {
	RkTablW16 *rkbuf;
	RkTablW16 *rkend;
	struct rkbufrec *rkbufp;
} RkbufRec;

#define STRSIZE		1024
#define STRINCZ		256
typedef struct strbufrec {
	wchar16_t *strbuf;
	wchar16_t *strend;
	struct strbufrec *strbufp;
} StrbufRec;

#define YMISIZE		512
#define YMIINCZ		64
typedef struct ymibufrec {
	wchar16_t *ymibuf;
	wchar16_t *ymiend;
	struct ymibufrec *ymibufp;
} YmibufRec;


#define	SJ3_NO		-1
#define SJ3_SJIS	0
#define SJ3_EUC		1

RkTabl *sj3_rcode[MAXCODE];	
RkTablW16 *sj3_rcode_w16[MAXCODE];
RkTablW16 *rktblp;		
wchar16_t rline[MAXLLEN];	
wchar16_t lstr[MAXLLEN];	
static wchar16_t intmp[MAXLLEN];
static wchar16_t outtmp[MAXLLEN];
static u_char  mtmp[MAXLLEN];
static int rk_erase;	
static int rk_errin = 0;
static int rk_cflag;	
static int rk_klen;	
static int file_code = SJ3_NO; 


static RkbufRec rkpbuf, *rkpp = NULL;
static RkTablW16 *rkhp;
static StrbufRec strpbuf, *strpp;
static wchar16_t *shp;
static YmibufRec ymipbuf, *ymipp;
static u_short *yhp;


sj3_rkcode(code)
int code;
{
	if (code)
		file_code = SJ3_NO;
}




sj3_rkinit2(rkfile, erase)
char *rkfile;
int erase;
{
	rk_erase = erase;
	return(sj3_rkinit_mb(rkfile));
}

rcode_sjis_init()
{
	int i, rkeylen = 0, ryomilen = 0, rstrlen = 0;
	int klen, ylen, slen, tablnum = 0, tablsize;
	u_char *mbstr, *mptr, mtmp[BUFSIZ], mtmp2[BUFSIZ];
	u_short *wcstr, *wcptr;
	RkTabl *tabl, *tabl_next, *tabl_prev;
	RkTablW16 *tabl_w16;

	tablsize = sizeof(RkTabl);


	for (i = 0; i < MAXCODE; i++) {
		if (sj3_rcode_w16[i]) {
			tabl_w16 = sj3_rcode_w16[i];
			while (tabl_w16) {
				tablnum++;
				if (tabl_w16->r_key)
				  rkeylen += wslen(tabl_w16->r_key) + 1;
				if (tabl_w16->r_str)
				  rstrlen += wslen(tabl_w16->r_str) + 1;
				if (tabl_w16->k_yomi)
				  ryomilen += wslen(tabl_w16->k_yomi) + 1;
				tabl_w16 = tabl_w16->next;
			}
		}
	}
        if (!tablnum)
                return 1;
	tabl = (RkTabl *) malloc(tablnum * tablsize);
        if (!tabl) {
		return 0;
	}
	memset(tabl, '\0', tablnum * tablsize);
	mbstr = (u_char *) malloc(rkeylen + rstrlen);
        if (!mbstr) {
		free(tabl);
		return 0;
	}
	memset(mbstr, '\0', rkeylen + rstrlen);
	wcptr = wcstr = (u_short *) malloc(ryomilen * sizeof(u_short));
        if (!wcstr) {
		free(tabl);
		free(mbstr);
		return 0;
	}
        memset(wcptr, '\0', ryomilen * sizeof(u_short));

	rkeylen *= 2;
	rstrlen *= 2;
	for (i = 0; i < MAXCODE; i++) {
		if (sj3_rcode_w16[i]) {
			tabl_w16 = sj3_rcode_w16[i];
			tabl_next = sj3_rcode[i] = tabl;
			while (tabl_w16) {
				tabl += 1; 
				if (tabl_w16->r_key) {
					if (current_locale == LC_CTYPE_EUC) {
						mptr = mtmp;
					} else {
						mptr = mbstr;
					}
					if (wcstombs((char *)mptr, tabl_w16->r_key, BUFSIZ) == -1) {
						free(tabl);
						free(mbstr);
						free(wcstr);
						return 0;
					}
					if (current_locale == LC_CTYPE_EUC) {
						if ((klen = euctosjis(mbstr, rkeylen, mtmp, sizeof(mtmp))) < 0) {
							free(tabl);
							free(mbstr);
							free(wcstr);
							return 0;
						}
					} else {
						klen = strlen((char *)mbstr);
					}
					tabl_next->r_key = mbstr;
					mbstr += klen + 1;
					rkeylen -= klen + 1;
				} else {
					tabl_next->r_key = NULL;
				}
				if (tabl_w16->r_str) {
					if (current_locale == LC_CTYPE_EUC) {
						mptr = mtmp;
					} else {
						mptr = mbstr;
					}
					if (wcstombs((char *)mptr, tabl_w16->r_str, BUFSIZ) == -1) {
						free(tabl);
						free(mbstr);
						free(wcstr);
						return 0;
					}
					if (current_locale == LC_CTYPE_EUC) {
						if ((slen = euctosjis(mbstr, rstrlen, mtmp, sizeof(mtmp))) < 0) {
							free(tabl);
							free(mbstr);
							free(wcstr);
							return 0;
						}
					} else {
						slen = strlen((char *)mbstr);
					}
					tabl_next->r_str = mbstr;
					mbstr += slen + 1;
					rstrlen -= slen + 1;
				} else {
					tabl_next->r_str = NULL;
				}
				if (tabl_w16->k_yomi) {
					mptr = mtmp;
					if (wcstombs((char *)mptr, tabl_w16->k_yomi, BUFSIZ) == -1) {
						free(tabl);
						free(mbstr);
						free(wcstr);
						return 0;
					}
					if (current_locale == LC_CTYPE_EUC) {
						if (euctosjis(mtmp2, sizeof(mtmp2), mtmp, sizeof(mtmp)) < 0) {
							free(tabl);
							free(mbstr);
							free(wcstr);
							return 0;
						}
						mptr = mtmp2;
					} else {
						mptr = mtmp;
					}
					tabl_next->k_yomi = wcptr;
					ylen = 0;
					while (*mptr) {
						if (issjis1(*mptr) && issjis2(mptr[1])) {
							*wcptr = (*mptr << 8) + mptr[1];
							mptr++;
						} else {
							*wcptr = *mptr;
						}
						wcptr++;
						mptr++;
						ylen++;
					}
					*wcptr++ = 0;
					ryomilen -= ylen + 1;
				} else {
					tabl_next->k_yomi = NULL;
				}
				tabl_w16 = tabl_w16->next;
				tabl_next->next = tabl;
				tabl_prev = tabl_next;
				tabl_next = tabl;
			}
			tabl_prev->next = NULL;
		}
	}
	return 1;
				
}

rcode_euc_init()
{
	int i, rkeylen = 0, ryomilen = 0, rstrlen = 0;
	int klen, ylen, slen, tablnum = 0, tablsize;
	u_char *mbstr, *mptr, mtmp[BUFSIZ], mtmp2[BUFSIZ];
	u_short *wcstr, *wcptr;
	RkTabl *tabl, *tabl_next, *tabl_prev;
	RkTablW16 *tabl_w16;

	tablsize = sizeof(RkTabl);


	for (i = 0; i < MAXCODE; i++) {
		if (sj3_rcode_w16[i]) {
			tabl_w16 = sj3_rcode_w16[i];
			while (tabl_w16) {
				tablnum++;
				if (tabl_w16->r_key)
				  rkeylen += wslen(tabl_w16->r_key) + 1;
				if (tabl_w16->r_str)
				  rstrlen += wslen(tabl_w16->r_str) + 1;
				if (tabl_w16->k_yomi)
				  ryomilen += wslen(tabl_w16->k_yomi) + 1;
				tabl_w16 = tabl_w16->next;
			}
		}
	}
        if (!tablnum)
                return 1;
	tabl = (RkTabl *) malloc(tablnum * tablsize);
        if (!tabl) {
		return 0;
	}
	memset(tabl, '\0', tablnum * tablsize);
	mbstr = (u_char *) malloc(rkeylen + rstrlen);
        if (!mbstr) {
		free(tabl);
		return 0;
	}
	memset(mbstr, '\0', rkeylen + rstrlen);
	wcptr = wcstr = (u_short *) malloc(ryomilen * sizeof(u_short));
        if (!wcstr) {
		free(tabl);
		free(mbstr);
		return 0;
	}
        memset(wcptr, '\0', ryomilen * sizeof(u_short));

	rkeylen *= 2;
	rstrlen *= 2;
	for (i = 0; i < MAXCODE; i++) {
		if (sj3_rcode_w16[i]) {
			tabl_w16 = sj3_rcode_w16[i];
			tabl_next = sj3_rcode[i] = tabl;
			while (tabl_w16) {
				tabl += 1; 
				if (tabl_w16->r_key) {
					if (current_locale == LC_CTYPE_SHIFTJIS) {
						mptr = mtmp;
					} else {
						mptr = mbstr;
					}
					if (wcstombs((char *)mptr, tabl_w16->r_key, BUFSIZ) == -1) {
						free(tabl);
						free(mbstr);
						free(wcstr);
						return 0;
					}
					if (current_locale == LC_CTYPE_SHIFTJIS) {
						if ((klen = euctosjis(mbstr, rkeylen, mtmp, sizeof(mtmp))) < 0) {
							free(tabl);
							free(mbstr);
							free(wcstr);
							return 0;
						}
					} else {
						klen = strlen((char *)mbstr);
					}
					tabl_next->r_key = mbstr;
					mbstr += klen + 1;
					rkeylen -= klen + 1;
				} else {
					tabl_next->r_key = NULL;
				}
				if (tabl_w16->r_str) {
					if (current_locale == LC_CTYPE_SHIFTJIS) {
						mptr = mtmp;
					} else {
						mptr = mbstr;
					}
					if (wcstombs((char *)mptr, tabl_w16->r_str, BUFSIZ) == -1) {
						free(tabl);
						free(mbstr);
						free(wcstr);
						return 0;
					}
					if (current_locale == LC_CTYPE_SHIFTJIS) {
						if ((slen = sjistoeuc(mbstr, rstrlen, mtmp, sizeof(mtmp))) < 0) {
							free(tabl);
							free(mbstr);
							free(wcstr);
							return 0;
						}
					} else {
						slen = strlen((char *)mbstr);
					}
					tabl_next->r_str = mbstr;
					mbstr += slen + 1;
					rstrlen -= slen + 1;
				} else {
					tabl_next->r_str = NULL;
				}
				if (tabl_w16->k_yomi) {
					mptr = mtmp;
					if (wcstombs((char *)mptr, tabl_w16->k_yomi, BUFSIZ) == -1) {
						free(tabl);
						free(mbstr);
						free(wcstr);
						return 0;
					}
					if (current_locale == LC_CTYPE_SHIFTJIS) {
						if (sjistoeuc(mtmp2, sizeof(mtmp2), mtmp, sizeof(mtmp)) < 0) {
							free(tabl);
							free(mbstr);
							free(wcstr);
							return 0;
						}
						mptr = mtmp2;
					} else {
						mptr = mtmp;
					}
					tabl_next->k_yomi = wcptr;
					ylen = 0;
					while (*mptr) {
						if (iseuc(*mptr) && iseuc(mptr[1])) {
							*wcptr = (*mptr << 8) + mptr[1];
							mptr++;
						} else {
							*wcptr = *mptr;
						}
						wcptr++;
						mptr++;
						ylen++;
					}
					*wcptr++ = 0;
					ryomilen -= ylen + 1;
				} else {
					tabl_next->k_yomi = NULL;
				}
				tabl_w16 = tabl_w16->next;
				tabl_next->next = tabl;
				tabl_prev = tabl_next;
				tabl_next = tabl;
			}
			tabl_prev->next = NULL;
		}
	}
	return 1;
				
}

sj3_rkinit(rkfile)
char *rkfile;
{
	return sj3_rkinit_sub(rkfile, rcode_sjis_init);
}

sj3_rkinit_euc(rkfile)
char *rkfile;
{
	return sj3_rkinit_sub(rkfile, rcode_euc_init);
}

sj3_rkinit_mb(rkfile)
char *rkfile;
{
	if (current_locale == LC_CTYPE_EUC)
	  return sj3_rkinit_sub(rkfile, rcode_euc_init);
	else
	  return sj3_rkinit_sub(rkfile, rcode_sjis_init);
}

sj3_rkinit_sub(rkfile, mbfunc)
char *rkfile;
int (*mbfunc)();
{
	register char *p;
	int len, klen, rlen, retv;
	FILE *fp, *fopen();
	char line[MAXLEN + 1];
	wchar16_t rkey[MAXWLEN + 1], rstr[MAXWLEN + 1];
	u_short kstr[MAXWLEN + 1];
	char *strcpy(), *getkey(), *rkgetyomi();
	RkTablW16 *rktp, *mktable();

	if (rkfile == NULL || *rkfile == NSTR)
		fp = stdin;
	else if ((fp = fopen(rkfile, "r")) == NULL)
		return(1);

	cltable();
	line[MAXLEN - 1] = EOL;
	retv = 0;

	
	while ((p = fgets(line, MAXLEN + 1, fp)) != NULL) {
		if (line[MAXLEN - 1] != EOL) {
			line[MAXLEN - 1] = EOL;
			continue;
		}
		if (SKLINE(*p) || *p == '#')
			continue;
		if ((p = getkey(p, rkey, &len)) == NULL)
			continue;
		if ((p = rkgetyomi(p, kstr, &klen)) == NULL)
			continue;
		p = getkey(p, rstr, &rlen);
		if (rlen > 0 && chk_rstr(rstr, rkey, rlen, len))
			rlen = 0;

		if ((rktp = mktable(rkey, len)) == NULL) {
			fprintf(stderr, "Can't allocate memory for table\n");
			retv = FAIL;
			break;
		}
		if (rktp->k_yomi != NULL) 
			continue;
		if (stradd(&(rktp->r_key), &rkey[1], len) == -1) {
			fprintf(stderr, "Can't allocate memory for char\n");
			retv = FAIL;
			break;
		}
		if (kstradd(&(rktp->k_yomi), kstr, klen + 1) == -1) {
			fprintf(stderr, "Can't allocate memory for short\n");
			retv = FAIL;
			break;
		}
		if (stradd(&(rktp->r_str), rstr, rlen + 1) == -1) {
			fprintf(stderr, "Can't allocate memory for char\n");
			retv = FAIL;
			break;
		}

	}
	fclose(fp);
	if (!(*mbfunc)())
	    retv = FAIL;
	sj3_rkclear();
	return(retv);
}



char *
getkey(istr, ostr, len)
char *istr;
wchar16_t *ostr;
int *len;
{
	register char c, *p;
	register int i;

	while (ENDMARK(*istr))
		istr++;

	p = istr;
	i = 0;
	while (!ENDMARK(*p)) {
		if (SKLINE(*p))
			break;
		c = *p++;
		
		if ( c == BSL) {
			if (SKLINE(*p))
				break;
			c = *p++;
			switch (c) {
			case 'n' :
				*ostr++ = (wchar16_t) EOL;
				break;
			case 't' :
				*ostr++ = (wchar16_t) TAB;
				break;
			default :
				*ostr++ = (wchar16_t) TOUPPER(c);
			}
		} else if ( c == '^') {
			if (*p >= '@' && *p <= '_') {
				*ostr++ = (wchar16_t) (*p - '@');
				p++;
			} else if (*p == '?') {
				*ostr++ = (wchar16_t) 0x7f;
				p++;
			} else
				*ostr++ = (wchar16_t) '^';
		} else
			*ostr++ = (wchar16_t) TOUPPER(c);
		if (++i > MAXWLEN) {
			i = 0;
			break;
		}
	}
	*ostr = (wchar16_t) NSTR;
	*len = i;
	if (i > 0)
		return(p);
	return(NULL);
}



char *
rkgetyomi(istr, ostr, len)
char *istr;
wchar16_t *ostr;
int *len;
{
	register u_char c;	
	register u_char *p;
	register int i;
	u_short cc;

	while (ENDMARK(*istr))
		istr++;

	p = (u_char *)istr;
	i = 0;
	while (!ENDMARK(*p)) {
		if (SKLINE(*p))
			break;
		cc = c = *p++;
		if (file_code == SJ3_SJIS) {
			if (issjis1(c) && issjis2(*p)) {
				cc = (c << 8) + (*p & MASK);
				p++;
				cc = sjis2euc(cc);
			}
		} else if (file_code == SJ3_EUC) {
			if (iseuc(c) && iseuc(*p)) {
				cc = (c << 8) + (*p & MASK);
				p++;
			} else if (iseuckana(c) && iskana2(*p)) {
				cc = *p++;
			}
		        else if (IsEUCHojo(c) && iseuc(*p) && iseuc(p[1])) {
				cc = WcSetX0212(*p, p[1]);
				p += 2;
			}
		} else {
			if (issjis1(c) && issjis2(*p)) {
				file_code = SJ3_SJIS;
				p--;
				continue;
			}
			if ((iseuc(c) && iseuc(*p)) || (iseuckana(c) &&
				iskana2(*p)) || (IsEUCHojo(c) && iseuc(*p) &&
						 iseuc(p[1]))) {
				file_code = SJ3_EUC;
				p--;
				continue;
			}
		}
		*ostr++ = (wchar16_t) cc;
		if (++i > MAXWLEN) {
			i = 0;
			break;
		}
	}
	*ostr = (wchar16_t) 0;
	*len = i;
	if (i > 0)
		return((char *)p);
	return(NULL);
}



cltable()
{
	register int i;

	if (rkpp != NULL) {
	    for (i = 0; i < MAXCODE; i++) {
			sj3_rcode_w16[i] = NULL;
			sj3_rcode[i] = NULL;
	    }
	    rkpp = &rkpbuf;
	    rkhp = rkpp->rkbuf;
	    strpp = &strpbuf;
	    shp = strpp->strbuf;
	    ymipp = &ymipbuf;
	    yhp = ymipp->ymibuf;
	}
}



chk_rstr(rstr, rkey, rlen, klen)
wchar16_t *rstr, *rkey;
int rlen, klen;
{
	register int i;
	register wchar16_t *p;

	if (rlen >= klen)
		return(1);
	p = rkey;
	while (rlen <= klen) {
		if (wsncmp(rstr, p, rlen) == 0)
			return(0);
		klen--;
		p++;
	}
	return(1);
}



RkTablW16 *
rkalloc()
{
	register RkTablW16 *rkp;
	register RkbufRec *rkbrp;

	if (rkpp == NULL) {
		rkp = (RkTablW16 *)malloc(RKSIZE * sizeof(RkTablW16));
		if (rkp == NULL)
			return(NULL);
		rkhp = rkp;
		rkpp = &rkpbuf;
		rkpp->rkbuf = rkp;
		rkpp->rkend = rkp + RKSIZE;
	} else if (rkhp >= rkpp->rkend) {
		rkp = (RkTablW16 *)malloc(RKINCZ * sizeof(RkTablW16));
		rkbrp = (RkbufRec *)malloc(sizeof(RkbufRec));
		if (rkp == NULL || rkbrp == NULL)
			return(NULL);
		rkhp = rkp;
		rkpp->rkbufp = rkbrp;
		rkpp = rkbrp;
		rkpp->rkbuf = rkp;
		rkpp->rkend = rkp + RKINCZ;
	}
	rkp = rkhp++;
	
	rkp->r_key = NULL;
	rkp->k_yomi = NULL;
	rkp->r_str = NULL;
	rkp->next = NULL;
	return(rkp);
}



stradd(cp, str, len)
wchar16_t **cp;
wchar16_t *str;
int len;
{
	register wchar16_t *strp;
	register StrbufRec *sbrp;

	if (len <= 1)
		return(0);
	if (strpp == NULL) {
		strp = (wchar16_t *)malloc(STRSIZE * sizeof(wchar16_t));
		if (strp == NULL)
			return(-1);
		shp = strp;
		strpp = &strpbuf;
		strpp->strbuf = strp;
		strpp->strend = strp + STRSIZE;
	} else if (shp + len >= strpp->strend) {
		strp = (wchar16_t *)malloc(STRINCZ * sizeof(wchar16_t));
		sbrp = (StrbufRec *)malloc(sizeof(StrbufRec));
		if (strp == NULL || sbrp == NULL)
			return(-1);
		shp = strp;
		strpp->strbufp = sbrp;
		strpp = sbrp;
		strpp->strbuf = strp;
		strpp->strend = strp + STRINCZ;
	}
	*cp = shp;
	wscpy(shp, str);
	shp += len;
	return(0);
}
		


kstradd(cp, kstr, len)
wchar16_t **cp, *kstr;
int len;
{
	register int i;
	register wchar16_t *usp;
	register YmibufRec *ybrp;

	if (len <= 1)
		return(0);
	if (ymipp == NULL) {
		usp = (wchar16_t *)malloc(YMISIZE * sizeof(wchar16_t));
		if (usp == NULL)
			return(-1);
		yhp = usp;
		ymipp = &ymipbuf;
		ymipp->ymibuf = usp;
		ymipp->ymiend = usp + YMISIZE;
	} else if (yhp + len >= ymipp->ymiend) {
		usp = (wchar16_t *)malloc(YMIINCZ * sizeof(wchar16_t));
		ybrp = (YmibufRec *)malloc(sizeof(YmibufRec));
		if (usp == NULL || ybrp == NULL)
			return(-1);
		yhp = usp;
		ymipp->ymibufp = ybrp;
		ymipp = ybrp;
		ymipp->ymibuf = usp;
		ymipp->ymiend = usp + YMIINCZ;
	}
	*cp = yhp;
	for (i = 0; i < len; i++)
		*yhp++ = *kstr++;
	return(0);
}



RkTablW16 *
mktable(key, len)
wchar16_t *key;
int len;
{
	register int i;
	register RkTablW16 *nrktp, *orktp;
	RkTablW16 *rktp;
	u_int code;

	nrktp = NULL;
	orktp = NULL;
	code = *key++ & MASK;
	rktp = sj3_rcode_w16[code];
	while (rktp != NULL) {
		if (rktp->r_key == NULL) {
			if (*key == NSTR)
				return(rktp);
			nrktp = rktp;
			break;
		}
		if ((i = wscmp(key, rktp->r_key)) == 0)
			return(rktp);
		if (i > 0 && (wslen(rktp->r_key) < len)) {
			nrktp = rktp;
			break;
		}
		orktp = rktp;
		rktp = orktp->next;
	}
	if ((rktp = rkalloc()) == NULL)
		return(NULL);
	if (sj3_rcode_w16[code] == NULL)
		sj3_rcode_w16[code] = rktp;
	else {
		if (nrktp != NULL) {
			rktp->next = nrktp;
			if (orktp == NULL)
				sj3_rcode_w16[code] = rktp;
			else
				orktp->next = rktp;
		} else
			orktp->next = rktp;
	}
	return(rktp);
}



sj3_rkebell(err)
int err;
{
	rk_errin = err;
}



sj3_rkclear()
{
	rline[0] = NSTR;
	lstr[0] = NSTR;
	rktblp = NULL;
	rk_klen = 0;
	sj3_rkreset();
}

sj3_rkreset()
{
	rk_cflag = -1;
}



sj3_rkconvc(c, rkstr)
wchar16_t c;
u_int *rkstr;
{
	register int i;
	register wchar16_t *p, *q;
	int len;
	wchar16_t wstr[MAXLLEN];
	u_int kstr[MAXLLEN], *kp;

	len = wslen(rline);
	if (c == rk_erase) {
		if (rk_cflag == 0) {
			for(i = 0; i < len; i++)
				*rkstr++ = ERRCODE + rk_erase;
			for (i = 0; i < rk_klen; i++)
				*rkstr++ = rk_erase;
			p = lstr;
			q = rline;
			while(*p != NSTR) {
				*q++ = *p;
				*rkstr++ = ERRCODE + *p++;
			}
			*q = NSTR;
			rktblp = NULL;
			rk_cflag = -1;
		} else {
			if (len <= 0)
				*rkstr++ = rk_erase;
			else
				*rkstr++ = ERRCODE + rk_erase;
			rk_cflag--;
			if (len <= 1) {
				rktblp = NULL;
				rline[0] = NSTR;
			} else
				rline[--len] = NSTR;
		}
	} else { 
		rline[len] = c;
		rline[len + 1] = NSTR;
		wscpy(wstr, rline);
		if ((i = sj3_rkconv2(wstr, kstr, len)) == CONTINUE) {
			if (rk_cflag >= 0)
				rk_cflag++;
			*rkstr++ = ERRCODE + c;
		} else if (i == NOMACH) {
			rline[len] = NSTR;
			*rkstr++ = ERRBEL;
		} else {
			rk_cflag = 0;
			rk_klen = i;
			for (i = 0; i < len; i++)
				*rkstr++ = ERRCODE + rk_erase;
			kp = kstr;
			while (*kp != 0)
				*rkstr++ = *kp++;
			wscpy(lstr, rline);
			lstr[len] = NSTR;
			p = wstr;
			q = rline;
			while(*p != NSTR) {
				*rkstr++ = ERRCODE + *p;
				*q++ = *p++;
			}
			*q = NSTR;
		}
	}
	*rkstr = RKEND;
}



sj3_rkconv2(wstr, kstr, wlen)
wchar16_t *wstr;
u_int *kstr;
int wlen;
{

	register int i;
	register wchar16_t *p, *q, *s;
	int rlen, len;
	wchar16_t svstr[MAXLLEN];
	RkTablW16 *rktp;
	u_short *kp;

	len = 0;
	if ((rktp = rktblp) == NULL)
		rktp = sj3_rcode_w16[WTOUPPER(*wstr)];
	if (*wstr > (wchar16_t) 0xff) rktp = NULL;
	while(1) {
		while(rktp != NULL) {
			rlen = rkmatch((wstr + 1), rktp->r_key, wlen);
			if (rlen == CONTINUE) {
				rktblp = rktp;
				break;
			} else if (rlen != NOMACH) {
				kp = rktp->k_yomi;
				if ((s = rktp->r_str) != NULL)
					i = wslen(s);
				else 
					i = 0;
				*kstr++ = SetMojilen(rlen - i) + *kp++;
				len++;
				while(*kp != 0) {
					*kstr++ = *kp++;
					len++;
				}
				p = (wstr + rlen);
				q = svstr;
				if (i > 0) {
					p -= i;
					while (*s != NSTR) {
						if (*s == WTOUPPER(*p))
							*q = *p;
						else
							*q = *s;
						q++;
						p++;
						s++;
					}
				}
				if (rlen <= wlen) {
					while (*p != NSTR) {
						*q++ = *p++;
					}
					*q = NSTR;
					wscpy(wstr, svstr);
					rktp = sj3_rcode_w16[WTOUPPER(*wstr)];
					wlen = wslen(wstr);
					wlen--;
					continue;
				}
				*q = NSTR;
				wscpy(wstr, svstr);
				rktblp = NULL;
				break;
			}
			rktp = rktp->next;
		}
		if (rktp == NULL) {
			if (wlen <= 0) {
				*kstr++ = SetMojilen(1) + *wstr;
				*wstr = NSTR;
				rktblp = NULL;
				len++;
			} else if (rk_errin) {
				len = NOMACH;
			} else {
				*kstr++ = SetMojilen(1) + *wstr;
				len++;
				p = wstr;
				for (i = 0; i < wlen; i++) {
					*p = *(p + 1);
					p++;
				}
				*p = NSTR;
				rktp = sj3_rcode_w16[WTOUPPER(*wstr)];
				if (*wstr > (wchar16_t) 0xff) rktp = NULL;
				wlen--;
				continue;
			}
		}
		*kstr = 0;
		break;
	}
	return(len);
}




extern int sj3_rkconv_w16();

sj3_rkconv(romaji, kana)
u_char *romaji;
u_char *kana;
{
	wchar16_t *wstr;
	int len, mflag = 0, ret;

	len = strlen((char *)romaji) + 1;
	if (len > (sizeof(intmp) / sizeof(wchar16_t))) {
		wstr = (wchar16_t *) malloc(len * sizeof(wchar16_t));
		if (!wstr) return -1;
		mflag = 1;
	} else {
		wstr = (wchar16_t *) intmp;
	}
	if (mbstowcs(wstr, (char *)romaji, len) == -1) {
		if (mflag) free(wstr);
		return -1;
	}
	ret = sj3_rkconv_w16(wstr, outtmp);
	if (ret == 0 || ret == -1) {
		if (mflag) free(wstr);
		return ret;
	}
	if (wcstombs((char *)mtmp, outtmp, sizeof(outtmp)) == -1) {
		if (mflag) free(wstr);
		return -1;
	}
	if (wcstombs((char *)romaji, wstr, len) == -1) {
		if (mflag) free(wstr);
		return -1;
	}
	if (current_locale == LC_CTYPE_EUC) {
		len = euctosjis(kana, MAXLLEN*2, mtmp, sizeof(mtmp));
	} else {
		(void) memcpy(kana, mtmp, strlen((char *)mtmp) + 1);
	}

	if (mflag) free(wstr);
	if (len > 0) {
		return ret;
	} else {
		return len;
	}
}

sj3_rkconv_euc(romaji, kana)
u_char *romaji;
u_char *kana;
{
	wchar16_t *wstr;
	int len, mflag = 0, ret;

	len = strlen((char *)romaji) + 1;
	if (len > sizeof(intmp)) {
		wstr = (wchar16_t *) malloc(len * sizeof(wchar16_t));
		if (!wstr) return -1;
		mflag = 1;
	} else {
		wstr = (wchar16_t *) intmp;
	}
	if (mbstowcs(wstr, (char *)romaji, len) == -1) {
		if (mflag) free(wstr);
		return -1;
	}
	ret = sj3_rkconv_w16(wstr, outtmp);
	if (ret == 0 || ret == -1) {
		if (mflag) free(wstr);
		return ret;
	}
	if (wcstombs((char *)mtmp, outtmp, sizeof(outtmp)) == -1) {
		if (mflag) free(wstr);
		return -1;
	}
	if (wcstombs((char *)romaji, wstr, len) == -1) {
		if (mflag) free(wstr);
		return -1;
	}
	if (current_locale == LC_CTYPE_EUC) {
		(void) memcpy(kana, mtmp, strlen((char *)mtmp) + 1);
	} else {
		len = sjistoeuc(kana, MAXLLEN*2, mtmp, sizeof(mtmp));
	}

	if (mflag) free(wstr);
	if (len > 0) {
		return ret;
	} else {
		return len;
	}
}

sj3_rkconv_mb(romaji, kana)
u_char *romaji;
u_char *kana;
{
	if (current_locale == LC_CTYPE_EUC)
	  return sj3_rkconv_euc(romaji, kana);
	else
	  return sj3_rkconv(romaji, kana);
}

sj3_rkconv_w16(wstr, kstr)
wchar16_t *wstr;
wchar16_t *kstr;
{

	register int i;
	register wchar16_t *p, *q, *s;
	register u_short cc;
	int rlen, len;
	wchar16_t svstr[MAXLLEN];
	RkTablW16 *rktp;
	u_short *kp;

	len = wslen(wstr);
	rktp = sj3_rcode_w16[WTOUPPER(*wstr)];
	if (*wstr > (wchar16_t) 0xff) rktp = NULL;
	while(rktp != NULL) {
		rlen = rkmatch((wstr + 1), rktp->r_key, len - 1);
		if (rlen == CONTINUE) {
			break;
		} else if (rlen != NOMACH) {
			kp = rktp->k_yomi;
			while((cc = *kp++) != 0) {
				*kstr++ = cc;
			}
			wscpy(svstr, wstr);
			q = wstr;
			p = svstr;
			if ((s = rktp->r_str) != NULL) {
				p += rlen;
				i = wslen(s);
				p -= i;
				while (*s != NSTR) {
					if (*s == WTOUPPER(*p))
						*q = *p;
					else
						*q = *s;
					q++;
					p++;
					s++;
				}
			}
			if (rlen < len) {
				p = svstr;
				p += rlen;
				while (*p != NSTR)
					*q++ = *p++;
			}
			*q = NSTR;
			break;
		}
		rktp = rktp->next;
	}
	if (rktp == NULL && len <= 1) {
		*kstr++ = *wstr;
		*wstr = NSTR;
		rlen = 1;
	}
	*kstr = NSTR;
	return(rlen);
}



rkmatch(s1, s2, len)
wchar16_t *s1, *s2;
int len;
{
	register int i;

	i = 1;
	if (s2 == NULL)
		return(i);
	for (; i < len + 1; i++) {
		if (*s1 > (wchar16_t) 0xff)
		    return(NOMACH);
		if (WTOUPPER(*s1) != *s2) {
			if (*s2 != NSTR)
				return(NOMACH);
			break;
		}
		s1++;
		s2++;
	}
	if (*s2 != NSTR)
		i = CONTINUE;
	return(i);
}
