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
 * $SonyRCSfile: adddic.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:26 $
 *
 * $Id$
 */


#include <sys/types.h>
#include <string.h>
#include "sj_euc.h"
#include "sj_kcnv.h"
#include "kanakan.h"


static u_int checkdict(u_char* kanji, TypeGram grm);
static int cal_nextym(u_char* ptr);
static int usr_freelen(void);
static void sprt_seg(TypeDicSeg seg, TypeDicOfs ofs);
static void apnd_uidx(TypeDicSeg seg, u_char* yomi, int len);


u_int
adddic(u_char* yomi, u_char* kanji, TypeGram  hinsi)
{
	u_int		err;			
	u_char		yptr[MaxWdYomiLen+1];	
	u_char		kptr[MaxWdKanjiLen+1];	
	int		klen;			
	TypeDicSeg	segnum;			
	int		saml;			
	TypeDicOfs	dstofs;			
	TypeDicOfs	douofs;			
	TypeDicOfs	hnsofs;			
	TypeDicOfs	knjofs;			
	TypeDicOfs	nxtask;			
	TypeDicOfs	newsiz;			
	TypeDicOfs	oldsiz;			
	TypeDicOfs	fresiz;			
	TypeIdxOfs	freidx;			
	u_char		*douptr;
	u_char		*dstptr;
	TypeDicOfs	pos;
	u_char		*p1;			
	int		i;
	int		nlen, plen;
	int		size;
	STDYIN	*stdy;
	STDYOUT	stdydat;

	if ((err = addel_arg(yomi, kanji, hinsi, yptr, sizeof(yptr))) != 0)
		return err;

	inputyomi = yomi;
	cnvstart = ystart = yptr;
	cnvlen = strlen(yptr);

	if ((err = checkdict(kanji, hinsi)) != 0)
		return err;

	if (!curdict -> maxunit) return AD_NotUserDict;

	klen = cvtknj(yomi, kanji, kptr);

        newsiz = DouonBlkSizeNumber + cnvlen + (klen !=0 ? klen+1 : 0) + 2; 

	if (newsiz > MaxDouonBlk) return AD_OvflwDouBlk;
	if (newsiz > curdict->seglen) return AD_OvflwDouBlk;

	segnum = srchidx((TypeDicSeg)DicSegBase, cnvlen);

	for ( ; ; ) {
		(*curdict->getdic)(curdict, segnum);
		i = srchkana(&p1, &saml);
		dstofs = douofs = p1 - dicbuf;
		hnsofs = knjofs = 0;
		if (i) {
			i = srchgram(p1, &p1, hinsi);
			dstofs = hnsofs = p1 - dicbuf;
			if (i) {
				srchkanji(&p1, kptr, klen);
				if (*p1 != HinsiBlkTerm) return AD_ArExist;
				dstofs = knjofs = p1 - dicbuf;
			}
		}

		p1 = dicbuf + douofs;

		if (!hnsofs) {
			oldsiz = 0;
			nlen = cnvlen - saml;         
		        newsiz = DouonBlkSizeNumber + nlen;

			nxtask = cal_nextym(p1);
		}
		else {
			oldsiz = newsiz = getsize(p1);
			nxtask = 0;
		}
		if (!knjofs) newsiz += 2;

	        newsiz += (klen != 0 ? klen+1 : 0); 

		fresiz = usr_freelen();

		if ((unsigned int) (newsiz - oldsiz - nxtask) <= 
		    (unsigned int) fresiz) break;

		if (segend(p1) && (segnum + 1 < curdict->segunit)) {
			segnum++;
			continue;
		}

		if (curdict->segunit >= curdict->maxunit)
			return AD_OvflwUsrDic;

		freidx = count_uidx();

		if ((unsigned int) (douofs + newsiz) <= curdict->seglen) {
			if (hnsofs) p1 = getntag(p1);
			pos = p1 - dicbuf;

			nlen = getnlen(p1);              
		        if ((unsigned int) freidx < getplen(p1) + nlen + 1)
				return AD_OvflwIndex;

			if ((*curdict->putdic)(curdict, curdict->segunit))
				return AD_OvflwUsrDic;

			sprt_seg(segnum, pos);

			nxtask = 0;
			break;
		}

		if (segend(p1)) {
			if ((unsigned int) freidx < cnvlen + 1) return AD_OvflwIndex;

			if ((*curdict->putdic)(curdict, curdict->segunit))
				return AD_OvflwUsrDic;
			curdict->segunit++;

			(*curdict->getdic)(curdict, curdict->segunit - 1);
			memset(dicbuf, DicSegTerm, (int)curdict->seglen);
			dicbuf[0] = 0;
			(*curdict->putdic)(curdict, curdict->segunit - 1);
			(*curdict->rszdic)(curdict, curdict->segunit);
			mkidxtbl(curdict);

			segnum++;
		}

		else {
			nlen = getnlen(p1);         
			if ((unsigned int) freidx < getplen(p1) + nlen + 1)
				return AD_OvflwIndex;

			if ((*curdict->putdic)(curdict, curdict->segunit))
				return AD_OvflwUsrDic;

			sprt_seg(segnum, douofs);

			segnum++;
		}
	}

	douptr = dicbuf + douofs;

	if (nxtask) {
		u_char	*tp;

		size = getsize(douptr) - nxtask;
		plen = getplen(douptr) + nxtask;
		nlen = getnlen(douptr) - nxtask;
		*douptr = (size >> 8);
		if (nlen & 0x10) *douptr |= NokoriYomiTop;
		if (plen & 0x10) *douptr |= AssyukuYomiTop;
		*(douptr + 1) = size;
		*(douptr + 2) = ((nlen & 0x0f) | ((plen & 0x0f) << 4));

		p1 = douptr + DouonBlkSizeNumber;
		tp = p1 + nxtask;
		mvmemi(tp, p1, dicbuf + curdict->seglen - tp);
	}

	dstptr = dicbuf + dstofs;
	size = newsiz - oldsiz;
	p1 = dicbuf + curdict->seglen;
	mvmemd(p1 - size, p1, p1 - dstptr - size);

	if (!hnsofs) {
		if (douofs == segtop() - dicbuf) {
			chg_uidx(segnum, yptr, cnvlen);
		}

		*dstptr   = 0;
		nlen = cnvlen - saml;
		if (nlen & 0x10) *dstptr |= NokoriYomiTop;
		if (saml & 0x10) *dstptr |= AssyukuYomiTop;
		*dstptr++ |= (size >> 8);
		*dstptr++  = size;
		*dstptr++  = ((nlen&0x0f)|((saml&0x0f)<<4));

		while (saml < cnvlen) *dstptr++ = yptr[saml++];
	}
	
	else {
		plen = getplen(douptr);
		nlen = getnlen(douptr);
		*douptr = (newsiz >> 8);
		if (nlen & 0x10) *douptr |= NokoriYomiTop;
		if (plen & 0x10) *douptr |= AssyukuYomiTop;
		*(douptr + 1) = newsiz;
		*(douptr + 2) = ((nlen & 0x0f) | ((plen & 0x0f) << 4));
	}

	if (!knjofs) *dstptr++ = hinsi;

	stdydat.stdy1.offset = dstptr - dicbuf;
	for (saml = 0 ; saml < klen ; ) *dstptr++ = kptr[saml++];
        if (klen > 0) *dstptr++ = KanjiStrEnd;

	if (!knjofs) *dstptr++ = HinsiBlkTerm;

	(*curdict->putdic)(curdict, segnum);

	if (StudyExist()) {
		size -= nxtask;
		for (i = StudyCount, stdy = StudyDict ; i-- ; stdy++) {
			if (stdy -> dicid != curdict->dicid) continue;
			if (stdy -> seg != segnum) continue;
			if (stdy -> offset >= dstofs) stdy -> offset += size;
		}

		for (p1 = ClStudyDict ; !iseocl(p1) ; p1 = ClNextTag(p1)) {
			err = strncmp(yptr, (u_char*)ClYomiPos(p1),
					(int)ClYomiLen(p1));
			if (err == MATCH) {
				delclsub(p1);		
				mkclidx();		
				putcldic();		
				break;
			}
			else if (err == OVER)
				break;
		}
	}

	stdydat.stdy1.seg    = segnum;
	stdydat.stdy1.dicid  = curdict->dicid;
	stdydat.stdy1.styno  = 0;
	stdydat.stdy1.sttkj  = 0;
	stdydat.stdy1.ka_kj  = 0;
	stdydat.stdy1.nmflg  = 0;
	stdydat.hinshi       = hinsi;
	stdydat.len          = cnvlen;
	stdydat.sttlen       = 0;
	stdydat.sttfg        = 0;
	stdydat.ka_fg        = 0;
	(void)study(&stdydat);

	return AD_Done;
}


static int
checksub(u_char* kanji, TypeGram grm)
{
	u_char	*tagp;		
	u_char	*ptr;
	u_char	*endp;
	int	flg, nlen;
	int	dounum;
	u_char	buf[MaxWdKanjiLen + 1];

	dicinl = cnvlen;
	dicsaml = 0;
	prevseg = (TypeDicSeg)-1;
	for (;;) {
		if (!(tagp = srchdict(tagp))) return 0;
		if (cnvlen == getnlen(tagp) + getplen(tagp))
			break;
	}

	dounum = 0;
	nlen = getnlen(tagp);              
        ptr = tagp + DouonBlkSizeNumber + nlen;
	endp = tagp + getsize(tagp);

	while (ptr < endp) {
		flg = (grm == *ptr++);
		while (*ptr != HinsiBlkTerm) {
			if (flg) {
				buf[getkanji(inputyomi, cnvlen, ptr, buf)] = 0;
				if (cmpstr(kanji, buf)) return -1;
			}

			dounum++;
			ptr = skipkstr(ptr);
		}

		ptr++;
	}

	return dounum;
}


static u_int
checkdict(u_char* kanji, TypeGram grm)
{
	DICTL	*dp;
	int	dounum = 0;
	int	tmp;
	DICT	*tmpdict;

	tmpdict = curdict;
	for (dp = dictlist ; dp ; dp = dp -> next) {
		curdict = dp -> dict;
		if ((tmp = checksub(kanji, grm)) < 0) {
			curdict = tmpdict;
			return AD_ArExist;
		}

		dounum += tmp;
	}
	curdict = tmpdict;

	if (MaxKouhoNumber <= dounum) return AD_OvflwDouNum;

	return 0;
}


static int
cal_nextym(u_char* ptr)
{
	int	count = 0;
	u_char	*src;
	int	len;
	
	if (!segend(ptr)) {
		src = cnvstart + getplen(ptr);
		len = getnlen(ptr);
		ptr += DouonBlkSizeNumber;

		while (len--) {
			if (*ptr++ == *src++)
				count++;
			else
				break;
		}
	}

	return count;
}


static int
usr_freelen(void)
{
	u_char* ptr;

	ptr = segtop();
	while (!segend(ptr)) ptr += getsize(ptr);

	return(curdict->seglen - (ptr - dicbuf));
}


static void
sprt_seg(TypeDicSeg seg, TypeDicOfs ofs)
{
	TypeDicSeg	s;
	u_char	*pos;
	u_char	*p;
	u_char	*q;
	int		i;
	int		j;
	int		ylen, nlen;
	TypeDicID	dicid;
	u_char		yomi[MaxWdYomiLen];
	STDYIN		*stdy;

	for (s = curdict->segunit - 1 ; s >= seg ; s--) {
		(*curdict->getdic)(curdict, s);
		(*curdict->putdic)(curdict, s + 1);
	}
	curdict->segunit++;
	(*curdict->rszdic)(curdict, curdict->segunit);

	(*curdict->getdic)(curdict, seg + 1);

	pos = dicbuf + ofs;

	for (p = segtop() ; (p <= pos) && !segend(p) ; p = q) {
		q = getntag(p);
		i = getplen(p);
		nlen = getnlen(p);
	        j = (nlen != 0) ? nlen+1: 0;
		p += DouonBlkSizeNumber;
		while (j-- > 0) yomi[i++] = *p++;
	}

	nlen = getnlen(pos);
        ylen = (i = getplen(pos)) + (j = nlen);

	apnd_uidx(seg, yomi, ylen);

        set_size(segtop(), getsize(pos) + i, 0, ylen);

	mvmemi(yomi, (p = segtop() + DouonBlkSizeNumber), ylen);

	q = pos + DouonBlkSizeNumber + j;	
	i = dicbuf + curdict->seglen - q;	
	mvmemi(q, p += ylen, i);
	j = q - p;				

	p += i;
	memset(p, DicSegTerm, (dicbuf + curdict->seglen - p));

	(*curdict->putdic)(curdict, seg + 1);

	if (StudyExist()) {
		dicid = curdict -> dicid;
		for (i = StudyCount, stdy = StudyDict ; i-- > 0 ; stdy++) {
			if (stdy -> dicid != dicid)
				continue;
			else if (stdy -> seg < seg)
				continue;
			else if (stdy -> seg > seg)
				stdy -> seg += 1;
			else if (stdy -> offset >= ofs) {
				stdy -> seg += 1;
				stdy -> offset -= j;
			}
		}
	}

	(*curdict->getdic)(curdict, seg);
	memset(dicbuf + ofs, DicSegTerm, (int)(curdict->seglen - ofs));
	(*curdict->putdic)(curdict, seg);
}


static void
apnd_uidx(TypeDicSeg seg, u_char* yomi, int len)
{
	u_char	*p;
	u_char	*q;

	p = get_idxptr(seg);
	while (*p++) ;

	q = idxbuf + curdict->idxlen;
	mvmemd(q - (len + 1), q, (int)(q - (len + 1) - p));

	while (len--) *p++ = *yomi++;
	*p = 0;

	(*curdict->putidx)(curdict, 0);

	mkidxtbl(curdict);
}
