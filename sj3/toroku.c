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
 * $SonyRCSfile: toroku.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:29 $
 */



#include "common.h"
#include "key.h"
#include "wchar16.h"
#define YOMILEN	32
#define KLEN	64

extern struct valtbl hin_val[];

extern int	keyvalue;

exec_toroku ()
{
	register Conversion	*cv;
	wchar16_t			yomi[YOMILEN+1], kanji[KLEN+1];
	wchar16_t			*hinshi;
	int			inc;
	int			h_code, retv;
	int			val;

	if (IsDcflag())
		return(0);
	cv = GetConversion ();
	val = getkstr (kanji, KLEN, WCGtoroku);
	if (*kanji == '\0')
		return (val);

	while (1) {
		if (getyomi (yomi, YOMILEN, WCGtoroku, kanji) || *yomi == '\0')
			break;
		h_code = gethinshi();
		hinshi= hin_val[h_code - 1].keyword;
		if (getcheck (yomi, kanji, hinshi, WCGtoroku))
			break;
	}
	if (*yomi == '\0')
		return (val);

	TopGuide (); 
	printR (WCGtoroku);
	Clear_EOL ();
	Flush ();
	cv->word_regist = 1;
	if ((retv = SJ2_toroku(yomi, kanji, hin_val[h_code -1].ivalue)) != 0) {
		switch (retv) {
		case -1:
                        (void)dconnect(0);
			return(val);
		case SJ3_WORD_EXIST:
			SJ_print(WCExist);
			break;
		case SJ3_BAD_YOMI_STR:
			SJ_print(WCYomiTouroku);
			break;
		case SJ3_BAD_KANJI_STR:
			SJ_print(WCKanjiTouroku);
			break;
		case SJ3_DOUON_FULL:
			SJ_print(WCOnajiTouroku);
			break;
		case SJ3_DICT_FULL:
			SJ_print(WCUserTouroku);
			break;
		case SJ3_DICT_LOCKED:
			SJ_print(WCJishoLock);
			break;
		default:
			SJ_print(WCSystemError);
			break;
		}
	} else
		SJ_print (WCTOUROKU_SUCC);
	Clear_EOL ();
	Flush ();

	inc = inkey ();
	if (AnotherConversion ()) {
		unget_key (inc);
	}
	return (val);
}

getyomi (yomi, lim, prompt, kanji)
wchar16_t	*yomi, *prompt, *kanji;
int	lim;
{
	register int	inc, c;
	register Conversion *cv;
	int		len;
	wchar16_t		wtmp[BUFFLENGTH], ytmp[YOMILEN+1], ktmp[KLEN+1];
	u_char		tmp[BUFFLENGTH];

	cv = GetConversion();
 	TopGuide ();  
	printR (prompt);
	len = cv->column - (wcbyte(kanji) + 24);
	if (len >= lim)
        {
		(void) wcstombs((char *)ytmp, WCMyomi, YOMILEN+1);
		(void) wcstombs((char *)ktmp, kanji, KLEN+1);
		sprintf ((char *)tmp, (char *)ytmp, ktmp);
		(void) mbstowcs(wtmp, (char *)tmp, BUFFLENGTH);
	}
	else
       {
	       (void) wcstombs((char *)ytmp, WCMyomi, YOMILEN+1);
	       (void) sprintf ((char *)tmp, (char *)ytmp);
               (void) mbstowcs(wtmp, (char *)tmp, BUFFLENGTH);
       }

	SJ_print (wtmp);
	Clear_EOL ();
	Flush ();

	*wtmp = '\0';
	*tmp = '\0';
	*yomi = '\0';
	wrap_off();
	SaveConversion ();
	while ((inc = inkey ()) != EOF) {
		if (AnotherConversion ()) {
			unget_key (inc);
			sj3_rkclear();
			wrap_on();
			return (1);
		}
		if (keyvalue == KEY_CONV) {
			sj3_rkclear();
			wrap_on();
			return (1);
		}
		if (keyvalue == KEY_KETTEI || keyvalue == KEY_HENKAN) {
			break;
		}
		else if (keyvalue != KEY_NORMAL) {
			if ((c = write_pseq (2)) != 0)
				inc = c;
		}
		
		if (inc == '\n' || inc == '\r'|| IsESC (inc))
			break;
		else if (iscntrl(inc) && !is_bs(inc))
			beep();
		else if ((len = wslen(wtmp)) >= lim && !is_bs(inc))
			beep();
		else	
			(void)exec_romaji2(inc, NULL, wtmp, NULL, len, -1);
	}
	(void)exec_romaji2(0, NULL, wtmp, NULL, wslen(wtmp), -1);
	wscpy(yomi, wtmp);
	sj3_rkclear();
	wrap_on();
	return (0);
}

gethinshi()
{
	register int inc, hcode;

	TopGuide(); 
	printR(WCGtoroku);
	SJ_print (WCTOUROKU_HIN);
	Clear_EOL();
	SJ_print(WCOne);
	Backspace();
	Flush();
	inc = inkey();
	if (inc >= '2' && inc <= '5')
		hcode = inc - '0';
	else
		hcode = 1;
	return(hcode);
}
	
getcheck (yomi, kanji, hinshi, prompt)
wchar16_t	*yomi, *kanji, *hinshi, *prompt;
{
	register int	c;
	u_char		tmp[BUFFLENGTH + KLEN];
	extern wchar16_t  *Y;
	wchar16_t         wtmp[BUFFLENGTH + KLEN];
	u_char          ktmp[KLEN+1], ytmp[YOMILEN+1], htmp[YOMILEN+1],
	                ttmp[BUFFLENGTH];
	
	TopGuide (); 
	printR (prompt);
	if (hinshi == NULL)
        {
		(void) wcstombs((char *)ktmp, kanji, KLEN+1);
		(void) wcstombs((char *)ytmp, yomi,  YOMILEN+1);
		(void) wcstombs((char *)ttmp, WCSYOUKYO_OK, BUFFLENGTH);
		sprintf ((char *)tmp, (char *)ttmp, ktmp, ytmp);
	}
	else
        {
		(void) wcstombs((char *)ktmp, kanji, KLEN+1);
		(void) wcstombs((char *)ytmp, yomi,  YOMILEN+1);
		(void) wcstombs((char *)htmp, hinshi, YOMILEN+1);
		(void) wcstombs((char *)ttmp, WCTOUROKU_OK, BUFFLENGTH);
		sprintf ((char *)tmp, (char *)ttmp, ktmp, ytmp, htmp);
	}

	(void) mbstowcs(wtmp, (char *)tmp, BUFFLENGTH + KLEN);
	SJ_print (wtmp);
	Clear_EOL ();
	SJ_print (WCY);
	Backspace ();
	Flush ();

	SaveConversion ();
	c = inkey ();
	if (AnotherConversion ()) {
		unget_key (c);
		return (0);
	}

	if (keyvalue == KEY_CONV) {
		*yomi = '\0';
		return (1);
	}

	if (c == 'y' || c == 'Y' || c == '\n' || c == '\r' || c == ' ')
		return (1);
	if (keyvalue == KEY_HENKAN || keyvalue == KEY_KETTEI)
		return (1);
	if (IsESC (c)) {
		*yomi = '\0';
		return (1);
	}
	return (0);
}

getkstr (s, lim, guide)
wchar16_t	*s, *guide;
int	lim;
{
	register Conversion	*cv;
	register int		i;
	wchar16_t			tmp[BUFFLENGTH * 2];
	wchar16_t			*p;
	int			plen, chbun;

	chbun = 0;
	cv = GetConversion ();
	if (cv->Kanji != 0) {
		if (cv->Bnum > 1) {
			Ssbun(0);
			chbun++;
			plen = getbunsetu (tmp, guide);
		} else
			plen = Bkanji (tmp);
	}
	else if (isspaned ()) {
		plen = cv->out_point;
		Strncpy (tmp, cv->out_buf, plen);
		tmp[plen] = '\0';
	}
	p = tmp;
	for (i = 0 ; i < plen && i < lim ; i ++)
		*(s ++) = *(p ++);
	*s = '\0';
	return (chbun);
}

getbunsetu (kanji, guide)
wchar16_t	*kanji, *guide;
{
	register Conversion	*cv;
	register int		c;
	register int		bstart, bend;
	register int		i;
	u_short			row, col;
	wchar16_t			*p;
	int			plen, len;

	cv = GetConversion ();
	len = 0;
	p = kanji;
	*p = '\0';
	bstart = bend = cv->CurBun;
	
	StartGuide (&row, &col);
	printR (guide);
	SJ_print (WCMhani);
	Clear_EOL ();
	EndGuide (row, col);
	Flush ();
	SaveConversion ();
	while ((c = inkey ()) != EOF) {
		if (AnotherConversion ()) {
			unget_key (c);
			return (0);
		}
		cv = GetConversion ();
		if (keyvalue == KEY_RIGHT) {
			if ((bend == bstart) && (bend < cv->Bnum - 1)) {
				cv->PreBun = bend;
				bend ++;
				cv->CurBun = bstart = bend;
				Rdisp ();
			}
			else
				beep ();
		}
		else if (keyvalue == KEY_LEFT) {
			if ((bend == bstart) && (bend > 0)) {
				cv->PreBun = bend;
				bend --;
				cv->CurBun = bstart = bend;
				Rdisp ();
			}
			else
				beep ();
		}
		else if (keyvalue == KEY_UP) {
			if (bend < cv->Bnum - 1) {
				bend ++;
				cv->CurBun = cv->PreBun = bend;
				Rdisp ();
			}
			else
				beep ();
		}
		else if (keyvalue == KEY_DOWN) {
			if (bend > bstart) {
				cv->PreBun = bend;
				bend --;
				cv->CurBun = bend;
				Rdisp ();
			}
			else
				beep ();
		}
		else if (keyvalue == KEY_HENKAN || keyvalue == KEY_KETTEI ||
						   keyvalue == KEY_FLUSH) {
			break;
		}
		else if (keyvalue == KEY_NORMAL) {
			if (c == '\n' || c == '\r') {
				bstart = 0;
				bend = cv->Bnum - 1;
				break;
			}
			else if (c == ' ') {
				break;
			}
			else if (IsESC (c)) {
				return (0);
			}
			else
				beep ();
		}
		else
			beep ();
		Flush ();
	}
	
	for (i = bstart ; i <= bend ; i ++) {
		if ((plen = Bstr (i, p)) <= 0)
			continue;
		p += plen;
		len += plen;
	}
	return (len);
}

exec_syoukyo ()
{
	register int 	i;
	wchar16_t		yomi[YOMILEN+1], kanji[KLEN+1];
	wchar16_t		tmp[BUFFLENGTH], tmp2[BUFFLENGTH * 2];
        u_char          ytmp[YOMILEN+1], ktmp[KLEN+1], 
	                mtmp[BUFFLENGTH], mtmp2[BUFFLENGTH * 2],
	                ttmp[BUFFLENGTH];
	int		inc;
	int		val, retv;

	if (IsDcflag())
		return(0);
	val = getkstr (kanji, KLEN, WCGsyoukyo);
	if (*kanji == '\0')
		return (val);

	while (1) {
		if (getyomi (yomi, YOMILEN, WCGsyoukyo, kanji) || *yomi == '\0')
			break;
		if (getcheck (yomi, kanji, NULL, WCGsyoukyo))
			break;
	}
	if (*yomi == '\0')
		return (val);

	TopGuide ();
	printR (WCGsyoukyo);
	tmp[0] = '\0';
	for (i = 0; i < 5; i++) {
		retv = SJ2_syoukyo (yomi, kanji, hin_val[i].ivalue);
		if (retv == 0) {
			wscat(tmp, hin_val[i].keyword);
			wscat(tmp, WCKanma);
		} else if (retv != SJ3_WORD_NOT_EXIST) 
			break;
	}
	if (tmp[0] == '\0') {
		switch (retv) {
		case -1:
                        (void)dconnect(0);
			return(val);
		case SJ3_WORD_NOT_EXIST:
			SJ_print(WCNotExist);
			break;
		case SJ3_DICT_LOCKED:
			SJ_print(WCUserLock);
			break;
		case SJ3_BAD_YOMI_STR:
			SJ_print(WCYomiShokyo); 
			break;
		case SJ3_BAD_KANJI_STR:
			SJ_print(WCKanjiShokyo);
			break;
		default:
			SJ_print(WCShokyoError);
			break;
		}
	} else {
		i = wslen(tmp);
		tmp[i - 1] = '\0';
                (void) wcstombs((char *)mtmp, tmp, BUFFLENGTH);
                (void) wcstombs((char *)ytmp, yomi, YOMILEN+1);
                (void) wcstombs((char *)ktmp, kanji, KLEN+1);
                (void) wcstombs((char *)ttmp, WCSYOUKYO_SUCC, BUFFLENGTH);
		sprintf((char *)mtmp2, (char *)ttmp, ktmp, ytmp, mtmp);
                (void) mbstowcs(tmp2, (char *)mtmp2, BUFFLENGTH * 2);
		SJ_print(tmp2);
	}
	Clear_EOL ();
	Flush ();

	inc = inkey ();
	if (AnotherConversion ()) {
		unget_key (inc);
	}
	return (val);
}
