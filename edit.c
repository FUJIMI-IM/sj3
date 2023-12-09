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
 * $SonyRCSfile: edit.c,v $  
 * $SonyRevision: 1.2 $ 
 * $SonyDate: 1997/01/23 11:09:36 $
 */



#include "common.h"
#include "key.h"

void Chain();

static wchar16_t	*jpoint;
static int	iedit, jedit;
static int	ilen, jlen;

push_obj (i, j)
int	i, j;
{
	Conversion	*cv;

	cv = GetConversion ();
	jpoint = cv->Ppoint[j];
	ilen = cv->Plen[i];
	jlen = cv->Plen[j];
	iedit = cv->Bkettei[i];
	jedit = cv->Bkettei[j];
}

pop_obj (i, j)
int	i, j;
{
	Conversion	*cv;

	cv = GetConversion ();
	cv->Ppoint[j] = jpoint;
	cv->Plen[i] = ilen;
	cv->Plen[j] = jlen;
	cv->Bkettei[i] = iedit;
	cv->Bkettei[j] = jedit;
}



Bbig ()
{
	Conversion	*cv;
	int		i, j, k;
	int			moji, last, val;

	cv = GetConversion ();
	if (cv->CurBun < 0)
		cv->CurBun = 0;
	i = cv->CurBun;
	j = i + 1;
	while (j < cv->Bnum) {
		if (cv->Plen[j] > 0) {
			push_obj (i, j);

			k = cv->Ppoint[j] - cv->Ppoint[0];
			last = k + cv->Plen[j];
			moji = 0;
			for (; k < last; k++) {
				if (k < last-1 &&(cv->out_mode[k+1]&OMODE_ZEN)){
					moji += 2;
					k++;
				}else 
					moji++;
				if (cv->out_mode[k+1] & OMODE_MASK)
					break;
			}
			cv->Plen[i] += moji;
			cv->Plen[j] -= moji;
			cv->Ppoint[j] = cv->Ppoint[i] + cv->Plen[i];

			cv->PreBun = j;

			if (cv->Bkettei[i] & Bhenkan) {
				val = Sdouon (0);
				
			}
			else {
				cv->Bkettei[i] |= Bmuhenkan;
				cv->Bkettei[i] &= ~Bhenkan;
				cv->Bkettei[j] |= Bmuhenkan;
				cv->Bkettei[j] &= ~Bhenkan;
				val = 0;
			}
			if (!val) {
				cv->Dflag = -1;
				Rdisp ();
				return (1);
			}
			break;
		}
		else
			j ++;
	}
	beep ();
	if (j < cv->Bnum)
		pop_obj (i, j);
	return (0);
}



Bsmall (small)
int	small;		
{
	Conversion	*cv;
	int	i, j, k, moji;
	int		val;

	cv = GetConversion ();
	if (cv->CurBun < 0)
		cv->CurBun = 0;
	i = cv->CurBun;
	j = i + 1;
	if (j <= cv->Bnum && cv->Plen[i] > 1) {
		if (j == cv->Bnum) {
			cv->Bnum ++;
			cv->Bkettei[cv->Bnum] = Bend;
			cv->Plen[j] = 0;
			cv->Pkettei[j] = cv->Pkettei[i];
			cv->Bkettei[j] |= Bmuhenkan;
			cv->Bkettei[j] &= ~Bhenkan;
		}
		else if (small) {
			while (j < cv->Bnum - 1 && cv->Plen[j] == 0)
				j ++;
			if (j == cv->Bnum)
				j --;
		}
		else if (cv->Plen[j] != 0 && cv->Enum < MAXENUM) {
			for (k = cv->Bnum ; k > j ; k --) {
				cv->Plen[k] = cv->Plen[k - 1];
				cv->Ppoint[k] = cv->Ppoint[k - 1];
				cv->Pkettei[k] = cv->Pkettei[k - 1];

				cv->Bpoint[k] = cv->Bpoint[k - 1];
				cv->Bkettei[k] = cv->Bkettei[k - 1];

				cv->Displen[k] = cv->Displen[k - 1];
				cv->Dispmod[k] = cv->Dispmod[k - 1];

				cv->Edit[k] = cv->Edit[k - 1];
			}
			cv->Bnum ++;
			cv->Plen[j] = 0;
			cv->Bkettei[j] = Bmuhenkan;
			cv->Displen[j] = 0;
		}
		else if (cv->Plen[j] != 0) {
			while (j < cv->Bnum - 1 && cv->Plen[j] == 0)
				j ++;
			if (j == cv->Bnum)
				j --;
		}
		else {
			while (j < cv->Bnum - 1 && cv->Plen[j] == 0 &&
						   cv->Plen[j+1] == 0)
				j ++;
		}

		push_obj (i, j);

		k = cv->Ppoint[i] - cv->Ppoint[0] + cv->Plen[i] - 1;
		if (cv->out_mode[k] & OMODE_ZEN)
				moji = 2;
		else 
				moji = 1;
		cv->Plen[i] -= moji;
		cv->Plen[j] += moji;
		cv->Ppoint[j] = cv->Ppoint[i] + cv->Plen[i];

		if (cv->Plen[i] < 1) {
			beep ();
			pop_obj (i, j);
			return(0);
		}

		cv->PreBun = j;

		if (cv->Bkettei[i] & Bhenkan) {
			val = Sdouon (0);
			
		}
		else {
			cv->Bkettei[i] |= Bmuhenkan;
			cv->Bkettei[i] &= ~Bhenkan;
			cv->Bkettei[j] |= Bmuhenkan;
			cv->Bkettei[j] &= ~Bhenkan;
			val = 0;
		}
		if (!val) {
			cv->Dflag = -1;
			Rdisp ();
			return (1);
		}
		beep ();
		pop_obj (i, j);
	}
	else {
		beep ();
	}
	return (0);
}


void
Bdelete ()
{
	Conversion	*cv;
	int	i, j, deletelen, val;

	cv = GetConversion ();
	if (cv->CurBun < 0) {
		cv->CurBun = 0;
		return;
	}
	i = cv->CurBun;
	j = i + 1;
	if (j <= cv->Bnum && cv->Plen[i] > 0) {
		while (j < cv->Bnum && cv->Plen[j] == 0)
			j ++;
		if (j == cv->Bnum) {
			j = i - 1;
			while (j >= 0 && cv->Plen[j] == 0)
				j --;
			if (j == -1)
				j = i;
		}

		push_obj (i, j);

		deletelen = cv->Plen[i];
		cv->Plen[i] = 0;

		cv->PreBun = i;
		cv->CurBun = j;
			
		val = Sdouon (0);
		if (!val) {
			cv->Dflag = -1;
			Chain (i, deletelen);
			Rdisp ();
			if (i == j)
				all_set_up ();
		} else {
			beep ();
			pop_obj (i, j);
		}
	}
	else {
		beep ();
		all_set_up ();
	}
}


void
Chain (n, len)
int	n;
int	len;
{
	Conversion	*cv;
	int		i;
	wchar16_t		*p;
	u_short	*pp;
	int 			mlen, plen, ipos, rlen, imlen;

	cv = GetConversion ();
	if (cv->Ppoint[n+1] == 0)
		return;
/*
 * Remove warning.
 * Patched by Hidekazu Kuroki(hidekazu@cs.titech.ac.jp)		1996/8/10
 */
	mlen = strlen ((char *)cv->Ppoint[n+1]);
	plen = cv->Ppoint[n] - cv->Ppoint[0];
	p = cv->Ppoint[n];
	pp = &cv->out_mode[plen];
	ipos = getipos(plen);
	if (cv->Ppoint[n+1] > cv->Ppoint[0])
		rlen = getipos(cv->Ppoint[n+1] - cv->Ppoint[0]) - ipos;
	else
		rlen = 0;
	imlen = 0;
	for (i = 0 ; i < mlen ; i ++) {
		p[i] = p[i+len];
		pp[i] = pp[i+len];
		imlen += pp[i] & OMODE_MASK;
	}
	p[mlen] = '\0';
	cv->out_point -= len;

	p = &cv->Halpha[ipos];
	for (i = 0; i < imlen; i++)
		p[i] = p[i+rlen];
	p[imlen] = '\0';
	cv->ha_point -= rlen;

	for (i = n + 1 ; i < cv->Bnum ; i ++)
		cv->Ppoint[i] -= len;

}

static int Elimit;
static int Ehlimit;
exec_edit ()
{
	Conversion	*cv;
	int		edit, inc, i;
	wchar16_t	*s;
	wchar16_t			Htmp[BUFFLENGTH], Ztmp[BUFFLENGTH * 2];
	u_short			Omode[BUFFLENGTH * 2];
	int			bun;
	int			edited, editlen;
	int			len, rlen, hlen, diff;
	int			Zcol, cur, icur, res, ncur;
	extern int		keyvalue;
	wchar16_t			number[3];
	wchar16_t			tmp[3];
	int			num, numpoint;
	extern char	*Gedit;

	cv = GetConversion ();
	bun = cv->CurBun;
	len = load_bun2(bun, Htmp, Ztmp, Omode);
	rlen = wslen(Htmp);
	if (len > DLEN) {
		guide_print_pause(WCGedit, WCTooLong);
		return(0);
	}
	TopGuide ();
	spaceR (WCGedit);
	SJ_print (Ztmp);
	Clear_EOL ();
	backspace (wcbyte (Ztmp));
	Zcol = 0;
	Flush ();

	
	cur = cv->Ppoint[bun] - cv->Ppoint[0];
	ncur = cur + cv->Plen[bun];
	s = cv->Ppoint[bun];
	s += cv->Plen[bun];
	res = wslen(s);
	i = BUFFLENGTH * 2 - res - cur - 1;
	if (i < DLEN)
		Elimit = i;
	else
		Elimit = DLEN;
	i = BUFFLENGTH - wslen(cv->Halpha) + rlen - 1;
	if (i < DLEN)
		Ehlimit = i;
	else
		Ehlimit = DLEN;
	edited = 0;
	edit = 1;
	numpoint = 0;
	SaveConversion ();
	while (edit) {
		inc = inkey ();
		cv = GetConversion ();
		if (AnotherConversion ()) {
			unget_key (inc);
			edited = 0;
			break;
		}
		if (keyvalue == KEY_CONV)
			return (0);
		if (keyvalue == KEY_HENKAN || keyvalue == KEY_KETTEI)
			break;
		else if (keyvalue == KEY_UP)
			inc = 'a';
		else if (keyvalue == KEY_DOWN)
			inc = 'i';
		else if (keyvalue == KEY_RIGHT)
			inc = 'l';
		else if (keyvalue == KEY_LEFT)
			inc = 'h';
		switch (inc) {
		case EOF:
		case ESC:
			edit = 0;
			break;
		case 'l':
		case ' ':
			num = getnum (number, &numpoint);
			if (Zcol >= len - 1) {
				beep ();
				break;
			}
			for (i = 0 ; i < num  && Zcol < len - 1 ; i++) {
				tmp[0] = Ztmp[Zcol];
				tmp[1] = 0;
				SJ_print(tmp);
				Zcol ++;
			}
			Flush ();
			break;
		case 'h':
		case BS:
			num = getnum (number, &numpoint);
			if (Zcol <= 0) {
				beep ();
				break;
			}
			for (i = 0 ; i < num && Zcol >= 1 ; i ++)
				backspace(2);
				Zcol -= 1;
			Flush ();
			break;
		case '\014':	
		case '$':
			num = getnum (number, &numpoint);
			if (len <= 0)
				break;
			Zcol = len - 1;
			TopGuide ();
			spaceR (WCGedit);
			SJ_print (Ztmp);
			Clear_EOL ();
			backspace (2);
			Flush ();
			break;
		case 'a':
			if (len >= 1) {
				tmp[0] = Ztmp[Zcol];
				tmp[1] = 0;
				SJ_print (tmp);
				Zcol ++;
				Flush ();
			}
			
		case 'i':
			num = getnum (number, &numpoint);
			if (editlen = Insert(Htmp, Ztmp, Omode, Zcol)) {
				len += editlen;
				Zcol += editlen;
				edited = 1;
			}
			if (Zcol >= 1) {
				backspace(2);
				Zcol -= 1;
			}
			Print_EOL (&Ztmp[Zcol]);
			Flush ();
			break;
		case DEL:
		case 'x':
			num = getnum (number, &numpoint);
			if (len <= 0) {
				beep ();
				break;
			}
			len -= EditDel (Htmp, Ztmp, Omode, Zcol, num);
			if (Zcol >= len && Zcol >= 1) {
				backspace(2);
				Zcol -= 1;
			}
			Print_EOL (&Ztmp[Zcol]);
			Flush ();
			edited = 1;
			break;
		case 'r':
			num = getnum (number, &numpoint);
			if (len <= 0) {
				beep ();
				break;
			}
			i = Replace(Htmp, Ztmp, Omode, Zcol, num);
			if (i > 0) {
				i -= 1;
				Zcol += i;
				backspace(2);
				edited = 1;
			}
			Print_EOL (&Ztmp[Zcol]);
			Flush ();
			break;
		case 's':
			num = getnum (number, &numpoint);
			if (len <= 0) {
				beep ();
				break;
			}
			if (editlen = Substitute(Htmp,Ztmp,Omode,&Zcol,num)) {
				len += editlen;
			}
			if (Zcol >= 1) {
				backspace(2);
				Zcol -= 1;
			}
			Print_EOL (&Ztmp[Zcol]);
			Flush ();
			edited = 1;
			break;
		case '0':
			if (numpoint == 0) {
				num = getnum (number, &numpoint);
				TopGuide ();
				spaceR (WCGedit);
				Flush ();
				Zcol = 0;
				break;
			}
			
		default:
			if (iswdigit (inc)) {
				if (numpoint > 1) {
					
					
					number[0] = number[1];
					number[1] = inc;
				}
				else
					number[numpoint ++] = inc;
			}
			else {
				num = getnum (number, &numpoint);
				beep ();
			}
			break;
		}
	}

	if (edited) {
		for (i = 0; i < res; i++) {
			Ztmp[i + len] = *s++;
			Omode[i + len] = cv->out_mode[i + ncur];
		}
		res += len;
		s = cv->Ppoint[bun];
		for (i = 0; i < res; i++) {
			*s++ = Ztmp[i];
			cv->out_mode[i + cur] = Omode[i];
		}
		*s = '\0';
		hlen = wslen(Htmp);
		icur = getipos(cur);
		s = &cv->Halpha[icur];
		s += rlen;
		if (*s)
			wscat (Htmp, s);
		wscpy (&cv->Halpha[icur], Htmp);
		if ((diff = len - cv->Plen[bun]) != 0) {
			cv->Plen[bun] = len;
			cv->out_point += diff;
			cv->out_epoint += diff;
			for (i = bun + 1 ; i < cv->Bnum ; i ++)
				cv->Ppoint[i] += diff;
		}
		if ((diff = hlen - rlen) != 0) {
			cv->ha_point += diff;
			cv->ha_epoint += diff;
		}
                if ((i = cv->Bpoint[bun]) < (unsigned int)cv->Bpoint[0])
                        i = cv->span_point;
                cv->kettei[i - 1] = MODE_ZHIRA;
		cv->Pedited[bun] = 1;	
		cv->Dflag = -1;
		return (1);		
	}
	return (0);			
}

getnum (number, numpoint)
wchar16_t	number[];
int	*numpoint;
{
	char temp[3];
	int i=0;

	if (*numpoint == 0)
		return (1);
	number[*numpoint] = '\0';
	*numpoint = 0;
        do {
                temp[i] = WcLowByte(number[i]);
	} while (number[i++] != 0) ;

	return (atoi (temp));
}

Replace(Hs, Zs, omode, cur, num)
wchar16_t	*Hs, *Zs;
u_short *omode;
int cur, num;
{
	int	i, len, inc;
	int		beep_on, icur1, icur2;
	wchar16_t		roma[SHORTBUFFSIZE+1], kana[SHORTBUFFSIZE+1];
	wchar16_t		work[DLEN+1];
	u_short		otmp[SHORTBUFFSIZE+1];

	*roma = '\0';
	*kana = '\0';
	SaveConversion(GetConversion());
	while (1) {
		inc = inkey();
		if (AnotherConversion(GetConversion())) {
			unget_key(inc);
			*kana = '\0';
			break;
		}
		len = wslen(roma);
		if (inc == ESC) {
			if (len > 0)
				(void)exec_romaji2(0, roma, kana, otmp, 
						   wslen(kana), len);
			break;
		}
		if (iswcntrl(inc) && !(len > 0 && is_bs(inc))) {
			beep();
			*kana = '\0';
			break;
		}
		if (exec_romaji2(inc, roma, kana, otmp, wslen(kana), len))
			break;
	}
	sj3_rkclear();
	if ((len = wslen(kana)) <= 0)
		return(0);
	if (kana[2] != '\0') {
		beep_on = 1;
		*otmp = sj_zentohan(roma, kana, 2);
		kana[2] = '\0';
	} else
		beep_on = 0;
	backspace(wcbyte(kana));
	len = wslen(&Zs[cur]);
	if (num <= 0)
		num = 2;
	if (!(omode[cur] & OMODE_MASK))
		chhstr(Hs, Zs, omode, cur);
	icur1 = getipos2(omode, cur);
	if (len <= num) {
		num = len;
		*work = '\0';
	} else {
		if (!(omode[cur + num] & OMODE_MASK))
			chhstr(Hs, Zs, omode, cur + num);
		icur2 = getipos2(omode, cur + num);
		wscpy(work, &Hs[icur2]);
	}
	Hs[icur1] = '\0';
	for (i = 0; i < num; i += 2) {
		SJ_print(kana);
		Zs[cur] = kana[0];
		omode[cur++] = otmp[0];
		Zs[cur] = kana[1];
		omode[cur++] = otmp[1];
		wscat(Hs, roma);
	}
	Flush();
	wscat(Hs, work);
	if (beep_on)
		beep();
	return(num);
}


Substitute(Hs, Zs, omode, cur, num)
wchar16_t	*Hs, *Zs;
u_short *omode;
int *cur, num;
{
	wchar16_t		work[DLEN + 1];
	wchar16_t		work2[DLEN + 1];
	int	len;

	len = wslen(&Zs[*cur]);
	if (len < num)
		num = len;

	Strncpy (work, &Zs[*cur], num - 1);
	work[num - 1] = '\0';
        wscpy(work2, work);
        wscat(work2, WCDollar);
	SJ_print (work2);
	backspace (wcbyte(&Zs[*cur]));

	Flush ();
	(void)EditDel(Hs, Zs, omode, *cur, num);
	if (len = Insert(Hs, Zs, omode, *cur))
		*cur += len;
	len -= num;
	return (len);
}

Insert(Hs, Zs, omode, cur)
wchar16_t	*Hs, *Zs;
u_short *omode;
int cur;
{
	int	inc, len, i;
	int		limit, hlimit, ylen, res, icur, rlen;
	wchar16_t		ztmp[DLEN + 1], htmp[DLEN + 1];
	u_short		otmp[DLEN + 1];

	ylen = wslen(Zs);
	limit = Elimit - ylen;
/*
 * Remove warning.
 * Patched by Hidekazu Kuroki(hidekazu@cs.titech.ac.jp)		1996/8/10
 */
	hlimit = Ehlimit - strlen((char *)Hs);
	*ztmp = '\0';
	*htmp = '\0';
	SaveConversion (GetConversion ());
	while (1) {
		inc = inkey ();
		if (AnotherConversion (GetConversion ())) {
			unget_key (inc);
			sj3_rkclear();
			return(0);
		}
		if (inc == ESC)
			break;
		if (iswcntrl(inc) && !is_bs(inc))
			beep ();
		else if (((len = wslen(ztmp)) >= limit || 
			 (rlen = wslen(htmp)) >= hlimit) && !is_bs(inc))
			beep();
		else
			(void)exec_romaji2(inc, htmp, ztmp, otmp, len, rlen);
	}
/*
 * Remove warning.
 * Patched by Hidekazu Kuroki(hidekazu@cs.titech.ac.jp)		1996/8/10
 */
	(void)exec_romaji2(0, htmp, ztmp, otmp, strlen((char *)ztmp), strlen((char *)htmp));
	sj3_rkclear();
	len = wslen (ztmp);
	backspace(wcbyte(ztmp));
	SJ_print(ztmp);
	Flush();
	if (len) {
		if (!(omode[cur] & OMODE_MASK))
			chhstr(Hs, Zs, omode, cur);
		icur = getipos2(omode, cur);
		res = ylen - cur;
		for (i = 0; i < res; i++) {
			ztmp[i + len] = Zs[i + cur];
			otmp[i + len] = omode[i + cur];
		}
		res += len;
		for (i = 0; i < res; i++) {
			Zs[i + cur] = ztmp[i];
			omode[i + cur] = otmp[i];
		}
		Zs[i + cur] = '\0';
		wscat(htmp, &Hs[icur]);
		wscpy(&Hs[icur], htmp);
	}
	return (len);
}

EditDel (Hs, Zs, omode, cur, num)
wchar16_t	*Hs, *Zs;
u_short *omode;
int	cur, num;
{
	int	i, j;
	int		icur, len, rlen;

	if (!(omode[cur] & OMODE_MASK))
		chhstr(Hs, Zs, omode, cur);
	len = wslen(&Zs[cur]);
	icur = getipos2(omode, cur);
	if (len < num) {
		Zs[cur] = '\0';
		Hs[icur] = '\0';
	} else {
		if (!(omode[cur + num] & OMODE_MASK))
			chhstr(Hs, Zs, omode, cur + num);
		rlen = getipos2(omode, cur + num) - icur;
		j = len - num + cur;
		for (i = cur; i < j; i++) {
			Zs[i] = Zs[i + num];
			omode[i] = omode[i + num];
		}
		Zs[i] = '\0';
		j = wslen(Hs) - rlen;
		for (i = icur; i < j; i++)
			Hs[i] = Hs[i + rlen];
		Hs[i] = '\0';
		len = num;
	}
	return (len);
}

chhstr(Hs, Zs, omode, cur)
wchar16_t *Hs, *Zs;
u_short *omode;
int cur;
{
	int i, j, len;
	int prep, nextp;
	int hprep, hnextp;
	wchar16_t hkana[BUFFLENGTH], *hkp;

	len = wslen(Zs);
	prep = cur;
	hprep = getipos2(omode, prep);
	nextp = cur + 1;
	hnextp = getipos2(omode, nextp);
	hkp = hkana;
	for(i = prep; i < nextp; i++) {
		if (i < nextp - 1 && (WcIsZENKAKU(Zs[i]))) {
			j = sj_zentohan(hkp, &Zs[i], 1);
			hkp += j;
			omode[i] = j;
		} else {
			*hkp++ = Zs[i];
			omode[i] = 1;
		}
	}
	wscpy(hkp, &Hs[hnextp]);
	wscpy(&Hs[hprep], hkana);
}

Print_EOL (s)
wchar16_t *s;
{
	if (*s != '\0')
		SJ_print (s);
	Clear_EOL ();
	if (*s != '\0')
		backspace (wcbyte (s));
}

backspace (n)
int	n;
{
	int	i;

	for (i = 0 ; i < n ; i ++)
		Backspace ();
}
