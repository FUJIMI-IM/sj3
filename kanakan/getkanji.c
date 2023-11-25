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
 * $SonyRCSfile: getkanji.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:49 $
 */




#include "sj_euc.h"
#include "sj_kcnv.h"



Static  Uchar	*code2kanji();
Int     codesize();



Uchar	*getkan_none(s, d, ym, yl, flg)
Uchar	*s;
Uchar	*d;
Uchar	*ym;
Int	yl;
Int	flg;
{
	return d;
}



Uchar	*getkan_hira(s, d, ym, yl, flg)
Uchar	*d;
Reg2	Uchar	*ym;
Uchar	*s;
Int	yl;
Int	flg;
{
	Reg1	Int	cnt, csize;

	csize = codesize(*s);
	
        cnt = (*s & KanjiCodeMask) + 1;

	
	if (flg && (s[csize] == KanjiStrEnd)) ym += (yl - cnt) * 2;

	
	cnt *= 2;
	while (cnt-- > 0) *d++ = *ym++;

	return d;
}



Uchar	*getkan_kata(s, d, ym, yl, flg)
Reg1	Uchar	*d;
Reg2	Uchar	*ym;
Uchar	*s;
Int	flg;
{
	Reg3	Int	cnt, csize;
	Reg4	Uchar	ch;

	csize = codesize(*s);
	
        cnt = (*s & KanjiCodeMask) + 1;

	
	if (flg && (s[csize] == KanjiStrEnd)) ym += (yl - cnt) * 2;

	
	while (cnt-- > 0) {
		
		if (*ym == 0xa4) {
			ch = *++ym;
			if (0xa1 <= ch && ch <= 0xf3) {
				*d++ = 0xa5;
			}
			else {
				*d++ = 0xa4;
			}
			*d++ = ch;
			ym++;
		}
		else {
			*d++ = *ym++;
			*d++ = *ym++;
		}
	}

	return d;
}



Uchar	*getkan_knj(s, d, ym, yl, flg)
Uchar	*s;
Uchar	*d;
Uchar	*ym;
Int	yl;
Int	flg;
{
	
	return code2kanji(askknj[*s & KnjAssyukuMask], d, ym, yl, flg);
}



Uchar	*getkan_ofs(s, d, ym, yl, flg)
Uchar	*s;
Uchar	*d;
Uchar	*ym;
Int	yl;
Int	flg;
{
	
        return code2kanji(dicbuf + ((*s & KanjiCodeMask) << 8) + *(s + 1),
                                d, ym, yl, flg);
}



Uchar	*getkan_norm(s, d, ym, yl, flg)
Uchar	*s;
Uchar	*d;
Uchar	*ym;
Int	yl;
Int	flg;
{
	Reg2 Int i, csize;
	Reg2 Uchar c;

	if (*s != 0) {
		c = s[1];
		if (c & 0x80) 
		  *d++ = SS3;
		*d++ = *s | 0x80; 
		*d++ =  c | 0x80; 
	} else {
		*d++ = *s;
	}
	return d;
}



Uchar	*getkan_ascii(s, d, ym, yl, flg)
Uchar	*s;
Uchar	*d;
Uchar	*ym;
Int	yl;
Int	flg;
{
	Reg2 Uchar c;

	c = s[1];
	if (c & 0x80)
	  *d++ = SS2;
	*d++ = c;

	return d;
}



Static	Uchar	*code2kanji(s, d, ym, yl, flg)
Uchar	*s;
Uchar	*d;
Uchar	*ym;
Int	yl;
Int	flg;
{
	Reg2 Int csize;

	for ( ; ; ) {
		csize = codesize(*s);
		if (s[csize] == KanjiStrEnd) {
			switch(*s & KanjiModeMask) {
			      case ZenHiraAssyuku:
                                d = getkan_hira(s, d, ym, yl, flg); break;
         
			      case ZenKataAssyuku:
                                d = getkan_kata(s, d, ym, yl, flg); break;
 
			      case OffsetAssyuku:
                                d = getkan_ofs(s, d, ym, yl, flg); break;
 
			      case AiAttribute:
                                d = getkan_none(s, d, ym, yl, flg); break;

			      case LeadingHankaku:
				d = getkan_ascii(s, d, ym, yl, flg); break;
                              
			      case KanjiAssyuku:
				d = getkan_knj(s, d, ym, yl, flg); break;

			      default:
                                        d = getkan_norm(s, d, ym, yl, flg);
			}
			break;
		}
		else
			switch(*s & KanjiModeMask) {
			      case ZenHiraAssyuku:
                                d = getkan_hira(s, d, ym, yl, FALSE); break;
         
			      case ZenKataAssyuku:
                                d = getkan_kata(s, d, ym, yl, FALSE); break;
 
			      case OffsetAssyuku:
                                d = getkan_ofs(s, d, ym, yl, FALSE); break;
 
			      case AiAttribute:
                                d = getkan_none(s, d, ym, yl, FALSE); break;

			      case LeadingHankaku:
				d = getkan_ascii(s, d, ym, yl, FALSE); break;

			      case KanjiAssyuku:
				d = getkan_knj(s, d, ym, yl, FALSE); break;
				
			      default:
				d = getkan_norm(s, d, ym, yl, FALSE);
			}
		
		s += codesize(*s);
	}

	return d;
}



Int	getkanji(ym, yl, ptr, buf)
Uchar	*ym;		
Int	yl;		
Uchar	*ptr;		
Uchar	*buf;		
{
	Uchar	*q;

	
	q = code2kanji(ptr, buf, ym, yl, TRUE);

	
	return q - buf;
}

