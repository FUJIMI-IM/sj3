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
 * $SonyRCSfile: etc.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:15 $
 */



#include "common.h"
#include "key.h"

extern int	keyvalue;
static char   rehname[SHORTLENGTH * 2];
extern char   *cur_serv;
static int dcflag = 0;

exec_etc (key)
int	key;
{
	u_short		row, col;
	register int	inc;
	int		chbun;
	int 		edited;
	int		etckey;
	register Conversion	*cv;

	cv = GetConversion ();

	edited = 0;
	chbun = 0;
	StartGuide (&row, &col);
	if (key == KEY_ETC) {
		printR (WCGEtc);
		if (cv->column >= MINCOL)
			spaceR (WCGetc);
		Clear_EOL ();
		EndGuide (row, col);
		SaveConversion ();
		inc = inkey ();
		if (AnotherConversion ())
			return(KEY_NORMAL);
		cv = GetConversion ();
		etckey = etc_map (keyvalue);
	}
	else {
		EndGuide (row, col);
		etckey = key;
	}

	switch (etckey) {
	case KEY_NORMAL:
normal:
		switch (inc) {
		case 'h':
			helplevel ();
			break;
		case 't':
			if (!IsCflag ())
				chbun = exec_toroku ();
			break;
		case 's':
			if (!IsCflag ())
				chbun = exec_syoukyo ();
			break;
		case 'v':
		case 'd':
			disp_version (inc);
			break;
		case 'w':
			change_weight ();
			break;
		default:
			break;
		}
		break;
	case KEY_HELP:
		helplevel ();
		break;
	case KEY_TOROKU:
		if (!IsCflag ())
			chbun = exec_toroku ();
		break;
	case KEY_SYOUKYO:
		if (!IsCflag ())
			chbun = exec_syoukyo ();
		break;
	case KEY_SJRC:
		if (IsCflag ())
			exec_sjrc ();
		break;
	case KEY_EDIT:
		if (IsHenkan ()) {
			if (cv->CurBun < 0) {
				cv->CurBun = 0;
				Bdisp (1);
			}
			edited = exec_edit ();
		}
		break;
	case KEY_RECONNECT:
                if (reconnect() && IsHenkan()) {
                        EndGuide (row, col);
                        Clear_EOL();
                        Ccheck();
                        Cclear(1);
                        ResetHenkan();
                        vprintU(cv->out_buf, 1);
                        cv->Kanji = 0;
                        return(KEY_NORMAL);
                }
		break;
	case KEY_ETC:
		break;
	default:
		if ((inc = write_pseq (2)) != 0)
			goto normal;
		return(etckey);
		break;
	}

	EndGuide (row, col);
	if (edited) {
		if (!Sdouon (3))	
			Rdisp ();
	}
	if (chbun)
		Ssbun(1);
	return(KEY_NORMAL);
}

Ssbun(flag)
int flag;
{
	register Conversion *cv;
	static short ScurBun, SpreBun;

	cv = GetConversion();
	if (flag) {
		if (cv->CurBun != ScurBun || cv->PreBun != SpreBun) {
			cv->CurBun = ScurBun;
			cv->PreBun = SpreBun;
			Bdisp();
		}
	} else {
		ScurBun = cv->CurBun;
		SpreBun = cv->PreBun;
	}
}

etc_map (key)
int	key;
{
	extern int	u_etckeys;
	extern int	Uetckey[], Uetcval[];
	register int	i;
	int		val;

	for (i = 0 ; i < u_etckeys ; i ++) {
		if (key == Uetckey[i])
			return (Uetcval[i]);
	}

	switch (key) {
	case KEY_HENKAN:
		val = KEY_TOROKU;
		break;
	case KEY_MUHENKAN:
		val = KEY_SYOUKYO;
		break;
	case KEY_ETC:
		val = KEY_HELP;
		break;
	case KEY_KETTEI:
		val = KEY_SJRC;
		break;
	case KEY_CODE:
		val = KEY_EDIT;
		break;
	case KEY_HALPHA:
		val = KEY_RECONNECT;
		break;
	default:
		val = KEY_NORMAL;
		break;
	}
	return (val);
}

helplevel ()
{
	wchar16_t                 wtmp[BUFFLENGTH];
	u_char                  mtmp[BUFFLENGTH];
	u_char			tmp[BUFFLENGTH];
	register Conversion	*cv;

	cv = GetConversion ();

	if (Hlevel == 1 && cv->column >= MINCOL) {
		HHlevel = 2;
		Hlevel = 2;
		(void) wcstombs((char *)mtmp, WCMHelp, BUFFLENGTH);
		sprintf ((char *)tmp, (char *)mtmp, "on");
		(void) mbstowcs(wtmp, (char *)tmp, BUFFLENGTH);
	}
	else {
		HHlevel = 1;
		Hlevel = 1;
                (void) wcstombs((char *)mtmp, WCMHelp, BUFFLENGTH);
                sprintf ((char *)tmp, (char *)mtmp, "off");  
                (void) mbstowcs(wtmp, (char *)tmp, BUFFLENGTH);
	}
	guide_print_pause (WCGHelp, wtmp);
}

disp_version (c)
int	c;
{
	extern char	*Version, *Copyright, *Date;
	char		tmp[BUFFLENGTH];

	wchar16_t         wtmp[BUFFLENGTH];

	if (c == 'v')
		sprintf (tmp, "%s,  %s ", Version, Copyright);
	else if (c == 'd')
		sprintf (tmp, "%s ", Date);
	else
		return;
        (void) mbstowcs(wtmp, tmp, BUFFLENGTH);
	guide_print_pause (0, wtmp);
}

exec_sjrc ()
{
	extern char	RCfile[];
	char		tmp[BUFFLENGTH];
	u_char         SUCCorFAIL[BUFFLENGTH];
	wchar16_t        wtmp[BUFFLENGTH];


	if (*RCfile == (char)NULL) {
		getsjrc();
		if (*RCfile == (char)NULL) 
	        {
			(void) wcstombs((char *)SUCCorFAIL, WCNOTHsjrc, BUFFLENGTH);
			sprintf((char *)tmp, (char *)SUCCorFAIL);
			(void) mbstowcs(wtmp, tmp, BUFFLENGTH);
		}
		else
	        {
			(void) wcstombs((char *)SUCCorFAIL, WCSUCCsjrc, BUFFLENGTH);
			sprintf (tmp, "%s %s ", RCfile, SUCCorFAIL);
			(void) mbstowcs(wtmp, tmp, BUFFLENGTH);
		}
	} else {
		if (setrc () == TRUE)
	        {
			(void) wcstombs((char *)SUCCorFAIL, WCSUCCsjrc, BUFFLENGTH);
		} else {
			(void) wcstombs((char *)SUCCorFAIL, WCFAILsjrc, BUFFLENGTH);
		}
		sprintf (tmp, "%s %s ", RCfile, SUCCorFAIL);
		(void) mbstowcs(wtmp, tmp, BUFFLENGTH);
	}
	guide_print_pause (0, wtmp);
}

static int	weight = 3;
static int	seed = 100000;	

change_weight ()
{
	register int	c;
	char		s[2];
	char		tmp[BUFFLENGTH];
	u_char          mtmp[BUFFLENGTH];
        wchar16_t         wtmp[BUFFLENGTH];

	(void) wcstombs((char *)mtmp, WCWeight_mes, BUFFLENGTH);
	sprintf(tmp, (char *)mtmp, weight);
	(void) mbstowcs(wtmp, tmp, BUFFLENGTH);
	c = guide_print_pause (0, wtmp);

	if (isdigit (c) && (c != '0')) {
		s[0] = c;
		s[1] = '\0';
		weight = atoi (s);
	}
}

UsecWeight (sw)
int	sw;
{
	

	if (sw)		
		return (seed);
	else		
		return (seed * weight);
}

dconnect(aflag)
int aflag;
{
        register int err;
	char tmp[BUFFLENGTH];
	wchar16_t wtmp[BUFFLENGTH];
	u_char  dtmp[BUFFLENGTH];

	if (!dcflag) {
                (void) wcstombs((char *)dtmp, WCDiscon, BUFFLENGTH);
		sprintf(tmp, (char *)dtmp, cur_serv);
		beep();
                (void) mbstowcs(wtmp, tmp, BUFFLENGTH);
		guide_print_pause (0, wtmp);
 
                
                if (aflag && keyvalue == KEY_HENKAN) {
                        if ((err = sj3_autocon()) == 0) {
				(void) wcstombs((char *)dtmp, WCRserv_OK, BUFFLENGTH);
				sprintf(tmp, (char *)dtmp, cur_serv);
				(void) mbstowcs(wtmp, (char *)tmp, BUFFLENGTH);
				guide_print_pause(0, wtmp);
                        } else {
                                guide_print_pause(WCSpace, WCNconnect);
                                dcflag = 1;
                        }
                } else
                        dcflag = 1;
		disp_mode();
	}
	return(dcflag);
}

set_dcflag(flag)
int flag;
{
	dcflag = flag;
}

IsDcflag()
{
	return(dcflag);
}

reconnect ()
{
	register int	inc, err;
	char		tmp[BUFFLENGTH], *hp;
	char		newserv[SHORTLENGTH * 2], *nsp;
	wchar16_t wtmp[BUFFLENGTH];
	u_char rtmp[BUFFLENGTH];

	strncpy(newserv, cur_serv, SHORTLENGTH * 2 - 1);
	newserv[SHORTLENGTH * 2 - 1] = '\0';
	while(1) {
		SaveConversion();
		TopGuide();
		printR(WCGrcon);
		(void) wcstombs((char *)rtmp, WCRserv, BUFFLENGTH);
		sprintf(tmp, (char *)rtmp, newserv);
		(void) mbstowcs(wtmp, tmp, BUFFLENGTH);
		SJ_print (wtmp);
		Clear_EOL();
		Flush();
		nsp = newserv;
		while ((inc = inkey()) != EOF) {
			if (AnotherConversion()) {
				unget_key(inc);
				return(0);
			}
			if (keyvalue != KEY_NORMAL && keyvalue != KEY_FLUSH)
				return(0);
			if (inc == '\n' || inc == '\r' || IsESC(inc))
				break;
			if (is_bs (inc) && nsp > newserv) {
				nsp--;
				*nsp = '\0';
				backspace(1);
				Flush();
			} else if (isgraph(inc) && 
				   nsp < &newserv[SHORTLENGTH * 2 - 1]) {
				*nsp++ = inc;
				printf("%c", inc);
				Flush();
			} else 
				beep();
		}
		if (nsp > newserv) {
			*nsp = '\0';
                        strcpy(rehname, newserv);
                        cur_serv = rehname;
		} else if (newserv[0] == '\0') {
                        strncpy(newserv, cur_serv, SHORTLENGTH * 2 - 1);
			newserv[SHORTLENGTH * 2 - 1] = '\0';
		}
		if ((err = SJ2_reconnect()) == 0) {
			TopGuide();
			printR(WCGrcon);
			(void) wcstombs((char *)rtmp, WCRserv_OK, BUFFLENGTH);
			sprintf(tmp, (char *)rtmp, newserv);
			(void) mbstowcs(wtmp, tmp, BUFFLENGTH);
			SJ_print(wtmp);
			Clear_EOL();
			Flush();
			set_dcflag(0);
			inc = inkey();
			if (AnotherConversion ())
				unget_key(inc);
			break;
		} else {
			TopGuide();
			printR(WCGrcon);
			SJ_print(WCSpace);
			sj_check_error(err);
			SJ_print(WCContinue);
			Clear_EOL();
			Backspace();
			Flush();
			inc = inkey();
			if (AnotherConversion()) {
				unget_key(inc);
				set_dcflag(1);
				break;
			}
			if (inc == 'n' || inc == 'N' || inc == EOF) {
				set_dcflag(1);
				break;
			}
		}
	}
        return(1);
}			

guide_print_pause (s1, s2)
wchar16_t *s1, *s2;
{
	int	c;

	guide_print (s1, s2);
	SaveConversion ();
	c = inkey ();
	if (AnotherConversion ()) {
		unget_key (c);
		return (EOF);
	}
	return (c);
}


