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
 * $SonyRCSfile: error.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:51 $
 */




#include <stdio.h>
#include "sj_typedef.h"

extern	char	program_name[];
extern	int	debug_level;
extern	int	client_fd;

static	char	*tty_name = "/dev/tty";
static	FILE	*errfp = NULL;
static	FILE	*logfp = NULL;
static	FILE	*dbgfp = NULL;



open_error()
{
	extern	char	*error_file;
	int	flg = 0;

	if (error_file && *error_file) {
		if (!strcmp(error_file, tty_name)) {
			errfp = stderr;
			flg = -1;
		}
		else if (!(errfp = fopen(error_file, "a")))
			fprintf(stderr, "Can't open \"%s\"\r\n", error_file);
	}
	else
		errfp = NULL;

	return flg;
}

error_out(s, p1, p2, p3, p4, p5)
char	*s;
int	p1, p2, p3, p4, p5;
{
	char	tmp[BUFSIZ];

	if (errfp) {
		sprintf(tmp, s, p1, p2, p3, p4, p5);
		fprintf(errfp, "%s: %s\r\n", program_name, tmp);
		fflush(errfp);
	}
	exit(1);
}

warning_out(s, p1, p2, p3, p4, p5)
char	*s;
int	p1, p2, p3, p4, p5;
{
	char	tmp[BUFSIZ];

	if (errfp) {
		sprintf(tmp, s, p1, p2, p3, p4);
		fprintf(errfp, "%s: warning: %s\r\n", program_name, tmp);
		fflush(errfp);
	}
}



open_log()
{
	extern	char	*log_file;
	int	flg = 0;

	if (log_file && *log_file) {
		if (!strcmp(log_file, tty_name)) {
			logfp = stderr;
			flg = -1;
		}
		else
			logfp = fopen(log_file, "a");
		if (logfp) {
			long	t;

			time(&t);
			fprintf(logfp, "%s: log started at %s\r",
				program_name, ctime(&t));
			fflush(logfp);
		}
		else
			fprintf(stderr, "Can't open \"%s\"\r\n", log_file);
	}
	else
		logfp = NULL;

	return flg;
}

logging_out(s, p1, p2, p3, p4, p5)
char	*s;
int	p1, p2, p3, p4, p5;
{
	char	tmp[BUFSIZ];

	if (logfp) {
		sprintf(tmp, s, p1, p2, p3, p4, p5);
		fprintf(logfp, "%s\r\n", tmp);
		fflush(logfp);
	}
}



open_debug()
{
	extern	char	*debug_file;
	int	flg = 0;

	if (debug_file && *debug_file) {
		if (!strcmp(debug_file, tty_name)) {
			dbgfp = stderr;
			flg = -1;
		}
		else  if (!(dbgfp = fopen(debug_file, "a")))
			fprintf(stderr, "Can't open \"%s\"\r\n", debug_file);
	}
	else
		dbgfp = NULL;

	return flg;
}

debug_out(lvl, s, p1, p2, p3, p4, p5)
int	lvl;
char	*s;
int	p1, p2, p3, p4, p5;
{
	if (lvl <= debug_level && dbgfp) {
		fprintf(dbgfp, s, p1, p2, p3, p4, p5);
		fflush(dbgfp);
	}
}
