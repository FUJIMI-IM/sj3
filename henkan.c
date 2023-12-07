/*
 * Copyright (c) 1994  Sony Corporation
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
 * $SonyRCSfile: henkan.c,v $  
 * $SonyRevision: 1.2 $ 
 * $SonyDate: 1997/01/23 11:09:37 $
 */



#include "common.h"
#include "key.h"
#include "wchar16.h"
#include "kctype.h"

void Rdisp();

int	i_point;		

exec_henkan ()
{
	register Conversion	*cv;
	register int		i, j;
	register wchar16_t		*s, *p;
	short			sp;
	int			len, rlen, ilen;
	int			opos, ipos, bnum;
	u_char			index[BUFFLENGTH * 2], mode;

	cv = GetConversion ();
	sp = 0;
	mode = cv->in_mode[sp];
	cv->kettei[sp] = mode;
	for (i = 1; i < cv->out_point; i++) {
		if (mode != cv->in_mode[i]) {
			p = &cv->out_buf[i];
			cv->span_l[sp] = p - cv->span_p[sp];
			sp++;
			cv->span_p[sp] = p;
			mode = cv->in_mode[i];
			cv->kettei[sp] = mode;
		}
	}
	cv->span_l[sp] = &cv->out_buf[i] - cv->span_p[sp];
	sp++;
	cv->span_point = sp;

	p = cv->yomiout;
	i_point = 0;
	ipos = -1;
	for (i = 0; i < cv->span_point; i++) {
		len = cv->span_l[i];

		s = cv->span_p[i];
		if (cv->kettei[i] == MODE_ZHIRA) {
			if (ipos >= 0) {
				ilen = getipos(s - cv->out_buf) - ipos;
				Strncpy(p, &cv->Halpha[ipos], ilen);
				p += ilen;
				ipos = -1;
				for (j = 0; j < ilen; j++)
					index[i_point++] = ObjMuhenkan;
			}
			Strncpy(p, s, len);
			p += len;
			for (j = 0; j < len; j++)
				index[i_point++] = ObjHenkan;
		} else if (ipos == -1)
			ipos = getipos(s - cv->out_buf);
	}

	if (ipos >= 0) {
		ilen = cv->ha_point - ipos;
		Strncpy(p, &cv->Halpha[ipos], ilen);
		p += ilen;
		for (j = 0; j < ilen; j++)
			index[i_point++] = ObjMuhenkan;
	}
	index[i_point] = ObjEnd;
	*p = '\0';
			
	bnum = SJ2_henkan(cv->yomiout, cv->Bun, cv->kanjiout, BUFFLENGTH*4);

        
        if (bnum < 0 && !dconnect(1))
            bnum = SJ2_henkan(cv->yomiout, cv->Bun, cv->kanjiout, BUFFLENGTH*4);

	if (bnum <= 0) {
		cv->Bun[0].srclen = i_point;
		cv->Bun[0].srcstr = cv->yomiout;
		cv->Bun[0].destlen = 0;
		cv->Bun[0].deststr = NULL;
		bnum = 1;
	}

	i_point = 0;
	cv->Bnum = 0;
	i = 0;
	while (i < bnum && i < cv->span_point) {
		s = cv->span_p[i];
		len = cv->span_l[i];

		if (index[i_point] == ObjMuhenkan) {
			cv->Bkettei[cv->Bnum] = Bmuhenkan;
			opos = s - cv->out_buf;
			len = getipos(opos + len) - getipos(opos);
			if (len < cv->Bun[i].srclen) {
				i_point += len;
				rlen = cv->Bun[i].srclen - len;
				opos = cv->span_p[i+1] - cv->out_buf; 
				if (index[i_point+rlen] == ObjMuhenkan ||
				    rlen != 1 || cv->out_mode[opos+rlen] == 0) {
					addBun(i, bnum);
					cv->Bun[i].srclen = len;
					cv->Bun[i+1].srclen -= len;
					cv->Bun[i+1].srcstr += len;
					if (cv->Bun[i].deststr != NULL) {
						cv->Bun[i].destlen = len;
						cv->Bun[i+1].destlen -= len;
						cv->Bun[i+1].deststr += len;
					}
					bnum++;
				} else {
					cv->span_l[i] += rlen;
					if ((unsigned int)cv->span_l[i + 1] >= rlen) {
						cv->span_l[i + 1] -= rlen;
						cv->span_p[i + 1] += rlen;
					} else {
						delspan(i + 1);
						cv->span_point--;
					}
					i_point += rlen;
				}
			} else if (len > cv->Bun[i].srclen) {
				delBun(i, bnum);
				bnum--;
				continue;
			} else {
				i_point += len;
			}
		} else if (index[i_point] == ObjHenkan) {
			cv->Bkettei[cv->Bnum] = Bhenkan;
			if (len < cv->Bun[i].srclen) {
				i_point += len;
				addBun(i, bnum);
				cv->Bun[i].srclen = len;
				cv->Bun[i+1].srclen -= len;
				cv->Bun[i+1].srcstr += len;
				if (cv->Bun[i].deststr != NULL) {
					rlen = cv->Bun[i+1].srclen;
					cv->Bun[i].destlen -= rlen;
					cv->Bun[i+1].destlen = rlen;
					cv->Bun[i+1].deststr += 
							cv->Bun[i].destlen;
				}
				bnum++;
			} else if (len > cv->Bun[i].srclen) {
				for (j = cv->span_point; j > i; j--) {
					cv->span_p[j] = cv->span_p[j-1];
					cv->span_l[j] = cv->span_l[j-1];
					cv->kettei[j] = cv->kettei[j-1];
				}
				cv->span_point++;
				rlen = cv->Bun[i].srclen;
				i_point += rlen;
				cv->span_l[i] = rlen;
				cv->span_l[i + 1] -= rlen;
				cv->span_p[i + 1] += rlen;
			} else {
				i_point += len;
			}
		} else {
			cv->Bnum++;
			break;
		}
		cv->Bnum++;
		i++;
	}
	cv->Bkettei[cv->Bnum] = Bend;

	for (i = 0 ; i < cv->Bnum ; i ++) {
		cv->Pkettei[i] = cv->kettei[i];
		cv->Ppoint[i] = cv->span_p[i];
		cv->OrgPlen[i] = cv->Plen[i] = cv->span_l[i];
		cv->Pedited[i] = 0;
		cv->Bpoint[i] = i + 1;
	}

	if (flush_conversion)
		cv->PreBun = cv->CurBun = -1;
	else
		cv->PreBun = cv->CurBun = 0;

	Bdisp (1);

	cv->Dflag = -1;	
	cv->Enum = 0;
	cv->e_point = 0;
	cv->Kanji = 1;
}

addBun(cur, end)
int cur, end;
{
	register int i;
	register Conversion *cv;

	cv = GetConversion();
	for (i = end; i > cur; i--) {
		cv->Bun[i].srclen = cv->Bun[i - 1].srclen;
		cv->Bun[i].srcstr = cv->Bun[i - 1].srcstr;
		cv->Bun[i].destlen = cv->Bun[i - 1].destlen;
		cv->Bun[i].deststr = cv->Bun[i - 1].deststr;
	}
}

delspan(cur)
int cur;
{
	register int i;
	register Conversion *cv;

	cv = GetConversion();
	for (i = cur; i < cv->span_point - 1; i++) {
		cv->span_l[i] = cv->span_l[i + 1];
		cv->span_p[i] = cv->span_p[i + 1];
		cv->kettei[i] = cv->kettei[i + 1];
	}
}

delBun(cur, end)
int cur, end;
{
	register int i;
	register Conversion *cv;

	cv = GetConversion();
	cv->Bun[cur].srclen += cv->Bun[cur + 1].srclen;
	cv->Bun[cur].destlen += cv->Bun[cur + 1].destlen;
	for (i = cur + 1; i < end; i++) {
		cv->Bun[i].srclen = cv->Bun[i + 1].srclen;
		cv->Bun[i].srcstr = cv->Bun[i + 1].srcstr;
		cv->Bun[i].destlen = cv->Bun[i + 1].destlen;
		cv->Bun[i].deststr = cv->Bun[i + 1].deststr;
	}
}


IsHex (c)
register wchar16_t c;
{
	return(iswxdigit(c));
}

Getcode (in, out)
wchar16_t	*in, *out;
{
	register int  c, i, j;
	u_char        s[5];
        int             base, c_code;
        u_short         code;
	wchar16_t       sw[5];
	wchar16_t       tmp[3];

	*out = (wchar16_t) '\0';
        i = 0;
        if (((c_code = GetCcode()) == CODE_KUTEN) || (c_code == CODE_KUTEN2))
                base = 10;
        else
                base = 16;
        while ((c = *in) != '\0') {
		if (WcIsKANJI(c)) {
			in ++;
			*(out ++) = c;
			i = 0;
			continue;
		} else if (IsHex(c))
		        s[i++] = WcLowByte(c);
		else
		        break;
		in ++;
		if (i == 4) {
			s[i] = '\0';
                       code = (u_short)strtol((char *)s, (char **)NULL, base);
                       switch (c_code) {
			     case CODE_KUTEN :
			     case CODE_KUTEN2 :
                                c = ((code / 100) << 8) + code % 100;
                                code = c + 0x2020;
			     case CODE_JIS :
			     case CODE_JIS2 :
                                code = jis2euc(code);
                                break;
			     case CODE_SJIS :
                                code = sjis2euc(code);
                                break;
		       }  
	               if ((c_code == CODE_KUTEN2) || 
			   (c_code == CODE_JIS2) || (c_code == CODE_EUC2))
                             code &= WcX0212MASK;
                       if (!WcIsZENKAKU(code))
                             break;
                       *out++ = code;
			i = 0;
		}
	}
	*out = (wchar16_t) '\0';
	if (i) {
		for (j = 0; j < i; j++)
		        sw[j] = (wchar16_t) s[j];
		sw[i] = (wchar16_t) '\0';
		i = sj_hantozen(out, sw, i);
		out += i;
	}
	if (*in != '\0')
/*
 * Remove warning.
 * Patched by Hidekazu Kuroki(hidekazu@cs.titech.ac.jp)		1996/8/10
 */
		(void)sj_hantozen(out, in, strlen((char *)in));
}			


static u_char	load_mode;	

load_bun (num, s, type)
int	num;
wchar16_t	*s;
u_char	type;
{
	register Conversion	*cv;
	register int	len, ilen;
	u_char		itype;
	wchar16_t Htmp[BUFFLENGTH], Ztmp[BUFFLENGTH * 2];

	cv = GetConversion ();
	if (num < 0 || (len = cv->Plen[num]) == 0) {
		*s = '\0';
		return(0);
	}
	if (type == 0)
		type = MODE_ZHIRA;
	if (cv->Bpoint[num] < cv->Bpoint[0])
		itype = MODE_ALPHA;
	else
		itype = cv->kettei[cv->Bpoint[num] - 1];

	if (type & MODE_ZEN)
		load_mode = 1;
	else
		load_mode = 0;
	if (type == itype) {
		Strncpy(s, cv->Ppoint[num], len);
		s += len;
		*s = '\0';
	} else {		

		if (type == MODE_CODE && itype != MODE_CODE) {
			ilen = getistr(Htmp, cv->Ppoint[num], len);
			Getcode(Htmp, s);
			len = wslen(s);
		} else if ((type & MODE_CONV) && (itype == MODE_ALPHA ||
						  itype == MODE_ZALPHA)) {
			ilen = sj_zentohan(Htmp, cv->Ppoint[num], len);
			ilen = sj_rkconv(Ztmp, Htmp, ilen);
			if (type & MODE_ZEN) {
				len = sj_hantozen(s, Ztmp, ilen);
				if (type == MODE_ZKATA) {
					wscpy(Ztmp, s);
					sj_htok(s, Ztmp);
				}
			} else 
				len = sj_zentohan(s, Ztmp, ilen);

		} else if (!(type & MODE_CONV) && (itype == MODE_CODE ||
						   itype & MODE_CONV)) {
			if (type & MODE_ZEN) {
				ilen = getistr(Htmp, cv->Ppoint[num], len);
				len = sj_hantozen(s, Htmp, ilen);
			} else {
				len = getistr(s, cv->Ppoint[num], len);
			}

		} else {
			if (type & MODE_ZEN) {
				len = sj_hantozen(Ztmp, cv->Ppoint[num], len);
				if (type == MODE_ZKATA)
					sj_htok(s, Ztmp);
				else
					sj_ktoh(s, Ztmp);
			} else 
				len = sj_zentohan(s, cv->Ppoint[num], len);
		}

	}
	return(len);
}

load_bun2(num, htmp, ztmp, omode)
int	num;
wchar16_t	*htmp, *ztmp;
u_short *omode;
{
	register Conversion	*cv;
	register int	i;
	register int	len, ilen;
	u_short		*op;
	u_char		type;
	wchar16_t		Ztmp[DLEN + 1];


	cv = GetConversion ();
	if (num < 0 || (len = cv->Plen[num]) == 0) {
		*ztmp = '\0';
		return(0);
	}

	if (cv->Bpoint[num] < cv->Bpoint[0])
		i = cv->span_point;
	else
		i = cv->Bpoint[num];
	type = cv->kettei[i - 1];
	if (type == MODE_ZHIRA || type == MODE_ZKATA) {
		if (len > DLEN)
			return(len);
		if (type == MODE_ZHIRA) {
			Strncpy(ztmp, cv->Ppoint[num], len);
			ztmp += len;
			*ztmp = '\0';
		} else {
			Strncpy(Ztmp, cv->Ppoint[num], len);
			Ztmp[len] = '\0';
                        sj_ktoh(ztmp, Ztmp);
		}
		(void)load_bun(num, htmp, MODE_ALPHA);
		op = &cv->out_mode[cv->Ppoint[num] - cv->Ppoint[0]];
		for (i = 0; i < len; i++)
			*omode++ = *op++;
	} else {
		ilen = load_bun(num, htmp, MODE_ALPHA);
		len = sj_rkconv2(ztmp, htmp, omode, ilen);
	}
	return(len);
}

getistr(s, ostr, len)
wchar16_t *s, *ostr;
int len;
{
	register int ipos, opos;
	register Conversion *cv;
	int rlen;

	cv = GetConversion();
	opos = ostr - cv->out_buf;
	if (!(cv->out_mode[opos] & OMODE_MASK))
		chhstr(cv->Halpha, cv->out_buf, cv->out_mode, opos);
	ipos = getipos(opos);
	if (*(ostr + len) != '\0' && !(cv->out_mode[opos + len] & OMODE_MASK))
		chhstr(cv->Halpha, cv->out_buf, cv->out_mode, opos + len);
	rlen = getipos(opos + len) - ipos;
	Strncpy(s, &cv->Halpha[ipos], rlen);
	s += rlen;
	*s = (wchar16_t) '\0';
	return(rlen);
}

exec_muhenkan (first)
int	first;
{
	register Conversion	*cv;

	cv = GetConversion ();
	cv->Bnum = 1;
	cv->Ppoint[0] = &(cv->out_buf[0]);
	cv->Plen[0] = cv->out_point;
	cv->OrgPlen[0] = cv->Plen[0];
	cv->Pedited[0] = 0;
	if (first) {
		cv->Bkettei[0] = Bmuhenkan;
		cv->Pkettei[0] = cv->Imode;
	}
	else if (cv->Bkettei[0] != Bmuhenkan) {
		cv->Bkettei[0] = Bmuhenkan;
		cv->Pkettei[0] = MODE_ZHIRA;
	}
	else if (cv->Pkettei[0] != MODE_ZHIRA) {
		cv->Pkettei[0] = MODE_ZHIRA;
	}
	else {
		cv->Pkettei[0] = MODE_ZKATA;
	}

	if (flush_conversion)
		cv->PreBun = cv->CurBun = -1;
	else
		cv->PreBun = cv->CurBun = 0;
	cv->Dflag = -1;	
	cv->Enum = 0;
	cv->e_point = 0;
	cv->Kanji = 1;

	Bdisp (1);
}



static u_char	Bstr_mode;	

Bstr (bnum, s)
int	bnum;
wchar16_t	*s;
{
	register Conversion	*cv;
	register int	i, len;

	cv = GetConversion ();
	if (cv->Bkettei[bnum] & Bmuhenkan) {
		len = load_bun (bnum, s, cv->Pkettei[bnum]);
		Bstr_mode = load_mode;
	}
	else if (cv->Bkettei[bnum] & Bedit) {
		i = cv->Edit[bnum];
		len = cv->Elen[i];
		if (len <= DLEN) {
			if (cv->Epoint[i] != NULL)  {
				Strncpy(s, cv->Epoint[i], len);
				s += len;
				*s = '\0';
			} else
				len = -1;
		}
		else
			len = load_bun (bnum, s, 0);
		Bstr_mode = 1;
	}
	else if (cv->Bkettei[bnum] & Bhenkan) {
	    if (!(i = cv->Bpoint[bnum])) {
                    len = -1;
                    return len;
	    }
		if (cv->Bun[i - 1].deststr != NULL) {
			len = cv->Bun[i - 1].destlen;
			Strncpy(s, cv->Bun[i - 1].deststr, len);
			s += len;
			*s = '\0';
		} else if (cv->Bun[i - 1].srcstr != NULL) {
			len = cv->Bun[i - 1].srclen;
			Strncpy(s, cv->Bun[i - 1].srcstr, len);
			s += len;
			*s = '\0';
		} else
			len = -1;
		Bstr_mode = 1;
	}
	else
		len = -1;
	return (len);
}



mode_muhenkan (mod, bkettei)
register int	mod, bkettei;
{
	register int	def;

	if (!(bkettei & Bmuhenkan))
		return (KEY_ZHIRA);
	def = value_muhenkan ();
	if (mod == MODE_ZHIRA)
		return (KEY_ZKATA);
	else if (mod == MODE_ZKATA) {
		if (def != KEY_ZHIRA && def != KEY_ZKATA)
			return (def);
	}
	return (KEY_ZHIRA);
}

Bchange (mod)
register int	mod;
{
	register Conversion	*cv;
	register int	i;
	int		PushKettei;

	cv = GetConversion ();
	if (cv->CurBun == -1)
		cv->CurBun = 0;
	i = cv->PreBun = cv->CurBun;

	if (mod == KEY_MUHENKAN)
		mod = mode_muhenkan (cv->Pkettei[i], cv->Bkettei[i]);
	else if (mod == KEY_TOGGLE)
		mod = toggle_mode (cv->Pkettei[i]);

	PushKettei = cv->Bkettei[i];
	if (!(cv->Bkettei[i] & Bmuhenkan)) {
		cv->Bkettei[i] |= Bmuhenkan;
		cv->Bkettei[i] &= ~Bhenkan;
	}
	switch (mod) {
	case KEY_CODE:
		cv->Pkettei[i] = MODE_CODE;
		Rdisp ();
		break;
	case KEY_ZHIRA:
		cv->Pkettei[i] = MODE_ZHIRA;
		Rdisp ();
		break;
	case KEY_HALPHA:
		cv->Pkettei[i] = MODE_HALPHA;
		Rdisp ();
		break;
	case KEY_ZALPHA:
		cv->Pkettei[i] = MODE_ZALPHA;
		Rdisp ();
		break;
	case KEY_HKATA:
		cv->Pkettei[i] = MODE_HKATA;
		Rdisp ();
		break;
	case KEY_ZKATA:
		cv->Pkettei[i] = MODE_ZKATA;
		Rdisp ();
		break;
	default:
		cv->Bkettei[i] = PushKettei;
		beep ();
		break;
	}
}



Bdisp (redraw)
{
	register Conversion	*cv;
	register int	i;
	int		len;
	wchar16_t		BDtmp[BUFFLENGTH * 2];
	int             displen;
        int             amount = 0;
        int             tmpcol=0;
        u_short         row, col;

        CursorRead(&row, &col);

	cv = GetConversion ();
	Cclear (redraw);
	SetVcol (0);
	for (i = 0 ; i < cv->Bnum ; i ++) {
		if ((len = Bstr (i, BDtmp)) < 0)
			continue;
                amount += len;
	  if (amount > BUFFLENGTH * 2) {
                    cv->Bnum = i + 1;
                    break;
	  }
		displen = wcbyte(BDtmp);
	        tmpcol += displen;
                if ((tmpcol > (unsigned int)cv->column) && (row == cv->line)
		    && (row == cv->SavedRow) && (row > 0)) {
                    cv->SavedRow--;
                    tmpcol -= cv->column;
		    }
		cv->Displen[i] = displen;
		cv->Dispmod[i] = Bstr_mode;

		if (len <= 0)
			continue;
		if (i == cv->CurBun)
			vprintR (BDtmp, Bstr_mode);
		else
			vprintU (BDtmp, Bstr_mode);
	}
	Flush ();
}


void
Rdisp ()
{
	register Conversion	*cv;
	register int	i;
	register int	Total;
	int		len;
	wchar16_t		BDtmp[BUFFLENGTH * 2];
	int             displen;
        int             amount=0;  
        int             tmpcol=0;
        u_short         row, col;

        CursorRead(&row, &col);

	cv = GetConversion ();
	Total = 0;
	for (i = 0 ; i < cv->Bnum ; i ++) {
		if ((len = Bstr (i, BDtmp)) < 0)
			continue;
                amount += len;
	  if (amount > BUFFLENGTH * 2) {
                    cv->Bnum = i + 1;
                    break;
	  }
		displen = wcbyte(BDtmp);
                tmpcol += displen;
                if ((tmpcol > (unsigned int)cv->column) && (row == cv->line)
		    && (row == cv->SavedRow) && (row > 0)) {
                    cv->SavedRow--;
                    tmpcol -= cv->column;
		    }
		if (cv->Displen[i] != displen || cv->Dispmod[i] != Bstr_mode) {
			AfterAllPrint (i, Total);
			return;
		}
		if (len <= 0)
			continue;
		if (i == cv->CurBun || i == cv->PreBun) {
			if (i == cv->CurBun)
				IprintR (BDtmp, Total);
			else
				IprintU (BDtmp, Total);
		}
		Total += displen;
	}
	Cgoto (Total);
	Flush ();
}

AfterAllPrint (from, Total)
int	from;
register int	Total;
{
	register Conversion	*cv;
	register int	i;
	int		len;
	wchar16_t		BDtmp[BUFFLENGTH * 2];
	int             displen;
        int             amount=0;  
        int             tmpcol=0;
        u_short         row, col;

        CursorRead(&row, &col);

	cv = GetConversion ();
	Cgoto (Total);
	for (i = from ; i < cv->Bnum ; i ++) {
		if ((len = Bstr (i, BDtmp)) < 0)
			continue;
                amount += len;
	  if (amount > BUFFLENGTH * 2) {
                    cv->Bnum = i + 1;
                    break;
	  }
		displen = wcbyte(BDtmp);
                tmpcol += displen;
                if ((tmpcol > (unsigned int)cv->column) && (row == cv->line)
		    && (row == cv->SavedRow) && (row > 0)) {
                    cv->SavedRow--;
                    tmpcol -= cv->column;
		}
	        cv->Displen[i] = displen;
		cv->Dispmod[i] = Bstr_mode;

		if (len <= 0)
			continue;
		if (i == cv->CurBun)
			vprintR (BDtmp, Bstr_mode);
		else
			vprintU (BDtmp, Bstr_mode);
		Total += displen;
	}
	ClearToMax ();
	Cgoto (Total);
	Flush ();
}


void
Bright ()
{
	register Conversion	*cv;
	int	flag;

	cv = GetConversion ();
	flag = 0;
	cv->PreBun = cv->CurBun;
	do {
		cv->CurBun ++;
		if (cv->CurBun >= cv->Bnum) {
			if (flag)
				return;
			else
				flag = 1;
			cv->CurBun = 0;
		}
	} while (cv->Plen[cv->CurBun] <= 0);
	Rdisp ();
}


void
Bleft ()
{
	register Conversion	*cv;
	int	flag;

	cv = GetConversion ();
	flag = 0;
	cv->PreBun = cv->CurBun;
	do {
		cv->CurBun --;
		if (cv->CurBun < 0) {
			if (flag)
				return;
			else
				flag = 1;
			cv->CurBun = cv->Bnum - 1;
		}
	} while (cv->Plen[cv->CurBun] <= 0);
	Rdisp ();
}


void
Blast ()
{
	register Conversion	*cv;

	cv = GetConversion ();
	cv->PreBun = cv->CurBun;
	cv->CurBun = cv->Bnum - 1;
	while (cv->Plen[cv->CurBun] <= 0) {
		cv->CurBun--;
		if (cv->CurBun < 0)
			return;
	}
	Rdisp();
	
}


void
Btop ()
{
	register Conversion	*cv;

	cv = GetConversion ();
	cv->PreBun = cv->CurBun;
	cv->CurBun = 0;
	while (cv->Plen[cv->CurBun] <= 0) {
		cv->CurBun++;
		if (cv->CurBun >= cv->Bnum)
			return;
	}
	Rdisp();
}


