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
 * $SonyRCSfile: makedict.c,v $  
 * $SonyRevision: 1.2 $ 
 * $SonyDate: 1994/12/09 11:27:05 $
 *
 * $Id$
 */


/* #include "sj_sysvdef.h" */ /* XXX: no more need it? */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include "const.h"
#include "dicttool.h"

FILE	*infp = NULL;
FILE	*outfp = NULL;

static	char	*idxtop = NULL;


int
main(int argc, char** argv)
{
	parse(argc, argv);

	while (readline()) setline(makelist);
	flush_douon();
        makehead((unsigned char *)argv[2]);

	return 0;
}


char*
make_idxlist(char* name)
{
	int	i;
	char	*p;

	i = strlen(name);
	p = Zalloc(i + 2);
	strcpy(p, name);
	return p;
}

char*
get_idxlist(char* name)
{
	FILE	*fp;
	int	i;
	char	*p, *q, *r;

	
	fp = Fopen(name, "r");

	
	i = Fsize(name);

	
	p = Zalloc(i + 2);
	Fseek(fp, 0L, 0);
	Fread(p, 1, i, fp);
	Fclose(fp);

	
	for (q = r = p ; *q ; ) {
		while (*q <= ' ') q++;
		while (*q > ' ') *r++ = *q++;
		*r++ = 0;
		q++;
	}
	*r = 0;

	
	i = 0;
	for (q = p ; *q ; ) {
		if ((fp = fopen(q, "r"))!=NULL)
			fclose(fp);
		else {
			fprintf(stderr, "Can't open %s mode \"r\"\n", q);
			i++;
		}
		while (*q++) ;
	}
	if (i) exit(1);

	return p;
}


void
parse(int argc, char** argv)
{
	char	*progname = NULL;
        char    *p;

	
	if ((progname = strrchr(argv[0], '/'))!=NULL)
		progname++;
	else
		progname = argv[0];

	
	if (argc != 3) usage(progname);

	
	p = argv[1];
	idxtop = (*p == '-') ? get_idxlist(p + 1) : make_idxlist(p);

	outfp = Fopen(argv[2], "w");
}


int
get_number(char* ptr)
{
	int	i = 0;

	while (*ptr >= '0' && *ptr <= '9') {
		i = i * 10 + (*ptr++ - '0');
	}

	return(i);
}


void
usage(char* name)
{
	fprintf(stderr, "Usage: %s text_file binary_file\n",
		name ? name : "encode");
	exit(0);
}


int
getch(void)
{
	if (infp) {
		int	c;

		if ((c = Fgetc(infp)) != EOF) return c;

		Fclose(infp);
		infp = NULL;

		while (*idxtop++) ;
	}

	if (*idxtop) {
		infp = Fopen(idxtop, "r");

		fprintf(stderr, "Reading %s\n", idxtop);
		fflush(stderr);

		return getch();
	}

	return EOF;
}


void
mark_file(FILE* out)
{
	static	char	pathname[MAXPATHLEN];
	static	long	pos;

	if (out) {
		FILE	*fp;
		int	c;

		fprintf(out, "%s:\t", pathname);
		fp = Fopen(pathname, "r");
		Fseek(fp, pos, 0);
		c = Fgetc(fp);
		while (c != EOF && c != '\n') {
			fputc(c, out);
			c = Fgetc(fp);
		}
		fputc('\n', out);
		Fclose(fp);
	}
	else {
		strcpy(pathname, idxtop);
		pos = infp ? Ftell(infp) : 0;
	}
}
