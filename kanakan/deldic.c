/*-
 * SPDX-License-Identifier: MIT-open-group
 *
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
 */



#include <sys/types.h>
#include <string.h>
#include "sj_kcnv.h"
#include "kanakan.h"


void	mvmemi();
void	mkidxtbl();


static int del_douon(TypeDicSeg seg, u_char* ptr, TypeDicOfs ofs);
static int del_segment(TypeDicSeg seg);
static void del_uidx(TypeDicSeg seg);
static void del_stdy(TypeDicSeg seg, TypeDicOfs ofs, int size);


u_int
deldic(u_char* yomi, u_char* kanji, TypeGram hinsi)
{
	u_char		yptr[MaxWdYomiLen + 1];
	u_char		kptr[MaxWdKanjiLen + 1];
	u_short		err;
	int		knjlen;
	TypeDicSeg	useg;
	TypeDicOfs	ofs;
	u_char		*p1, *p2, *p3;
	int		dblknum;
	int		hblknum;
	int		kblknum;
	int		samlen;
	u_char		*stp, *edp;
	int		size, len;

	if ((err = addel_arg(yomi, kanji, hinsi, yptr, sizeof(yptr))) != 0)
		return err;

	if ((curdict -> maxunit) == 0) return AD_NotUserDict;

	knjlen = cvtknj(yomi, kanji, kptr);

	cnvstart = yptr;
	cnvlen = strlen(yptr);

	useg = srchidx((TypeDicSeg)DicSegBase, cnvlen);
	(*curdict->getdic)(curdict, useg);

	if ((dblknum = srchkana(&p1, &samlen)) == 0) return AD_NoMidasi;

	if ((hblknum = srchgram(p1, &p2, hinsi)) == 0) return AD_NoHinsi;

	p3 = p2;
	kblknum = srchkanji(&p3, kptr, knjlen);
	if (*p3 == HinsiBlkTerm) return AD_NoKanji;
	ofs = p3 - dicbuf;

	edp = skipkstr(p3);

	if (kblknum == 1) {
		if (hblknum == 1) {
			if (dblknum == 1) {
				return del_segment(useg);
			}
			else {
				return del_douon(useg, p1, ofs);
			}
		}
		else {
			stp = p2;
			edp++;
		}
	}
	else {
		stp = p3;
	}

	size = edp - stp;

	len = dicbuf + (curdict->seglen) - edp;
	mvmemi(edp, stp, len);

	edp += len;
	memset(edp, DicSegTerm, dicbuf + curdict->seglen - edp);

	set_size(p1, (int)(getsize(p1) - size),
			(int)getplen(p1), (int)getnlen(p1));

	(*curdict->putdic)(curdict, useg);

	del_stdy(useg, ofs, size);

	return AD_Done;
}


static int
del_douon(TypeDicSeg seg, u_char* ptr, TypeDicOfs ofs)
{
	u_char	*nxt, *p1, *p2;
	int	size;
	int	nlen, plen, len;

	nxt = getntag(ptr);

	if (segend(nxt)) {
		size = nxt - ptr;

		memset(ptr, DicSegTerm, dicbuf + curdict->seglen - nxt);
	}
	else {
		size = getsize(nxt);
		nlen = getnlen(nxt);
		plen = getplen(nxt);

		if (ptr == segtop()) {
		        if (nlen + plen > (unsigned int) count_uidx()
				+ strlen(get_idxptr(seg)))
				return AD_OvflwIndex;
		}
		
		len = ((len = getplen(ptr)) < plen) ? plen - len : 0;

		set_size(ptr, size + len, plen - len, nlen + len);

		p1 = nxt + DouonBlkSizeNumber;
		p2 = ptr + DouonBlkSizeNumber + len;

		mvmemi(p1, p2, dicbuf + curdict->seglen - p1);

		size = p1 - p2;
		memset(dicbuf + curdict->seglen - size, DicSegTerm, size);

		if (ptr == segtop()) {
			chg_uidx(seg, ptr + DouonBlkSizeNumber, nlen + len);
		}
	}

	(*curdict->putdic)(curdict, seg);

	del_stdy(seg, ofs, size);

	return AD_Done;
}


static int
del_segment(TypeDicSeg seg)
{
	int		i;
	STDYIN		*styp;
	TypeDicID	dicid;
	TypeStyNum	stdynum;
	TypeDicSeg	sg;

	if (curdict->segunit > 1) {
		for (sg = seg + 1 ; sg < curdict->segunit ; sg++) {
			(*curdict->getdic)(curdict, sg);
			(*curdict->putdic)(curdict, sg - 1);
		}
		curdict->segunit--;
	}
	else {
		memset(dicbuf, DicSegTerm, (int)curdict->seglen);
		dicbuf[0] = 0;
		(*curdict->putdic)(curdict, DicSegBase);
	}

	(*curdict->rszdic)(curdict, curdict->segunit);

	del_uidx(seg);

	if (StudyExist()) {
		dicid = curdict->dicid;
		for (i = 0, styp = StudyDict ; i < StudyCount ; ) {
			if (styp -> dicid != dicid)
				;
			else if (styp -> seg > seg)
				styp -> seg -= 1;
			else if (styp -> seg < seg)
				;
			else {
				stdynum = styp -> styno;
				StudyCount--;
				mvmemi((u_char*)(styp + 1), (u_char*)styp,
					sizeof(*styp) * (StudyCount - i));
				continue;
			}

			i++;
			styp++;
		}
		for (i = 0, styp = StudyDict ; i < StudyCount ; i++, styp++)
			if (styp -> styno > stdynum) styp -> styno -= 1;

		putstydic();
	}

	return AD_Done;
}


static void
del_uidx(TypeDicSeg seg)
{
	u_char	*p, *q;
	int	len;

	p = get_idxptr(seg);
	for (q = p ; *q++; )
		;

	mvmemi(q, p, idxbuf + curdict->idxlen - q);

	len = q - p;
	memset(idxbuf + curdict->idxlen - len, 0, len);

	(*curdict->putidx)(curdict, 0);

	mkidxtbl(curdict);
}


static void
del_stdy(TypeDicSeg seg, TypeDicOfs ofs, int size)
{
	int		i;
	TypeStyNum	stdynum;
	STDYIN		*stdy;
	TypeDicID	dicid;

	if (StudyExist()) {

		stdynum = 0;
		dicid = curdict->dicid;
		for (i = 0, stdy = StudyDict ; i < StudyCount ; ) {
			if (stdy -> dicid != dicid)
				;
			else if (stdy -> seg != seg)
				;
			else if (stdy -> offset > ofs)
				stdy -> offset -= size;

			else if (stdy -> offset == ofs) {
				stdynum = stdy -> styno;
				StudyCount--;
				mvmemi((u_char*)(stdy + 1), (u_char*)stdy,
					sizeof(*stdy) * (StudyCount - i));
				continue;
			}

			i++;
			stdy++;
		}
		for (i = 0, stdy = StudyDict ; i < StudyCount ; i++, stdy++)
			if (stdy -> styno > stdynum) (stdy -> styno)--;

		putstydic();
	}
}
