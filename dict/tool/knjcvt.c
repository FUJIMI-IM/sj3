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
 * $SonyRCSfile: knjcvt.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:00:37 $
 */




#include <stdio.h>
#include <sys/types.h>
#include "sj_const.h"
#include "sj_struct.h"

typedef struct div_list {
	int	len;		
	u_char	code[3];	
	struct div_list *child;	
} DivList;

extern	HindoRec *askknj[];
extern	int	askknj_num;



static	free_divlist(p)
register DivList *p;
{
	register DivList *q;

	while (p) {
		q = p -> child;
		Free(p);
		p = q;
	}
}



static	DivList	*make_divrec()
{
	register DivList *p;

	p = (DivList *)Malloc(sizeof(DivList));
	if (!p) {
		fprintf(stderr, "\245\341\245\342\245\352\244\254\302\255\244\352\244\336\244\273\244\363\n");
		exit(1);
	}
	p -> child = NULL;

	return p;
}



static	make_divlist(parent, knj, len)
DivList	*parent;	
u_char	*knj;		
int	len;		
{
	int	i, j;
	int	num;
	int	minlen;
	int	tmplen;
	DivList	tmprec;
	DivList	*minrec;

	
	minrec = make_divrec();

	
	if (len == 0) {
		free_divlist(minrec);

		parent -> child = NULL;
		return 0;
	}
	
	else if (len == 1) {
		
		minrec -> len     = len;
		minrec -> code[0] = *knj;

		parent -> child = minrec;
		return len;
	}

	
	if (codesize(*knj) == 1) {
		minrec -> len     = 1;
		minrec -> code[0] = *knj;
		minlen = make_divlist(minrec, knj + 1, len - 1) + 1;
	}
	
	else if (codesize(*knj) == 2) {
		minrec -> len     = 2;
		minrec -> code[0] = *knj;
		minrec -> code[1] = *(knj + 1);
		minlen = make_divlist(minrec, knj + 2, len - 2) + 2;
	}

	
	for (i = 0 ; i < len ; i = j) {
		
		j = i + codesize(*(knj + i));

		
		if (j <= 2 || !(num = isknjexist(knj, j))) continue;

		
		tmprec.len     = 2;
		tmprec.code[0] = (OffsetAssyuku | ((num >> 8) & 0xff));
		tmprec.code[1] = (num & 0xff);
		tmprec.child   = NULL;
		tmplen = make_divlist(&tmprec, knj + j, len - j) + 2;

		
		if (tmplen < minlen) {
			
			free_divlist(minrec -> child);

			
			*minrec = tmprec;
			minlen = tmplen;
		}
		else
			free_divlist(tmprec.child);
	}

	
	for (i = 0 ; i < askknj_num ; i++) {
		
		if ((j = askknj[i] -> klen) > len) continue;

		
		if (string_cmp(askknj[i]->kptr, j, knj, j)) continue;

		
		tmprec.len     = 1;
		tmprec.code[0] = (KanjiAssyuku | i);
		tmprec.child   = NULL;
		tmplen = make_divlist(&tmprec, knj + j, len - j) + 1;

		
		if (tmplen < minlen) {
			
			free_divlist(minrec -> child);

			
			*minrec = tmprec;
			minlen = tmplen;
		}
		else
			free_divlist(tmprec.child);
	}

	
	parent -> child = minrec;

	
	return minlen;
}



u_char	*knjofscvt(ptr, len, ret)
u_char	*ptr;
int	len;
int	*ret;
{
	int	i;
	u_char	*p, *q;
	DivList parent, *dl;

	
	askknj_num = 0;

	
	parent.child = NULL;
	i = make_divlist(&parent, ptr, len);
	*ret = i;

	
	p = (u_char *)Malloc(i);
	if (!p) {
		fprintf(stderr, "\245\341\245\342\245\352\244\254\311\324\302\255\244\267\244\336\244\267\244\277");
		exit(1);
	}
	for (q = p, dl = parent.child ; dl ; dl = dl -> child) {
		*q++ = dl -> code[0];
		if (dl -> len == 2) {
			*q++ = dl -> code[1];
		} else if (dl -> len == 3) {
			*q++ = dl -> code[1];
			*q++ = dl -> code[2];
		}
	}
	
	free_divlist(parent.child);

	
	return p;
}



u_char	*knjcvt(ptr, len, ret)
u_char	*ptr;
int	len;
int	*ret;
{
	int	i;
	u_char	*p, *q;
	DivList parent, *dl;

	
	parent.child = NULL;
	i = make_divlist(&parent, ptr, len);
	*ret = i;

	
	p = (u_char *)Malloc(i);
	if (!p) {
		fprintf(stderr, "\245\341\245\342\245\352\244\254\311\324\302\255\244\267\244\336\244\267\244\277");
		exit(1);
	}
	for (q = p, dl = parent.child ; dl ; dl = dl -> child) {
		*q++ = dl -> code[0];
		if (dl -> len == 2) {
			*q++ = dl -> code[1];
		} else if (dl -> len == 3) {
			*q++ = dl -> code[1];
			*q++ = dl -> code[2];
		}
	}
	
	free_divlist(parent.child);

if (i != q - p) {
	printf("\244\263\244\263\n");
	exit(1);
}
	
	return p;
}


