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
 * $SonyRCSfile: setjrec.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:17 $
 *
 * $Id$
 */


#include "sj_euc.h"
#include "sj_kcnv.h"
#include "sj_hinsi.h"
#include "kanakan.h"


int
setj_atrb(u_char* p)
{
	return 2;
}


int
setj_ofs(u_char* p)
{
	int	contf = TRUE;

	
        p = dicbuf + ((*p & KanjiCodeMask) << 8) + *(p + 1);

	
	do {
		if (p[codesize(*p)] == KanjiStrEnd) contf = FALSE;
		switch(*p & KanjiModeMask) {
		      case ZenHiraAssyuku:
			p += setj_norm1(p); break;
			
		      case ZenKataAssyuku:
			p += setj_norm1(p); break;
			
		      case OffsetAssyuku:
                        p += setj_ofs(p); break;
			
		      case AiAttribute:
			p += setj_atrb(p); break;
			
		      case LeadingHankaku:
			p += setj_norm2(p); break;

		      case KanjiAssyuku:
			p += setj_knj(p); break;

		      case KanjiStrEnd:
			contf = FALSE; break;

		      default:
			p += setj_norm2(p);
		}
	} while (contf);

	return 2;
}


int
setj_knj(u_char* p)
{
	int	contf = TRUE;

	
	p = askknj[*p & KnjAssyukuMask];

	
	do {
		if (p[codesize(*p)]== KanjiStrEnd) contf = FALSE;
		switch(*p & KanjiModeMask) {
		      case ZenHiraAssyuku:
			p += setj_norm1(p); break;
			
		      case ZenKataAssyuku:
			p += setj_norm1(p); break;
			
		      case OffsetAssyuku:
                        p += setj_ofs(p); break;
			
		      case AiAttribute:
			p += setj_atrb(p); break;

		      case LeadingHankaku:			
			p += setj_norm2(p); break;
			
		      case KanjiAssyuku:
			p += setj_knj(p); break;

		      case KanjiStrEnd:
			contf = FALSE; break;

		      default:
			p += setj_norm2(p);
		}
	} while (contf);

	return 1;
}


int
setj_norm1(u_char* p)
{
	return 1;
}


int
setj_norm2(u_char* p)
{
	return 2;
}


int
setj_norm3(u_char* p)
{
	return 3;
}


static int
chkhead(TypeGram gram)
{
	
	if (!headcode) return TRUE;
	
	
	if (headcode == SETTOU_DAI) {
		
		if (gram == SUUSI) return TRUE;
	}

	
	else {
		gram--;
		if ((Scncttbl(headcode-1, gram >> 3) << (gram & 0x07)) & 0x80) {
			return TRUE;
		}
	}

	return FALSE;
}


void
setjrec(u_char* tagp, int mode)
{
	TypeGram	gram;		
	u_char	*ptr;		
	u_char	*endp;		
	u_char	*tmp;		
	JREC	*rec;		
	int	plen;
	int	nlen;
	int	len;

	
	plen = getplen(tagp);
	nlen = getnlen(tagp);

	
	len = plen + nlen;

	
	if (headcode) len += headlen;

	
        ptr = tagp + DouonBlkSizeNumber + nlen;

	
	endp = tagp + getsize(tagp);

	
	for ( ; ptr < endp ; ptr = tmp + 1) {

		
		for (tmp = ptr + 1; *tmp != HinsiBlkTerm ; ) {
			switch(*tmp & KanjiModeMask) {
			      case ZenHiraAssyuku:
				tmp += setj_norm1(tmp); break;
				
			      case ZenKataAssyuku:
				tmp += setj_norm1(tmp); break;
				
			      case OffsetAssyuku:
				tmp += setj_ofs(tmp); break;
				
			      case AiAttribute:
				tmp += setj_atrb(tmp); break;
				
			      case LeadingHankaku:		
				tmp += setj_norm2(tmp); break;
				
			      case KanjiAssyuku:
				tmp += setj_knj(tmp); break;

			      case KanjiStrEnd:
				tmp++; break;

			      default:
				tmp += setj_norm2(tmp);
			}
		}

		
		if ((gram = *ptr) == IKKATU && !(mode & DO_IKKATU)) continue;

		
		if (!chkhead(gram)) continue;

		
		if (!(rec = argjrec(len, (JREC *)NULL))) continue;

		
		rec -> jseg   = prevseg;
		rec -> jofsst = ptr - dicbuf;
		rec -> jofsed = tmp - dicbuf;
		rec -> class  = C_DICT;
		rec -> dicid  = curdict->dicid;
		rec -> hinsi  = gram;
		rec -> sttofs = headcode;
	}
}


void
setnumrec(u_char* tagp, JREC* rec, TypeGram gram)
{
	u_char		*ptr;		
	u_char		*endp;		
	u_char		*tmp;		
	int		plen, nlen, len;
	JREC		*jrec;

	
	plen = getplen(tagp);
	nlen = getnlen(tagp);

	
	len = plen + nlen;

	
        ptr = tagp + DouonBlkSizeNumber + nlen;

	
	endp = tagp + getsize(tagp);

	
	for ( ; ptr < endp ; ptr = tmp + 1) {

		
		for (tmp = ptr + 1; *tmp != HinsiBlkTerm ; ) {
			switch(*tmp & KanjiModeMask) {
			      case ZenHiraAssyuku:
				tmp += setj_norm1(tmp); break;
				
			      case ZenKataAssyuku:
				tmp += setj_norm1(tmp); break;
				
			      case OffsetAssyuku:
				tmp += setj_ofs(tmp); break;
				
			      case AiAttribute:
				tmp += setj_atrb(tmp); break;
				
			      case LeadingHankaku:
				tmp += setj_norm2(tmp); break;
				
			      case KanjiAssyuku:
				tmp += setj_knj(tmp); break;

			      case KanjiStrEnd:
				tmp++; break;

			      default:
				tmp += setj_norm2(tmp);
			}
		}

		
		if (gram != *ptr) continue;

		
		if (!(jrec = argjrec((int)(rec -> jlen + len), rec))) continue;

		
		jrec -> jseg   = prevseg;
		jrec -> jofsst = ptr - dicbuf;
		jrec -> jofsed = tmp - dicbuf;
		jrec -> dicid  = curdict->dicid;
	}
}


void
setcrec(u_char* tagp)
{
	JREC	*rec;		

 	
	if (!(rec = argjrec((int)ClYomiLen(tagp), (JREC *)NULL))) return;

	
	rec -> jofsst = tagp - ClStudyDict;
	rec -> class  = C_BUNSETU;
	rec -> hinsi  = ClGramCode(tagp);
}
