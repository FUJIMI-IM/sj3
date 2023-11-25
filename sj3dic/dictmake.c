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
 * $SonyRCSfile: dictmake.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:35 $
 */




#include "sj_sysvdef.h"
#include <stdio.h>
#ifdef SVR4
#include <string.h>
#else
#include <strings.h>
#endif
#include <ctype.h>
#include "sjctype.h"
#include "sj3lib.h"
#include "sjtool.h"

#define	IsEOL(c)	((c) == '\0')
#define	IsBlank(c)	((c) == ' ' || (c) == '\t')

extern	int	force_flag;
extern	int	verbose_flag;
extern  int     _sys_code;

static void touroku(y, k, h)
char	*y, *k, *h;
{
	int	err;
	int	grm;
	unsigned char buf[BUFSIZ], *hp;
	extern int sjistoeuc();

	if (_sys_code == SYS_SJIS) {
		(void) sjistoeuc(buf, BUFSIZ, h, strlen(h)+1);
		hp = buf;
	} else {
		hp = (unsigned char *) h;
	}
	if (!(grm = str2hns(hp))) {
		error_out("\311\312\273\354\312\270\273\372\316\363\244\362\262\341\244\303\244\306\244\244\244\336\244\271 %s:%s:%s", y, k, h);
		if (force_flag) return;
		exit(1);
	}

	err = sj3_touroku_mb(y, k, grm);

	switch (err) {
	case -1:
		error_out("\245\265\241\274\245\320\244\254\315\356\244\301\244\336\244\267\244\277");
		exit(1);

	case 0:
		if (verbose_flag)
			normal_out("%s:%s:%s \244\362\305\320\317\277\244\267\244\336\244\267\244\277\n", y, k, h);
		break;

	case SJ3_DICT_ERROR:
		error_out("\274\255\275\361\244\254\244\312\244\244\241\242\244\242\244\353\244\244\244\317\274\255\275\361\244\313\275\361\244\255\271\376\244\341\244\336\244\273\244\363");
		exit(1);

	case SJ3_DICT_LOCKED:
		error_out("\274\255\275\361\244\254\245\355\245\303\245\257\244\265\244\354\244\306\244\244\244\306\275\361\244\255\271\376\244\341\244\336\244\273\244\363");
		exit(1);

	case SJ3_WORD_EXIST:
		if (verbose_flag)
			normal_out("%s:%s:%s \244\317\305\320\317\277\244\265\244\354\244\306\244\244\244\336\244\271\n", y, k, h);
		break;

	case SJ3_DOUON_FULL:
		error_out("\244\263\244\354\260\312\276\345\306\261\262\273\270\354\244\362\305\320\317\277\244\307\244\255\244\336\244\273\244\363 %s:%s:%s", y, k, h);
		break;

	case SJ3_DICT_FULL:
		error_out("\244\263\244\354\260\312\276\345\274\255\275\361\244\362\302\347\244\255\244\257\244\307\244\255\244\336\244\273\244\363 %s:%s:%s", y, k, h);
		if (force_flag) break;
		exit(1);

	case SJ3_BAD_YOMI_STR:
		error_out("\305\320\317\277\244\307\244\255\244\312\244\244\306\311\244\337\244\307\244\271 %s", y);
		if (force_flag) break;
		exit(1);

	case SJ3_BAD_KANJI_STR:
		error_out("\305\320\317\277\244\307\244\255\244\312\244\244\264\301\273\372\244\307\244\271 %s:%s", y, k);
		if (force_flag) break;
		exit(1);

	case SJ3_BAD_HINSI_CODE:
		error_out("\305\320\317\277\244\307\244\255\244\312\244\244\311\312\273\354\244\307\244\271 %s:%s:%s", y, k, h);
		if (force_flag) break;
		exit(1);

	case SJ3_INDEX_FULL:
		error_out("\245\244\245\363\245\307\245\303\245\257\245\271\244\254\244\244\244\303\244\321\244\244\244\307\244\271");

	case SJ3_TOUROKU_FAILED:
	default:
		error_out("\305\320\317\277\244\307\244\255\244\336\244\273\244\363\244\307\244\267\244\277 %s:%s:%s(%d)",
				y, k, hns2str(h), h);
		break;
	}
}

dictmake(input)
char	*input;
{
	unsigned char	buf[BUFSIZ];
	FILE	*fp;
	unsigned char	*p;
	unsigned char	*yom, *knj, *grm;
	int	c;
	int	flg;
	static	char	*FormatErrorString = "\245\325\245\251\241\274\245\336\245\303\245\310\245\250\245\351\241\274:%s";

	if (input && *input) {
		fp = fopen(input, "r");
		if (!fp) {
			error_out("%s \244\254\245\252\241\274\245\327\245\363\244\307\244\255\244\336\244\273\244\363", input);
			exit(1);
		}
	}
	else
		fp = stdin;

	while (fgets((char *)buf, sizeof(buf), fp)) {
		if (p = (unsigned char *)strchr((char *)buf, '\n'))
			*p = '\0';
		else {
			do {
				c = fgetc(fp);
			} while (c != '\n' && c != EOF);
		}

		yom = buf;
		while (IsBlank(*yom)) yom++;
		if (IsEOL(*yom)) {
			error_out(FormatErrorString, buf);
			if (force_flag) continue;
			break;
		}

		knj = yom;
		while (!IsEOL(*knj) && !IsBlank(*knj)) knj++;
		if (IsEOL(*knj)) {
			error_out(FormatErrorString, buf);
			if (force_flag) continue;
			break;
		}
		*knj++ = '\0';
		while (IsBlank(*knj)) knj++;
		if (IsEOL(*knj)) {
			error_out(FormatErrorString, buf);
			if (force_flag) continue;
			break;
		}

		grm = knj;
		while (!IsEOL(*grm) && !IsBlank(*grm)) grm++;
		if (IsEOL(*grm)) {
			error_out(FormatErrorString, buf);
			if (force_flag) continue;
			break;
		}
		*grm++ = '\0';
		while (IsBlank(*grm)) grm++;
		if (IsEOL(*grm)) {
			error_out(FormatErrorString, buf);
			if (force_flag) continue;
			break;
		}

		flg = -1;
		while (*grm && flg) {
			p = grm;
			while (!IsEOL(*p) && !IsBlank(*p) && (*p != ':')) p++;
			if (IsEOL(*p))
				flg = 0;
			else
				*p++ = '\0';

			touroku(yom, knj, grm);

			if (flg) {
				grm = p;
				while (IsBlank(*grm)) grm++;
			}
		}
	}
}
