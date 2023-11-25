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
 * $SonyRCSfile: alloc.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:28 $
 */



#include <sys/types.h>
#include "sj_rename.h"
#include "sj_typedef.h"
#include "sj_const.h"
#include "sj_var.h"
#include "sj_struct.h"

#define	JREC_NUM	128
#define	CLREC_NUM	128

static	JREC	*jrec = NULL;
static	CLREC	*clrec = NULL;

JREC	*alloc_jrec()
{
	register JREC	*p;
	register int	i;

	if (jrec == NULL) {
		jrec = (JREC *)malloc(sizeof(JREC) * JREC_NUM);
		if (jrec == NULL) return (JREC *)malloc(sizeof(JREC));
		for (i = 0, p = jrec ; i < JREC_NUM - 1 ; i++, p++)
			p -> jsort = p + 1;
		p -> jsort = NULL;
	}
	p = jrec;
	jrec = jrec -> jsort;

	return p;
}

Void	free_jrec(p)
JREC	*p;
{
	if (p != NULL) {
		p -> jsort = jrec;
		jrec = p;
	}
}

CLREC	*alloc_clrec()
{
	register CLREC	*p;
	register int	i;

	if (clrec == NULL) {
		clrec = (CLREC *)malloc(sizeof(CLREC) * CLREC_NUM);
		if (clrec == NULL) return (CLREC *)malloc(sizeof(CLREC));
		for (i = 0, p = clrec ; i < CLREC_NUM - 1 ; i++, p++)
			p -> clsort = p + 1;
		p -> clsort = NULL;
	}
	p = clrec;
	clrec = clrec -> clsort;

	return p;
}

Void	free_clrec(p)
CLREC	*p;
{
	if (p != NULL) {
		p -> clsort = clrec;
		clrec = p;
	}
}
