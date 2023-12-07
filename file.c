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
 * $SonyRCSfile: file.c,v $  
 * $SonyRevision: 1.2 $ 
 * $SonyDate: 1995/02/03 07:38:32 $
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "dicttool.h"

typedef	struct filelist {
	FILE	*fp;
	char	*fname;
	struct filelist *next;
} FileList;

static	FileList *flist = NULL;

static char*
get_fname(FILE* fp)
{
	FileList *p;

	for (p = flist ; p ; p = p -> next)
		if (p -> fp == fp) return p -> fname;
	return "some file";
}

FILE*
Fopen(char* filename, char* type)
{
	FILE	*fp;
	FileList *fl;

	if (!(fp = fopen(filename, type))) {
		fprintf(stderr, "Can't open %s mode \"%s\"\n", filename, type);
		exit(1);
	}

	fl = (FileList *)Malloc(sizeof(*fl));
	fl -> fname = Malloc(strlen(filename) + 1);
	strcpy(fl -> fname, filename);
	fl -> fp = fp;
	fl -> next = flist;

	return fp;
}

void
Fclose(FILE* fp)
{
	FileList *p, *q;

	if (fclose(fp) == EOF) {
		fprintf(stderr, "Close error in %s\n", get_fname(fp));
		exit(1);
	}

	for (p = flist, q = NULL ; p ; q = p, p = p -> next) {
		if (p -> fp == fp) {
			if (q)
				q -> next = p -> next;
			else
				flist = p -> next;
			free(p -> fname);
			free(p);
			break;
		}
	}
}

int
Fseek(FILE* fp, long ofs, int ptr)
{
	if (fseek(fp, ofs, ptr) == 0) return 0;

	fprintf(stderr, "Seek error in %s\n", get_fname(fp));
	exit(1);
}

long
Ftell(FILE* fp)
{
	return ftell(fp);
}

off_t /* XXX: just looked in FreeBSD's sys/stat.h, need to check SUS spec. */
Fsize(char* filename)
{
	struct	stat	buf;

#if defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)
	if (stat(filename, &buf) == 0) return (long)buf.st_size;
#else
	if (stat(filename, &buf) == 0) return buf.st_size;
#endif

	fprintf(stderr, "Stat error in %s\n", filename);
	exit(1);
}

int
Fread(char* p, int s, int n, FILE* fp)
{
	if (fread(p, s, n, fp) == n) return n;

	fprintf(stderr, "Read error in %s\n", get_fname(fp));
	exit(1);
}

int
Fwrite(char* p, int s, int n, FILE* fp)
{
	if (fwrite(p, s, n, fp) == n) return n;

	fprintf(stderr, "Write error in %s\n", get_fname(fp));
	exit(1);
}

int
Fgetc(FILE* fp)
{
	return fgetc(fp);
}

void
Fflush(FILE* fp)
{
        fflush(fp);
}    
