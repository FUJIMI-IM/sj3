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
 * $SonyRCSfile: display.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:12 $
 */



#include "sj_sysvdef.h"
#include <sys/errno.h>
#include "common.h"
#include "inkey.h"
#include "select.h"

char bp[TCBUFSIZE];		
char bpwork[AREASIZE];		
char cmove[SHORTLENGTH];	



extern int	master;
extern int	vflag;
extern char term_name[];

int	status_line;
char	*CM,*US,*UE,*SO,*SE,*CE,*CS,*BEL,*RP,*AP,*TS,*FS,*DS,*I2;
char	*SC,*RC,*SR,*DO;
static int keyboard = OTHER_KB;



checkterm ()
{
	char	*kb, *area, *tgetstr (), *tmp;
	int	val;
	register Conversion	*cv;

	cv = current_conversion;

	val = tgetent (bp, term_name);
	if (val == -1) {
		aprintf ("Can't open termcap file\n\r");
		done3 ();
	} else if (val == 0) {
		aprintf ("No termcap entry for %s\n\r",term_name);
		done3 ();
	}
        if ((val = tgetnum("co")) < 0) {
		cv-> column = 0;
	} else {
		cv-> column = val;
	}
        if ((val = tgetnum("li")) < 0) {
		cv->line = 0;
	} else {
		cv->line = val;
	}
        if (tmp = getenv("ROWS")) {
	        (void) sscanf(tmp, "%d", &val);
	        cv->line = val;
	}
        if (tmp = getenv("COLUMNS")) {
	        (void) sscanf(tmp, "%d", &val);
	        cv->column = val;
	}
	area = bpwork;

	if ((CM = tgetstr ("cm", &area)) == (char *)NULL) {
		aprintf ("No termcap 'cm'\n\r");
		done3 ();
	}
	US = tgetstr ("us", &area);
	UE = tgetstr ("ue", &area);
	SO = tgetstr ("so", &area);
	SE = tgetstr ("se", &area);
	CE = tgetstr ("ce", &area);
	if ((SC = tgetstr ("sc", &area)) == NULL)
		SC = "\0337";
	if ((RC = tgetstr ("rc", &area)) == NULL)
		RC = "\0338";
	if ((SR = tgetstr ("sr", &area)) == NULL)
		SR = "\033M";
	if ((DO = tgetstr ("do", &area)) == NULL)
		DO = "\033E";
	if ((CS = tgetstr ("cs", &area)) == NULL)
		CS = "\033[%i%d;%dr";
	if ((BEL = tgetstr ("bl", &area)) == NULL) {
		BEL = "\007";
	}
	if ((RP = tgetstr ("RP", &area)) == NULL) {
		RP = "\033[6n";
	}
	if ((AP = tgetstr ("AP", &area)) == NULL) {
		AP = "\033[%d;%dR";
	}
	if ((status_line = tgetflag("hs")) && tgetflag("es")) {
		TS = tgetstr ("ts", &area);  
		FS = tgetstr ("fs", &area);
		DS = tgetstr ("ds", &area);
#ifdef SVR4
		I2 = tgetstr ("i1", &area);
#else
		I2 = tgetstr ("i2", &area);
#endif
	} else {
		status_line = 0;
	}
#ifdef SVR4
	

	if (strstr(term_name, "news") != NULL) {
		keyboard = NEWS_KB;
		set_news();
	} else
		keyboard = OTHER_KB;
#else 

	kb = tgetstr ("KB", &area);
	if (kb == (char *)NULL || *kb == '\0')
		keyboard = OTHER_KB;
	else if (strcmp (kb, "news") == 0) {
		keyboard = NEWS_KB;
		set_news();
	} else
		keyboard = OTHER_KB;
#endif 

	strcpy (cmove, CM);
}

void outc(c)
char	c;
{
	putchar (c);
}

void erroutc(c)
{
	fputc (c, stderr);
}

start_guideline ()
{
	if (status_line && I2)
		tputs(I2, 1, erroutc);
}

end_guideline ()
{
	if (status_line) {
		tputs(DS, 1, outc);
		fflush(stdout);
	}
}

put_space (n)
int	n;
{
	register int	i;

	for (i = 0 ; i < n ; i ++)
		putchar (' ');
}

Backspace ()
{
	putchar (BS);
}

master_flush ()
{
	SELECT_FD	ifds;
	int		nc;
	struct timeval	t;
	extern int	errno;

	do {
cont:
		t.tv_sec = 0;
		t.tv_usec = UsecWeight (1);	
		FD_ZERO (&ifds);
		FD_SET (STDIN, &ifds);
		FD_SET (master, &ifds);
		if (select (master + 1, &ifds, 0, 0, &t) > 0) {
			if (FD_ISSET (STDIN, &ifds))
				if (ThroughIntr ())
					return;
			if (FD_ISSET (master, &ifds))
				output_master ();
		} else if (errno == EINTR) {
			errno = 0;
			goto cont;
		}
		if (ioctl (master, TIOCOUTQ, &nc) < 0)
			return (1);
	} while (nc > 0);
	return (0);
}

ThroughIntr ()
{
	wchar16_t	s[2];

	SJ_read (s, 1);
	s[1] = '\0';
	if (isintr (*s)) {
		master_out (s, 1);
		return (1);
	}
	else {
		unget_inkey2 (s, 1);
		return (0);
	}
}

under_in ()
{
	tputs(US, 1, outc);
}

under_out ()
{
	tputs(UE, 1, outc);
}

reverse_in ()
{
	tputs(SO, 1, outc);
}

reverse_out ()
{
	tputs(SE, 1, outc);
}



beep ()
{
	tputs(BEL, 1, outc);
	Flush ();
}



CursorSet (row, col)
u_short	row, col;
{
	tputs(tgoto(CM, col - 1, row - 1), 1, outc);
	Flush ();
}



RegionSet (row1, row2)
int row1, row2;
{
	


	tputs (SC, 1, outc);
	tputs (tgoto (CS, row2, row1 - 1), 1, outc);
	tputs (RC, 1, outc);
	tputs (DO, 1, outc);
	tputs (SR, 1, outc);
	tputs (SC, 1, outc);
	tputs (tgoto (CS, row2 - 1, row1 - 1), 1, outc);
	tputs (RC, 1, outc);
	Flush ();
}

#define SUCC	0
#define FAIL	1
#define CRLIMIT	4

int	master_flush_flag = 1;

CursorRead (row, col)
u_short	*row, *col;
{
	
	register int	count;
	int		row1, col1, row2, col2;

	row2 = col2 = 0;
	count = 0;
	for (;;) {
		if (++count > CRLIMIT) {
			break;
		}
		tputs(RP, 1, outc);
		fflush (stdout);
		if (CRP (&row1, &col1) == FAIL)
			continue;
		if (master_flush_flag)
			master_flush ();
		tputs(RP, 1, outc);
		fflush (stdout);
		if (CRP (&row2, &col2) == FAIL)
			continue;
		if (row1 == row2 && col1 == col2)
			break;
	}
	*row = row2;
	*col = col2;
}



#define CRPLIM	4
#define BACKLIM	16
#define MARKER	'%'			
#define SEPARA	';'			

CRP (row, col)
int	*row, *col;
{
	register char	*sa, *sd;
	register int	c;
	char		Rchar[4], Cchar[4];
	wchar16_t		Back[BACKLIM];
	register int	Back_count, count;

	

	count = 0;
	Back_count = 0;
	c = '\0';
top:
	sa = AP;
	sd = Rchar;

	if (++count > CRPLIM)
		return (FAIL);

	if (Back_count) {
		unget_inkey2 (Back, Back_count);
		Back_count = 0;
	}
	while ((c != ESC) && ((c = CRI ()) != ESC) && Back_count < BACKLIM)
		Back[Back_count++] = c;
	if (Back_count) {
		unget_inkey2 (Back, Back_count);
		Back_count = 0;
	}
	Back[Back_count++] = c;

	sa ++;
	while (*sa != '\0') {
		if (*sa == MARKER) {			
			sa += 2;	

			while (c = CRI ()) {
				if (c == ESC)
					goto top;
				Back[Back_count++] = c;
				if (isdigit (c)) {	
					*(sd ++) = c;
					*sd = '\0';
				}
				else if (c == *sa)	
					break;
				else
					goto top;
			}
			if (c == SEPARA)		
				sd = Cchar;
		}
		else {
			c = CRI ();
			if (c == ESC)
				goto top;
			Back[Back_count++] = c;
			if (c != *sa)
				goto top;
		}
		sa ++;
	}
	*row = atoi (Rchar);
	*col = atoi (Cchar);

	return (SUCC);
}



unget_inkey2 (s, n)
register wchar16_t *s;
register int	n;
{
	extern wchar16_t	ibuf[];
	extern int	buf_count;
	register int	i;

	if (buf_count < KEYBUFSIZ - n) {
		for (i = 0 ; i < n ; i ++)
			ibuf[buf_count ++] = *(s ++);
	}
}

unget_key (c)
int	c;
{
	wchar16_t tmp[2];

	tmp[0] = c;
	unget_inkey2 (tmp, 1);
}



CRI ()
{
	wchar16_t	c;

	SJ_read (&c, 1);
	return (c);
}



Clear_EOL ()
{
	reverse_out();
	tputs(CE, 1, outc);
}

StartGuide (row, col)
u_short	*row, *col;
{
	register Conversion	*cv;

	cv = current_conversion;
	master_flush_flag = 0;
	if (status_line) {
		tputs (tgoto (TS, 0, 0), 1, outc);
	}
	else {
		CursorRead (row, col);
		CursorSet (cv->line + 1, 0);
	}
	Flush ();
}

EndGuide (row, col)
u_short	row, col;
{
	if (status_line) {
		tputs (FS, 1, outc);
	}
	else {
		CursorSet (row, col);
	}
	Flush ();
	master_flush_flag = 1;
}

Flush ()
{
	fflush (stdout);
}

Cscroll ()
{
	
	return (1);
}
