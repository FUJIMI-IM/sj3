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
 * $SonyRCSfile: study.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:38 $
 */



#include "sj_kcnv.h"

Void	mvmemi(), mvmemd(), memcpy();

STDYIN	*srchstdy();

Int	study(stdy)
STDYOUT	*stdy;
{
	STDYOUT		outp;
	STDYIN		*p1;
	STDYIN		*p2;
	Int		wcnt;
	Int		ret;
	TypeDicSeg	seg;
	TypeDicOfs	ofs;
	TypeStyNum	nm;
	TypeStyNum	wi;

	if (!StudyExist()) return StudyNoDict;

	memcpy((Uchar *)stdy, (Uchar *)&outp, sizeof(outp));

	if (outp.stdy1.offset == 0) return StudyNotStudy;

	p1 = srchstdy(outp.stdy1.seg, outp.stdy1.offset, outp.stdy1.dicid);
	if (p1) {
		nm = p1 -> styno;

		for (wcnt = StudyCount, p2 = StudyDict ; wcnt-- ; p2++) {
			if (p2 -> styno < nm) p2 -> styno++;
		}

		p1 -> styno  = 1;

		if (outp.sttfg) p1 -> sttkj  = outp.stdy1.sttkj;
		if (outp.ka_fg) p1 -> ka_kj  = outp.stdy1.ka_kj;

		p1 -> nmflg = outp.stdy1.nmflg;

		putstydic();

		return StudyNormEnd;
	}


	if (StudyCount >= StudyMax) {

		p1 = StudyDict; p2 = NULL; wi = 0;
		for (wcnt = StudyMax ; wcnt-- ; p1++) {
			if ((nm = p1 -> styno + 1) > wi) {
				wi = nm;
				p2 = p1;
			}

			p1 -> styno = nm;
		}

		if (p2) {
			mvmemi((Uchar *)(p2 + 1), (Uchar *)p2,
				(StudyTail - p2) * StudyRecSize);
			StudyCount -= 1;
		}
		else
			return StudyError;

		ret = StudyFullArea;
	}
	else {
		p1 = StudyDict;
		for (wcnt = StudyCount ; wcnt-- ; p1++) p1 -> styno++;

		ret = StudyNormEnd;
	}

	p1 = StudyDict;
	seg = outp.stdy1.seg;
	for (wcnt = StudyCount ; wcnt && (p1 -> seg < seg) ; ) {
		wcnt--; p1++;
	}

	ofs = outp.stdy1.offset;
	while (wcnt-- && (p1 -> seg == seg)) {
		if (p1 -> offset > ofs) break;
		p1++;
	}

	mvmemd((Uchar *)(StudyTail - 1), (Uchar *)StudyTail,
			(Uchar *)(StudyTail - 1) - (Uchar *)p1);
	p1 -> offset = outp.stdy1.offset;
	p1 -> seg    = outp.stdy1.seg;
	p1 -> dicid  = outp.stdy1.dicid;
	p1 -> styno  = 1;
	p1 -> sttkj  = (outp.sttfg) ? outp.stdy1.sttkj : 0;
	p1 -> ka_kj  = (outp.ka_fg) ? outp.stdy1.ka_kj : 0;
	p1 -> nmflg  = outp.stdy1.nmflg;
	StudyCount += 1;

	putstydic();

	return ret;
}

STDYIN		*srchstdy(seg, ofs, dicid)
TypeDicSeg	seg;
TypeDicOfs	ofs;
TypeDicID	dicid;
{
	Int	high;
	Int	mid;
	Int	low;
	STDYIN	*ptr;


	if (!StudyExist()) return NULL;

	if (!StudyCount || !ofs) return NULL;

	low = 0;
	high = StudyCount - 1;

	for ( ; ; ) {
		mid = ((low + high) >> 1);

		if (StudyDict[mid].seg > seg)
			high = mid - 1;

		else if (StudyDict[mid].seg < seg)
			low = mid + 1;

		else
			break;

		if (low > high) return NULL;
	}

	low = mid + 1;
	ptr = &StudyDict[mid];
	while (ptr -> seg == seg) {
		if (ptr -> offset < ofs) break;
		if (ptr -> dicid == dicid && ptr -> offset == ofs) return ptr;
		if (!mid--) break;
		ptr--;
	}

	ptr = &StudyDict[low];
	mid = StudyCount - low;
	while (mid-- && ptr -> seg == seg) {
		if (ptr -> offset > ofs) break;
		if (ptr -> dicid == dicid && ptr -> offset == ofs) return ptr;
		ptr++;
	}

	return NULL;
}

