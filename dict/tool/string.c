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
 * $SonyRCSfile: string.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:00:45 $
 */


#include <stdio.h>
#include <sys/types.h>
#include "sj_const.h"



bubun_str(p1, l1, p2, l2)
u_char	*p1;
int	l1;
u_char	*p2;
int	l2;
{
	register u_char	*p;
	register int	l;
	register int	i;

	for (p = p1, l = l1 ; l > 0 ; ) {
		
		if (l < l2) break;
		for (i = 0 ; i < l2 ; i++)
			if (*(p + i) != *(p2 + i)) break;

		
		if (i >= l2) return 1;

		i = codesize(*p);
		p += i;
		l -= i;
	}

	for (p = p2, l = l2 ; l > 0 ; ) {
		
		if (l < l1) break;
		for (i = 0 ; i < l1 ; i++) if (*(p + i) != *(p1 + i)) break;

		
		if (i >= l1) return -1;

		i = codesize(*p);
		p += i;
		l -= i;
	}

	
	return 0;
}



overlap_str(p1, l1, p2, l2)
u_char	*p1;
u_char	*p2;
int	l1;
int	l2;
{
	register u_char	*p;
	register int	l;
	register int	i;
	register int	j;
	u_char	tmp[MaxKanjiLength * 2 + MaxAtrNumber * 2];

	for (p = p1, l = l1 ; l > 0 ; ) {
		
		if (l < l2) {
			for (i = 0 ; (i < l) && (*(p + i) == *(p2 + i)) ; i++) ;

			
			if ((i >= l) && (l2 + l1 - l < sizeof(tmp))) {
				
				for (i = 0 ; i < l1 ; i++)
					tmp[i] = *(p1 + i);
				for (j = l ; j < l2 ; i++, j++)
					tmp[i] = *(p2 + j);

				
				if (check_hindo(tmp, i)) {
					return -1;
				}
			}
		}

		i = codesize(*p);
		p += i;
		l -= i;
	}

	for (p = p2, l = l2 ; l > 0 ; ) {
		
		if (l < l1) {
			for (i = 0 ; (i < l) && (*(p + i) == *(p1 + i)) ; i++) ;

			
			if ((i >= l) && (l1 + l2 - l < sizeof(tmp))) {
				
				for (i = 0 ; i < l2 ; i++)
					tmp[i] = *(p2 + i);
				for (j = l ; j < l1 ; i++, j++)
					tmp[i] = *(p1 + j);

				
				if (check_hindo(tmp, i)) {
					return -1;
				}
			}
		}

		i = codesize(*p);
		p += i;
		l -= i;
	}

	
	return 0;
}



istrlen(s)
int	*s;
{
	register int *p;

	for (p = s ; *p ; p++) ;

	return(p - s);
}



istrcmp(s, d)
register int	*s;
register int	*d;
{
	for ( ; *s && (*s == *d) ; s++, d++) ;

	return (*s - *d);
}



top_strcmp(src, dst)
register int	*src;
register int	*dst;
{
	register int	i = 0;

	
	for (i = 0 ; *src && (*src == *dst) ; src++, dst++)
		i++;

	
	return (i > MaxYomiAskNumber) ? MaxYomiAskNumber : i;
}



last_strcmp(src, dst)
int	*src;		
int	*dst;		
{
	int	slen, dlen;

	
	slen = istrlen(src);
	dlen = istrlen(dst);

	
	if (dlen > MaxYomiAskNumber)
		return 0;

	
	if (istrcmp(src + (slen - dlen), dst))
		return 0;

	
	return dlen;
}



string_cmp(p1, l1, p2, l2)
register u_char	*p1;
register u_char	*p2;
register int	l1;
register int	l2;
{
	while (l1 > 0 && l2 > 0) {
		if (*p1 != *p2) return (*p1 - *p2);
		p1++; l1--;
		p2++; l2--;
	}

	if (l1 == l2) return 0;
	if (l1 > l2) return 1;
	return -1;
}
