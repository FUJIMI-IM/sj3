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
 * $SonyRCSfile: dictdisp.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:34 $
 */




#include <stdio.h>
#include <ctype.h>

dictdisp(output)
char	*output;
{
	unsigned char	buf[BUFSIZ];
	unsigned char	*p;
	FILE	*fp;
	int	flg;
	int	i;

	if (output && *output) {
		fp = fopen(output, "w");
		if (!fp) {
			error_out("%s \244\254\245\252\241\274\245\327\245\363\244\307\244\255\244\336\244\273\244\363", output);
			exit(1);
		}
	}
	else
		fp = stdout;
		
	flg = sj3_getdict_mb(buf);
	while (flg == 0) {
		p = buf;
		printout_mb(fp, p);
		i = strlen(p);
		if (i < 24)
			while (i < 24) {
				printout_mb(fp, "\t");
				i += 8;
			}
		else
			printout_mb(fp, " ");
		while (*p++) ;
		printout_mb(fp, p);
		i = strlen(p);
		if (i < 24)
			while (i < 24) {
				printout_mb(fp, "\t");
				i += 8;
			}
		else
			printout_mb(fp, " ");
		while (*p++) ;
		printout(fp, hns2str(*p));
		printout_mb(fp, ":\n");

		flg = sj3_nextdict_mb(buf);
	}

	if (fp != stdout) fclose(fp);
}
