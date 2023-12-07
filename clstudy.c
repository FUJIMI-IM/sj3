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
 * $SonyRCSfile: clstudy.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:32 $
 *
 * $Id$
 */


#include <string.h>
#include <sys/types.h>
#include "sj_kcnv.h"
#include "kanakan.h"


int	sj2cd_str();
void	mvmemd(), mvmemi();

static u_char* delcldata(u_char* ycode);
static int delclold(void);
static u_char* srchclpos(u_char* ycode);


int
clstudy(u_char* yomi1, u_char* yomi2, STDYOUT *stdy)
{
	u_char	ytmp[CLSTDY_TMPLEN];
	u_char	*dst, *ptr;
	int	ret;
	int	len1, len2;
	int	alen, blen;
	int	i;

	if (!StudyExist()) return ClstudyNoDict;

	if (!yomi1 || !*yomi1) return ClstudyParaErr;

	if (!yomi2 || !*yomi2) {
		if (!sj2cd_str(yomi1, ytmp, CLSTDY_TMPLEN))
			return ClstudyYomiErr;

		(Void)delcldata(ytmp);

		ret = ClstudyNormEnd;
	}
	else if (!stdy) {
		return ClstudyParaErr;
	}
	else {
		if (!sj2cd_str(yomi1, ytmp, CLSTDY_TMPLEN))
			return ClstudyYomiErr;
		len1 = strlen(ytmp);

		if (!sj2cd_str(yomi2, ytmp + len1, CLSTDY_TMPLEN - len1))
			return ClstudyYomiErr;

		len2 = len1 + stdy -> len;
		ytmp[len2] = 0;
		alen = len2 + CL_HDLEN;

		if (alen + 1 >= ClStudyLen) return ClstudyNoArea;

		blen = ClStudyTail - delcldata(ytmp) - 1;

		while (blen < alen) {
			if (!(i = delclold())) break;
			blen += i;
		}

		if (blen >= alen) {
			dst = srchclpos(ytmp);

			ptr = ClStudyTail - alen;
			mvmemd(ptr, ClStudyTail, (ptr - dst));
			ClYomiLen(dst)  = (Uchar)len2;
			ClGramCode(dst) = stdy -> hinshi;
			ClYomi1Len(dst) = (Uchar)len1;
			ClSetNum(dst, 1);
			memcpy(ytmp, ClYomiPos(dst), len2);

			ret = ClstudyNormEnd;
		}
		else {
			ret = ClstudyNoArea;
		}
	}

	mkclidx();
	putcldic();

	return ret;
}


static u_char*
delcldata(u_char* ycode)
{
	u_char	*tagptr;
	u_char	*ptr1;
	u_char	*ptr2;
	int	len1;

	tagptr = ClStudyDict;
	while (!iseocl(tagptr)) {
		ptr1 = ycode;
		ptr2 = ClYomiPos(tagptr);
		len1 = ClYomiLen(tagptr);

		while (*ptr1 && len1) {
			if (*ptr1 != *ptr2) break;
			ptr1++;
			ptr2++;
			len1--;
		}

		if (!*ptr1 || !len1) {
			delclsub(tagptr);
		}
		else {
			tagptr = ClNextTag(tagptr);
		}
	}

	return tagptr;
}


void
delclsub(u_char* target)
{
	u_char	*src;
	int	num;
	int	tmp;
	int	len;

	num = ClGetNum(target);
	len = ClBlkLen(target);
	src = target + len;
	mvmemi(src, target, (ClStudyTail - src));
	memset(ClStudyTail - len, 0, len);

	src = ClStudyDict;
	while (!iseocl(src)) {
		tmp = ClGetNum(src);

		if (tmp > num) {
			tmp -= 1;
			ClSetNum(src, tmp);
		}

		src = ClNextTag(src);
	}
}


static int
delclold(void)
{
	u_char	*tagptr;
	u_char	*target;
	int	num;
	int	tmp;

	target = NULL;
	num = 0;

	tagptr = ClStudyDict;
	while (!iseocl(tagptr)) {

		tmp = ClGetNum(tagptr);

		if (tmp >= num) {
			num = tmp;
			target = tagptr;
		}

		tagptr = ClNextTag(tagptr);
	}

	if (target) {
		num = ClBlkLen(tagptr);

		delclsub(target);
	}

	return num;
}


static u_char*
srchclpos(u_char* ycode)
{
	u_char	*tagptr;
	u_char	*target;
	u_char	*ptr1;
	u_char	*ptr2;
	int	len;
	int	tmp;

	target = NULL;
	tagptr = ClStudyDict;
	while (!iseocl(tagptr)) {
		tmp = ClGetNum(tagptr);

		tmp++;
		ClSetNum(tagptr, tmp);

		ptr1 = ycode;
		ptr2 = ClYomiPos(tagptr);
		len  = ClYomiLen(tagptr);
		while (*ptr1 && len) {
			if (*ptr1 != *ptr2) break;
			ptr1++;
			ptr2++;
			len--;
		}

		if (!target) {
			if (!len)
				;
			else if (!*ptr1)
				target = tagptr;
			else if (*ptr1 < *ptr2)
				target = tagptr;
		}

		tagptr = ClNextTag(tagptr);
	}

	return target ? target : tagptr;
}


void
mkclidx(void)
{
	u_char	*ptr;
	int	num;
	int	st, ed;

	if (!StudyExist()) return;

	ptr = ClStudyDict;
	for (num = st = 0 ; st < 0x100 ; num++, st = ed) {
		ed = st + ClStudyStep;

		while (!iseocl(ptr)) {
			if ((unsigned int) *ClYomiPos(ptr) >= st) break;
			ptr = ClNextTag(ptr);
		}

		if (ClYomiLen(ptr) && ((Int)*ClYomiPos(ptr) < ed))
			ClStudyIdx[num] = ptr - ClStudyDict;
		else
			ClStudyIdx[num] = (Ushort)-1;
	}
}
