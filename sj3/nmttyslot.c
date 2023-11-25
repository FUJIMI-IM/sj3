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
 * $SonyRCSfile: nmttyslot.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:20 $
 */



#include "sj2.h"
#define LINESIZE	128








char	*ttyname();
char	*getttys();
char	*strrchr();
static	char	*ttys	= "/etc/ttys";

#define	NULL	0

nmttyslot (name)
char	*name;
{
	register char	*tp, *p;
	register int	s, tf;

	tp = name;

	if ((p = strrchr (tp, '/')) == NULL)
		p = tp;
	else
		p++;
	if ((tf = open (ttys, 0)) < 0)
		return(0);
	s = 0;
	while (tp = getttys (tf)) {
		s++;
		if (strcmp (p, tp) == 0) {
			close (tf);
			return (s);
		}
	}
	close (tf);
	return (0);
}

static char *
getttys (f)
int	f;
{
	static char line[LINESIZE];
	register char *lp;
top:
	for (lp = line ; lp < &line[LINESIZE] ; lp ++) {
		if (read (f, lp, 1) != 1)
			return (NULL);
		if (*lp == '\n') {
			*lp = '\0';
			break;
		}
		else if (*lp == ' ' || *lp == '\t') {
					
			while (1) {
				if (read (f, lp, 1) != 1)
					break;
				if (*lp == '\n')
					break;
			}
			*lp = '\0';
			break;
		}
	}
	lp = line;
	if (*lp == '0' || *lp == '1')	
		lp += 2;
	else if (*lp == '#')		
		goto top;
	return (lp);
}

