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
 * $SonyRCSfile: douon.c,v $  
 * $SonyRevision: 1.2 $ 
 * $SonyDate: 1996/02/23 11:04:02 $
 */



#include "common.h"
#include "key.h"
#include "wchar16.h"
#define DNUM		5		

static int		Dcount;
static int		Dpoint;
static struct douon_sj3	HenkanDouon[DOUON_N];
static wchar16_t		Dkanji[BUFFLENGTH * 2];

void go_gakusyuu();

SJ_getdouon (s, d, n, choice, reconv)
wchar16_t		*s;
struct douon_sj3	*d;
int		n;
int		choice;
int		reconv;
{
	int	len;

        Dcount = SJ2_getdouon (s, d, n, choice, reconv);

        
        if (Dcount < 0 && !dconnect(1))
                Dcount = SJ2_getdouon (s, d, n, choice, reconv);

	if (Dcount <= 0) {
		Dcount = 1;
		len = wslen (s);
		Strncpy ((char *)d[0].ddata, s, len);
		d[0].dlen = wcbyte(s);
		d[0].wlen = len;
		return(0);		
	} else
		return(1);		
}



exec_douon ()
{
	Conversion	*cv;
	int	val;

	cv = GetConversion ();
	if (cv->CurBun == -1) {
		cv->CurBun = 0;
		Rdisp ();
	}
	cv->PreBun = cv->CurBun;
	if (cv->Bkettei[cv->CurBun] & Bmuhenkan)
		val = Sdouon (2);
	else
		val = Sdouon (1);		
	if (!val)
		Rdisp ();
}

wrap_douon (back)
int	back;	
{
	Conversion	*cv;
	int	val;

	cv = current_conversion;
	if (cv->CurBun == -1) {
		cv->CurBun = 0;
		Rdisp ();
	}
	cv->PreBun = cv->CurBun;
	if (cv->Dflag == cv->CurBun)
		val = Rdouon (back);
	else {
		val = Sdouon (2);
		cv->Dflag = cv->CurBun;
	}
	if (!val)
		Rdisp ();
}



Cdouon (yomi, gnum)
wchar16_t	*yomi;
int	gnum;
{
	

	int	i, j;
	int	len;
	wchar16_t	*s;
	int	dispnum;
	int		num;
	int		displen;
	int		base;
	char		tmp[BUFFLENGTH], tmp2[BUFFLENGTH * 2];
	wchar16_t		wtmp[BUFFLENGTH], wtmp2[BUFFLENGTH * 2];

	int		gakusyuu;
	u_short		row, col;
	Conversion	*cv;

	cv = current_conversion;

	gakusyuu = SJ_getdouon (yomi, HenkanDouon, gnum, 0, 0);

	dispnum = DNUM;
	if (dispnum > Dcount)
		j = Dcount;
	else
		j = dispnum;
	len = 0;
	for (i = 0 ; i < j ; i ++)
		len += HenkanDouon[i].dlen;
	len /= j;
	if (len < 1)
		len = 1;
	len += 6;
	sprintf (tmp, "%d/%d ", Dcount, Dcount);
	displen = cv->column - strlen (tmp) - 1;
	if (displen < 1)
		displen = 1;
	num = displen / len;
	if (num < 1)
		num = 1;
	if (num < dispnum && num > 0)
		dispnum = num;

	for (i = 0 ; ; ) {
		if (i >= Dcount)
			i = 0;
		StartGuide (&row, &col);
		Clear_EOL ();
		base = i;

		sprintf (tmp, "%d/%d", i + 1, Dcount);
		(void) mbstowcs(wtmp, tmp, BUFFLENGTH);
		printR (wtmp);
		put_space (1);

		for (j = 0 ; j < dispnum ; j ++, i ++) {
			if (i >= Dcount)
				break;
			len = HenkanDouon[i].dlen;

			if (len > displen)
				len = displen;

			s = (wchar16_t *)HenkanDouon[i].ddata;
			Strncpy (wtmp, s, HenkanDouon[i].wlen);
			wtmp[HenkanDouon[i].wlen] = '\0';
                        (void) wcstombs(tmp, wtmp, BUFFLENGTH);
			sprintf (tmp2, "[%d] %s ", j+1, tmp);
                        (void) mbstowcs(wtmp2, tmp2, BUFFLENGTH);
			SJ_print (wtmp2);
		}

		EndGuide (row, col);
		Flush ();
		num = Dselect (j);	

		
		if (num == 0)
			continue;

		
		else if (num == -1) {
			if ((i >= Dcount) && ((Dcount % dispnum) != 0))
				i = Dcount - (Dcount % dispnum);
			else
				i -= dispnum;
			i -= dispnum;
			if (i < 0) {
				if (Dcount > dispnum) {
					if ((Dcount % dispnum) == 0)
						i = Dcount - dispnum;
					else
						i = Dcount - (Dcount % dispnum);
				}
				else
					i = 0;
			}
			continue;
		}

		
		else if (num > j) {
			disp_mode ();
			return (FALSE);
		}

		
		else
			break;
	}

	disp_mode ();

	Dpoint = base + num - 1;
	len = HenkanDouon[Dpoint].dlen;
	s = (wchar16_t *)HenkanDouon[Dpoint].ddata;
	Strncpy (Dkanji, s, HenkanDouon[Dpoint].wlen);
	Dkanji[HenkanDouon[Dpoint].wlen] = '\0';
 	if (HenkanDouon[Dpoint].valid) 
 		PushGD (gnum, &HenkanDouon[Dpoint].dcid);

	if (gakusyuu)
 		if (HenkanDouon[Dpoint].valid)
 			SJ2_study (&HenkanDouon[Dpoint].dcid);
 		else
 			SJ2_toroku(yomi, HenkanDouon[Dpoint].ddata, SJ3_H_NRMNOUN);

	return (TRUE);
}

Dselect (lim)
int	lim;
{
	extern int	keyvalue;
	int	c, num;
	char		s[2];

	SaveConversion ();
	while ((c = inkey ()) != EOF) {
		if (AnotherConversion ()) {
			unget_key (c);
			break;
		}
		if (keyvalue == KEY_KETTEI)
			break;			
		else if (keyvalue == KEY_HENKAN || keyvalue == KEY_RIGHT)
			return (0);		
		else if (c == ' ')
			return (0);
		else if (keyvalue == KEY_MUHENKAN || keyvalue == KEY_LEFT
			|| is_bsdel (c))
			return (-1);		
		else if (WcIsASCII(c) && isdigit (WcLowByte(c))) {
			*s = c;
			s[1] = '\0';
			num = atoi (s);
			if (num > 0 && num <= lim)
				return (num);	
		}
		else if (keyvalue != KEY_NORMAL)
			break;			
		else if (keyvalue == KEY_NORMAL && IsESC (c))
			break;
	}
	return (lim + 1);			
}


Gdouon (yomi, num, choice, reconv)
wchar16_t	*yomi;
int	num;
int	choice;
int	reconv;
{
	

	int	len;
	wchar16_t	*s;

	SJ_getdouon (yomi, HenkanDouon, num, choice, reconv);

	Dpoint = 0;
	len = HenkanDouon[Dpoint].wlen;
	s = (wchar16_t *)HenkanDouon[Dpoint].ddata;
	Strncpy (Dkanji, s, len);
	Dkanji[len] = '\0';
	if (HenkanDouon[Dpoint].valid) 
		PushGD (num, &HenkanDouon[Dpoint].dcid);
}

Sdouon (Choice)
int Choice;
{
	Conversion	*cv;
	int	i, j;
	int		len;
	wchar16_t		yomi[BUFFLENGTH * 2];
	int		count;

	cv = current_conversion;
	count = 0;
	if (Choice == 0)
		i = cv->PreBun;
	else
		i = cv->CurBun;
top:
	if ((len = load_bun(i, yomi, MODE_ZHIRA)) <= 0) {
		cv->Bkettei[i] |= Bmuhenkan;
		cv->Bkettei[i] &= ~Bhenkan;
		cv->Pkettei[i] = MODE_ZHIRA;
		*Dkanji = '\0';
		goto null;
	}
	else if (len > DLEN) {
		cv->Bkettei[i] |= Bmuhenkan;
		cv->Bkettei[i] &= ~Bhenkan;
		*Dkanji = '\0';
		goto null;
	}
	else if (Choice == 0 && (cv->Bkettei[i] & Bmuhenkan)) {
		goto tail;
	}
	else {
		cv->Bkettei[i] |= Bhenkan;
		cv->Bkettei[i] &= ~Bmuhenkan;
	}

	if (Choice == 1) {
		if (Cdouon (yomi, i) == FALSE)
			return (1);
		cv->Pkettei[i] = MODE_ZHIRA;
	}
	else if (Choice == 2) {
		Gdouon (yomi, i, 0, 0);
	}
	else if (Choice == 3)
		Gdouon (yomi, i, count, 1);	
	else
		Gdouon (yomi, i, count, 0);
null:
	len = wslen (Dkanji);

	if (!(cv->Bkettei[i] & Bedit)) {
		if (cv->Enum >= MAXENUM)
			return (1);
		j = cv->Enum ++;
		cv->Edit[i] = j;
		cv->Epoint[j] = &(cv->Ebuff[cv->e_point]);
		cv->e_point += DLEN;
	}
	else
		j = cv->Edit[i];
	if (0 < len && len <= DLEN) {
		cv->Bkettei[i] |= Bedit;
		cv->Pkettei[i] = MODE_ZHIRA;
	}
	cv->Elen[j] = len;
	wscpy (cv->Epoint[j], Dkanji);

tail:
	if (Choice == 0 && i != cv->CurBun) {
		i = cv->CurBun;
		goto top;
	}
	if (cv->PreBun == cv->Bnum)
		cv->Bkettei[cv->Bnum] = Bend;
	return (0);
}



Rdouon (back)
int	back;	
{
	Conversion	*cv;
	int	i, j;
	int	len;
	wchar16_t	*s;

	cv = current_conversion;
	if (Dcount > 0) {
		if (back) {
			Dpoint --;
			if (Dpoint < 0)
				Dpoint = Dcount - 1;
		}
		else {
			Dpoint ++;
			if (Dpoint >= Dcount)
				Dpoint = 0;
		}
		len = HenkanDouon[Dpoint].wlen;
		s = (wchar16_t *)HenkanDouon[Dpoint].ddata;
		Strncpy (Dkanji, s, len);
		Dkanji[len] = '\0';

		i = cv->CurBun;
		j = cv->Edit[i];
		cv->Elen[j] = len;
		wscpy (cv->Epoint[j], Dkanji);
		return (0);
	}
	return (1);
}

Bkanji (kanji)
wchar16_t	*kanji;
{
	Conversion	*cv;
	int	i;
	int		plen, len;
	wchar16_t		*s;

	cv = current_conversion;
	s = kanji;
	*s = '\0';
	plen = 0;
	for (i = 0 ; i < cv->Bnum ; i ++) {
		if ((len = Bstr (i, s)) <= 0)
			continue;
		s += len;
		plen += len;
	}
	return(plen);
}

struct studyrec	Gdata[BUFFLENGTH];

PushGD (num, wordid)
int		num;
struct studyrec	*wordid;
{
	Gdata[num] = *wordid;
}

cl_gakusyuu ()
{
	Conversion	*cv;
	int	i, j;
	int	o_point, n_point;

	cv = current_conversion;
	for (i = 0 ; i < cv->Bnum ; i ++) {
		if (cv->OrgPlen[i] != cv->Plen[i]) {
			o_point = n_point = 0;
			j = i;
			while (j < cv->Bnum) {
				o_point += cv->OrgPlen[j];
				n_point += cv->Plen[j];
				j ++;
				if (o_point == n_point)
					break;
			}
			j--;
			check_and_gakusyuu (i, j);
			i = j;
		}
	}
}

check_and_gakusyuu (m, n)
int	m, n;
{
	Conversion	*cv;
	int	i, j;

	cv = current_conversion;
	i = m;
	j = i + 1;
	while (j <= n && j < cv->Bnum) {
		if (cv->Bkettei[i] & Bedit && !(cv->Bkettei[i] & Bmuhenkan))
			go_gakusyuu (i);
		i ++;
		j ++;
	}
}

void
go_gakusyuu (n)
int	n;
{
	Conversion	*cv;
	int	i, j;
	wchar16_t		yomi1[BUFFLENGTH], yomi2[BUFFLENGTH];
	struct studyrec	*gdatap;
	int		len;

	cv = current_conversion;
	i = n;
	j = i + 1;

	if (cv->Pedited[i] == 1 || cv->Pedited[j] == 1)
		return;

	
	if ((len = load_bun(i, yomi1, MODE_ZHIRA)) <= 0 && len > DLEN)
		return;
	if ((len = load_bun(j, yomi2, MODE_ZHIRA)) > DLEN)
		return;

	
	if (len <= 0)
		gdatap = NULL;
	else
		gdatap = &Gdata[j];
	SJ2_clstudy (yomi1, yomi2, gdatap);
}
