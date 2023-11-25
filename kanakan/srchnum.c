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
 * $SonyRCSfile: srchnum.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:30 $
 */



#include "sj_kcnv.h"
#include "sj_yomi.h"
#include "sj_hinsi.h"
#include "sj_suuji.h"

JREC	*argjrec();
Uchar	*srchdict();
Void	setnumrec(), memcpy(), memset();

Static Void	srch_kurai1(), srch_kurai2();

Static	Void	srch_josuu_sub(jrec, gram)
JREC		*jrec;
TypeGram	gram;
{
	Uchar	*tagp;
	DICTL	*dp;

	for (dp = dictlist ; dp ; dp = dp -> next) {
		curdict = dp -> dict;
		dicinl  = 1;
		dicsaml = 0;
		prevseg = -1;
		while (tagp = srchdict(tagp)) setnumrec(tagp, jrec, gram);
	}
}

Static	Void	srch_josuu(jrec)
JREC	*jrec;
{
	Uchar	*kp;
	Int	klen;
	Int	len;

	if (headcode && headcode != SETTOU_DAI) return;

	kp   = cnvstart;
	klen = cnvlen;

	cnvstart += (len = jrec -> numlen);
	cnvlen   -= len;

	srch_josuu_sub(jrec, JOSUUSI);

	if (!headcode) srch_josuu_sub(jrec, JOSUUSI2);

	cnvstart = kp;
	cnvlen   = klen;
}

Static	Void	setnrec_sub(p, flag, stb)
Uchar	*p;
Ushort	flag;
Int	stb;
{
	JREC	*rec;
	Int	len1;
	Int	len2;

	len1 = len2 = p - suuji_yptr;
	if (headcode) len1 += headlen;
	if (stb) len2--;

	if ((Int)suuji_len == len2) {
		if (suuji_wkeep) {
			memcpy((Uchar *)suuji_wbuf, (Uchar *)suuji_wkeep,
				sizeof(suuji_wbuf[0]) * NumWordBuf);
		}
		if (suuji_ukeep) {
			memcpy(suuji_ubuf, suuji_ukeep, NumKetaLength);
		}
		suuji_exit = suuji_keta;
		return;
	}

	if (rec = argjrec(len1, (JREC *)NULL)) {
		rec -> class  = suuji_class;
		rec -> hinsi  = SUUSI;
		rec -> sttofs = headcode;
		rec -> stbofs = (Uchar)stb;
		rec -> flags  = flag;
		rec -> numlen = (Uchar)len2;

		if (!stb) srch_josuu(rec);
	}
}

Static	Void	setnrec(p, flag)
Uchar	*p;
Ushort	flag;
{
	setnrec_sub(p, flag, 0);

	if (!headcode && (*p == Y_S_KIGOU2)) {
		setnrec_sub(p + 1, flag, -1);
	}
}

Static	Uchar	TFar	*srchtbl(ch, tbl, rec, n)
Uchar	ch;
Uchar	TFar	*tbl;
Int	rec;
Int	n;
{
	Int	high, low, mid;
	Uchar	TFar	*p;

	if (!ch) return NULL;

	low = 0;
	high = n - 1;

	while (low <= high) {
		mid = (low + high) / 2;
		p = tbl + mid * rec;

		if (ch == *p)
			break;

		else if (ch < *p)
			high = mid - 1;

		else
			low = mid + 1;
	}

	if (low > high) return NULL;

	while (mid-- > 0 && ch == *(p - rec)) p -= rec;

	return p;
}

Static	Int	isconnect(cnct, num)
Uchar	TFar	*cnct;
Int	num;
{
	return (cnct[num / 8] & (0x80 >> (num % 8)));
}

Static	Int	string_cmp(s, l, d)
Reg3	Uchar	TFar	*s;
Reg1	Int	l;
Reg2	Uchar	*d;
{
	Uchar	TFar	*p;

	p = s;

	while (*s && l-- > 0) {
		if (*s != *d) return 0;
		s++;
		d++;
	}

	return (s - p);
}

Static	Int	check_num(ptr)
Uchar	*ptr;
{
	Int	i;
	Int	j;
	Int	k;
	int	tmp;
	Int	hketa;
	Int	lketa;
	Int	flg = FALSE;
	Int	cnt;
	Ushort	flag;

	memset((Uchar *)suuji_wbuf, 0, sizeof(Ushort) * NumWordBuf);
	flag = 0;

	hketa = lketa = cnt = 0;
	for (i = suuji_keta ; --i >= 0 ;) {
		if (hketa >= 4) return FALSE;
		tmp = suuji_ubuf[i];
		if (j = ((tmp >> 6) & 3)) {
			if (j < hketa) return FALSE;
			if (j > hketa) {
				flag |= JFLAG_N01;
				do {
					suuji_wbuf[hketa] |= (_Num0<<(lketa*4));
					if (++lketa >= 4) {
						lketa = 0;
						hketa++;
					}
				} while (j > hketa);
			}
		}
		k = ((tmp >> 4) & 3);
		if (j || k) {
			if (k < lketa) return FALSE;
			if (k > lketa) {
				flag |= JFLAG_N01;
				do {
					suuji_wbuf[hketa] |= (_Num0<<(lketa*4));
					lketa++;
				} while (k > lketa);
			}
			flg = TRUE;
		}
		tmp &= 0x0f;
		suuji_wbuf[hketa] |= (tmp << (lketa * 4));
		cnt++;

		j = (1 << tmp);
		if (j & NumberS1) flag |= JFLAG_NS1;
		if (j & Number01) flag |= JFLAG_N01;
		if (j & Number23) flag |= JFLAG_N23;

		if (tmp != _Num0) {
			flg = FALSE;

			switch (lketa) {
			case 0:
				if (tmp == _Num1) flag |= JFLAG_N_1;
				break;

			case 1:
				flag |= JFLAG_N10;
				break;
			}
		}
		if (++lketa >= 4) {
			lketa = 0;
			hketa++;
		}
	}

	if (flg) return FALSE;

	if (suuji_keta == 1 || tmp != _Num0) flag |= JFLAG_NN0;

	if (hketa)
		flag |= (JFLAG_NK4 | JFLAG_NK2);
	else if (lketa > 1)
		flag |= JFLAG_NK2;

	if (cnt > 1) flag |= JFLAG_NSN;

	setnrec(ptr, flag);

	return TRUE;
}

Static	Void	srch_number1(ptr)
Uchar	*ptr;
{
	Uchar		ch;
	Uchar		mode;
	Uchar		TFar	*p;
	Int		l;
	TypeClass	cls;
	Int		len;
	Uchar		*tptr;

	if (suuji_keta >= NumKetaLength) return;

	if (!(p = srchtbl((ch = *ptr), Suuji_tbl, SjTblRecLen, SjTblRecNum)))
		return;

	len = suuji_yptr + MaxClInputLen - ptr;

	for ( ; *SjTblYPos(p) == ch ; p += SjTblRecLen) {
		if (!(l = string_cmp(SjTblYPos(p), SjTblYLen, ptr))) continue;

		if (l > len) continue;

		mode = *SjTblModeP(p);
		suuji_ubuf[suuji_keta++] = SjTblNumber(mode);
		tptr = ptr + l;

		switch (suuji_class) {
		case C_N_ARABIA:
			if (!(mode & SjTblNArabiaF)) break;

			check_num(tptr);
			srch_number1(tptr);
			srch_kurai2(tptr, (Uchar TFar *)NULL);

			break;

		case C_N_SUUJI:
			if (mode & (SjTblNArabiaF | SjTblNYomiF)) break;

			check_num(tptr);
			srch_number1(tptr);
			srch_kurai1(tptr, (Uchar TFar *)NULL);
			srch_kurai2(tptr, (Uchar TFar *)NULL);

			break;

		case C_N_KAZU:
		case C_N_NANSUU:
			if (mode & SjTblNNarabiF) break;
			if (mode & SjTblNNanSuuF) {
				cls = suuji_class;
				suuji_class = C_N_NANSUU;
			}

			check_num(tptr);
			srch_kurai1(tptr, SjTblCnctP(p));
			srch_kurai2(tptr, SjTblCnctP(p));

			if (mode & SjTblNNanSuuF) suuji_class = cls;
			break;

		default:
			if (mode & SjTblNArabiaF) {
				suuji_class = C_N_ARABIA;
				check_num(tptr);
				srch_number1(tptr);
				srch_kurai2(tptr, (Uchar TFar *)NULL);

				suuji_class = C_N_KAZU;
			}
			else if (mode  & SjTblNYomiF) {
				if (mode & SjTblNNanSuuF)
					suuji_class = C_N_NANSUU;
				else
					suuji_class = C_N_KAZU;
				check_num(tptr);
			}
			else if (mode & SjTblNNarabiF) {
				suuji_class = C_N_SUUJI;
				check_num(tptr);
				srch_number1(tptr);
				break;
			}
			else {
				suuji_class = C_N_SUUJI;
				srch_number1(tptr);

				suuji_class = C_N_KAZU;
				check_num(tptr);
			}
			srch_kurai1(tptr, SjTblCnctP(p));
			srch_kurai2(tptr, SjTblCnctP(p));
			break;
		}

		suuji_keta--;
	}
}

Static	Void	srch_kurai1(ptr, cnct)
Uchar	*ptr;
Uchar	TFar	*cnct;
{
	Uchar		ch;
	Uchar		TFar	*p;
	Int		l;
	Int		keta;
	Int		mode;
	TypeClass	cls;
	Int		len;
	Uchar		*tptr;

	if (!(p = srchtbl((ch = *ptr), Kurai1_tbl, Kr1TblRecLen, Kr1TblRecNum)))
		return;

	len = suuji_yptr + MaxClInputLen - ptr;

	for ( ; *Kr1TblYPos(p) == ch ; p += Kr1TblRecLen) {
		if (!(l = string_cmp(Kr1TblYPos(p), Kr1TblYLen, ptr))) continue;

		if (l > len) continue;

		mode = *Kr1TblModeP(p);
		keta = Kr1TblKeta(mode);
		tptr = ptr + l;

		if (suuji_class != C_N_KAZU && suuji_class != C_N_NANSUU) {
			cls = suuji_class;
			suuji_class = (suuji_class == C_N_ARABIA) ?
				C_N_ARAKURA : C_N_KANKURA;
			suuji_ubuf[suuji_keta - 1] |= (keta << 4);
			check_num(tptr);
			suuji_class = cls;
			suuji_ubuf[suuji_keta - 1] &= 0xcf;
			continue;
		}

		if (cnct) {
			if (!isconnect(cnct, (Int)Kr1TblCnct(mode))) continue;
			suuji_ubuf[suuji_keta - 1] |= (keta << 4);
		}
		else {
			if (!Kr1TblTop(mode)) continue;
			if (suuji_keta >= NumKetaLength) continue;
			suuji_ubuf[suuji_keta++] = (_Num1 | (keta << 4));
		}

		check_num(tptr);

		srch_number1(tptr);
		srch_kurai1(tptr, (Uchar TFar *)NULL);
		srch_kurai2(tptr, Kr1TblCnctP(p));

		if (cnct)
			suuji_ubuf[suuji_keta - 1] &= 0xcf;
		else
			suuji_keta--;
	}
}

Static	Void	srch_kurai2(ptr, cnct)
Uchar	*ptr;
Uchar	TFar	*cnct;
{
	Uchar		ch;
	Uchar		TFar	*p;
	Int		l;
	Int		keta;
	Int		mode;
	TypeClass	cls;
	Int		len;
	Uchar		*tptr;

	if ((suuji_ubuf[0] & 0x0f) == _Num0) return;

	if (!(p = srchtbl((ch = *ptr), Kurai2_tbl, Kr2TblRecLen, Kr2TblRecNum)))
		return;

	len = suuji_yptr + MaxClInputLen - ptr;

	for ( ; *Kr2TblYPos(p) == ch ; p += Kr2TblRecLen) {
		if (!(l = string_cmp(Kr2TblYPos(p), Kr2TblYLen, ptr))) continue;

		if (l > len) continue;

		mode = *Kr2TblModeP(p);
		keta = Kr2TblKeta(mode);
		tptr = ptr + l;

		if (suuji_class != C_N_KAZU && suuji_class != C_N_NANSUU) {
			suuji_ubuf[suuji_keta - 1] |= (keta << 6);
			cls = suuji_class;
			suuji_class = (suuji_class == C_N_ARABIA) ?
				C_N_ARAKURA : C_N_KANKURA;
			check_num(tptr);
			suuji_class = cls;
			suuji_ubuf[suuji_keta - 1] &= 0x3f;
			continue;
		}

		if (cnct) {
			if (!isconnect(cnct, (Int)Kr2TblCnct(mode))) continue;
			suuji_ubuf[suuji_keta - 1] |= (keta << 6);
		}
		else {
			if (suuji_keta >= NumKetaLength) continue;
			suuji_ubuf[suuji_keta++] = (_Num1 | (keta << 6));
		}

		check_num(tptr);

		srch_number1(tptr);
		srch_kurai1(tptr, (Uchar TFar *)NULL);

		if (cnct)
			suuji_ubuf[suuji_keta - 1] &= 0x3f;
		else
			suuji_keta--;
	}
}

Static	Void	srch_number2(p)
Uchar	*p;
{
	Uchar	ch;
	Int	i;

	suuji_class = (*p == N_0) ? C_N_SUUJILONG : C_N_KAZULONG;

	suuji_keta = suuji_comma = 0;
	for (i = 0 ; i < MaxClInputLen ; i++) {
		if ((ch = *p++) == Y_COMMA) {
			if (suuji_keta) {
				if (suuji_comma) {
					if (suuji_comma != 3) break;
				}
				else {
					if (suuji_keta >= 4) break;
					if (*suuji_yptr == N_0) break;
				}
			}
			else
				break;

			suuji_comma = 1;
			ch = *p++;
		}
		else if (suuji_comma) {
			suuji_comma++;
		}

		if (suuji_comma > 3) break;

		if (!(Chrtbl[ch] & NUMBER)) break;

		if (suuji_keta < NumKetaLength) {
			suuji_ubuf[suuji_keta] = ch - N_0 + _Num0;
		}
		suuji_keta++;

		if (!suuji_comma && suuji_keta > NumKetaLength) {
			setnrec(p, 0);
		}
		else if (suuji_comma == 3) {
			if (suuji_keta <= NumKetaLength) {
				suuji_class = C_N_ARACMA;
				check_num(p);
			}
			else {
				suuji_class = C_N_KAZULCMA;
				setnrec(p, 0);
			}
		}
	}
}

Static	Void	srchnum_sub(p)
Uchar	*p;
{
	suuji_yptr = p;

	suuji_comma = suuji_keta = 0;
	suuji_class = 0;
	srch_number1(p);

	suuji_keta = 0;
	suuji_class = C_N_KAZU;
	srch_kurai1(p, (Uchar TFar *)NULL);
	if (suuji_exit) return;

	srch_number2(p);
}

Void	srchnum()
{
	suuji_len = suuji_exit = 0;

	srchnum_sub(cnvstart);

	if (!headcode && (*cnvstart == Y_S_KIGOU1)) {
		headcode = SETTOU_KIGOU;
		headlen = 1;

		srchnum_sub(cnvstart + 1);

		headcode = headlen = 0;
	}
}

Void	setwdnum(p, len, wd)
Uchar	*p;
Int	len;
Ushort	*wd;
{
	suuji_len   = (Uchar)len;
	suuji_wkeep = wd;
	suuji_ukeep = NULL;
	headcode = headlen = 0;
	suuji_exit = 0;

	srchnum_sub(p);
}

Int	setucnum(p, len, ud)
Uchar	*p;
Int	len;
Uchar	*ud;
{
	suuji_len   = (Uchar)len;
	suuji_wkeep = NULL;
	suuji_ukeep = ud;
	headcode = headlen = 0;
	suuji_exit = 0;

	srchnum_sub(p);

	return (Int)suuji_exit;
}

