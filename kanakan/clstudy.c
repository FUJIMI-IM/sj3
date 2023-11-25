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
 */





#include "sj_kcnv.h"



Int	sj2cd_str(), sstrlen();
Void	memcpy(), memset(), mvmemd(), mvmemi();



Static  Uchar	*delcldata(), *srchclpos();
Void	delclsub(), mkclidx();
Static  Int	delclold();



Int	clstudy(yomi1, yomi2, stdy)
Uchar	*yomi1;
Uchar	*yomi2;
STDYOUT	*stdy;
{
	Uchar	ytmp[CLSTDY_TMPLEN];
	Uchar	*dst, *ptr;
	Int	ret;
	Int	len1, len2;
	Int	alen, blen;
	Int	i;

	if (!StudyExist()) return ClstudyNoDict;

	
	if (!yomi1 || !*yomi1) return ClstudyParaErr;

	
	if (!yomi2 || !*yomi2) {

		
		if (!sj2cd_str(yomi1, ytmp, CLSTDY_TMPLEN))
			return ClstudyYomiErr;

		
		(Void)delcldata(ytmp);

		ret = ClstudyNormEnd;
	}

	
	else if (!stdy)
		return ClstudyParaErr;

	else {
		
		if (!sj2cd_str(yomi1, ytmp, CLSTDY_TMPLEN))
			return ClstudyYomiErr;
		len1 = sstrlen(ytmp);

		
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
		
		else
			ret = ClstudyNoArea;
	}

	
	mkclidx();

	
	putcldic();

	
	return ret;
}



Static	Uchar	*delcldata(ycode)
Uchar	*ycode;
{
	Reg1	Uchar	*tagptr;
	Reg2	Uchar	*ptr1;
	Reg3	Uchar	*ptr2;
	Int	len1;

	
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



Void	delclsub(target)
Uchar	*target;
{
	Reg1	Uchar	*src;
	Int	num;
	Int	tmp;
	Int	len;

	
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



Static	Int	delclold()
{
	Reg1	Uchar	*tagptr;
	Reg2	Uchar	*target;
	Int	num;
	Int	tmp;

	
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



Static	Uchar	*srchclpos(ycode)
Uchar	*ycode;
{
	Reg1	Uchar	*tagptr;
	Reg3	Uchar	*target;
	Reg2	Uchar	*ptr1;
	Reg4	Uchar	*ptr2;
	Reg5	Int	len;
	Int	tmp;

	
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



Void	mkclidx()
{
	Reg1	Uchar	*ptr;
	Reg2	Int	num;
	Int	st, ed;

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
