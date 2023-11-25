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
 * $SonyRCSfile: codecnv.c,v $  
 * $SonyRevision: 1.3 $ 
 * $SonyDate: 1997/01/23 11:26:31 $
 */




#include <stdio.h>
#include <ctype.h>
#include <locale.h>
#include "sjctype.h"
#include "sjtool.h"

static	int	euc_mode;

init_code()
{
	char *loc;

#ifdef LACKOF_SETLOCALE 
        loc = getenv("LANG");
#else
	loc = setlocale(LC_CTYPE, "");
#endif
	if (!loc) {
		fprintf(stderr, "Error: can't get current locale\n");
		exit(1);
	}
	if (strncmp(loc, "ja_JP.SJIS", 10) == 0) {
		euc_mode = 0;
	} else if (strncmp(loc, "ja", 2) == 0 ||
		   strncmp(loc, "Ja", 2) == 0) {
		euc_mode = -1;
	} else 
#ifdef __sony_news
	        return 0;
#else
	  {
		  fprintf(stderr, "Warrning: In this locale japanese will not be used.\n");
		  euc_mode = -1;
	  }
#endif
	return 1;
}

cnvcode(s)
unsigned char	*s;
{
	if (euc_mode) {
		unsigned short	i;
		unsigned char	*d = s;

		while (i = *s++) {
			if (iseuckana(i)) {
				if (*s) {
					*d++ = *s++;
				}
				else {
					*d++ = i;
					break;
				}
			}
			else if (iseuc(i)) {
				if (*s) {
					i = euc2sjis((i << 8) + *s++);
					*d++ = (i >> 8);
					*d++ = i;
				}
				else {
					*d++ = i;
					break;
				}
			}
			else {
				*d++ = i;
			}
		}
		*d = 0;
	}
}

printout_mb(fp, s)
FILE    *fp;
unsigned char   *s;
{
	fputs((char *)s, fp);
	fflush(fp);
}

printout(fp, s) 
FILE	*fp;
unsigned char	*s;
{
	unsigned char	buf[BUFSIZ];

	if (euc_mode)
		strcpy((char *)buf, s);
	else
		euctosjis(buf, BUFSIZ, s, strlen(s)+1);
	fputs((char *)buf, fp);
	fflush(fp);
}

normal_out(fmt, p1, p2, p3, p4, p5)
char	*fmt;
int	p1, p2, p3, p4, p5;
{
	char	buf[BUFSIZ];

	sprintf(buf, fmt, p1, p2, p3, p4, p5);
	printout(stdout, buf);
}

error_out(fmt, p1, p2, p3, p4, p5)
char	*fmt;
int	p1, p2, p3, p4, p5;
{
	char	buf[BUFSIZ];

	sprintf(buf, fmt, p1, p2, p3, p4, p5);
	strcat(buf, "\n");
	printout(stderr, buf);
}
