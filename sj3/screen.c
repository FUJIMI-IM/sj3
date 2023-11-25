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
 * $SonyRCSfile: screen.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:22 $
 */



#include "common.h"
#include "key.h"

#define LINE_LENGTH	80

static wchar16_t	*Guide;		
static wchar16_t	Nguide[LINE_LENGTH + 1];
static wchar16_t	Cguide[LINE_LENGTH + 1];
wchar16_t		Mguide[LINE_LENGTH + 1];


vbackchar (n)
register int	n;
{
	register int	i;

	for (i=0 ; i < n ; i++)
		VBackspace ();
	for (i=0 ; i < n ; i++)
		Vput_space ();
	for (i=0 ; i < n ; i++)
		VBackspace ();
	Flush ();
}



wrap_off ()
{
	register Conversion	*cv;

	cv = GetConversion ();
	cv->wrap = 0;
	cv->PushCurrentVcol = cv->CurrentVcol;
}

wrap_on ()
{
	register Conversion	*cv;

	cv = GetConversion ();
	cv->wrap = 1;
	cv->CurrentVcol = cv->PushCurrentVcol;
}



Vput_space ()
{
	register Conversion	*cv;

	cv = GetConversion ();
	put_space (1);
	cv->Vindex[cv->Vlen ++] = cv->CurrentVcol ++;
	if (cv->wrap && ((cv->CurrentVcol % cv->column) == 1)) {
		put_space (1);
		Backspace ();
	}
}



VBackspace ()
{
	register Conversion	*cv;
	register int	i;

	cv = GetConversion ();
	if (cv->wrap && ((cv->CurrentVcol % cv->column) == 1)) {
		Ccheck ();
		i = cv->Vindex[cv->Vlen - 1] % cv->column;
		if (cv->Vlen > 0 && i > 1) {
			CursorSet (
			    cv->SavedRow + (cv->CurrentVcol / cv->column) - 1,
			    cv->Vindex[cv->Vlen - 1] % cv->column
			);
		}
		else
		CursorSet (
			cv->SavedRow + (cv->CurrentVcol / cv->column) - 1,
			cv->column
		);
	}
	else
		Backspace ();
	if (cv->Vlen > 0)
		cv->CurrentVcol = cv->Vindex[--(cv->Vlen)];
	else {
		cv->Vlen = 0;
		cv->CurrentVcol = cv->Vindex[cv->Vlen];
	}
}



master_out (s, n)
wchar16_t	*s;
int	n;
{
	master_write (s, n);
	master_flush ();
}

master_write (s, n)
wchar16_t	*s;
int	n;
{
	SJ_write (s, n);
}



vprintU (s, mod)
wchar16_t	*s;
char	mod;	
{
	register Conversion	*cv;
	register int		i, col, oldcol, len;
	register u_short	*p;

	cv = GetConversion ();
	printU (s);
	oldcol = cv->CurrentVcol % cv->column;
	if (cv->wrap && (mod == 1) && (oldcol == 0))
		cv->CurrentVcol ++;
	len = wcbyte (s);
	p = &(cv->Vindex[cv->Vlen]);
	cv->Vlen += len;
	for (i = 0 ; i < len ; i ++)
		*(p ++) = cv->CurrentVcol ++;
	if (cv->wrap) {
		col = cv->CurrentVcol % cv->column;
		if (col == 1) {
			put_space (1);
			Backspace ();
		}
		if ((mod == 1) && (oldcol + len > (unsigned int)cv->column)
		 && ((col & 1) == 0)) {
			cv->CurrentVcol ++;
		}
	}
	*p = cv->CurrentVcol;
	if (cv->CurrentVcol > cv->MaxVcol)
		cv->MaxVcol = cv->CurrentVcol;
	Flush ();
}



vprintR (s, mod)
wchar16_t	*s;
char	mod;	
{
	register Conversion	*cv;
	register int		i, col, oldcol, len;
	register u_short	*p;

	cv = GetConversion ();
	printR (s);
	oldcol = cv->CurrentVcol % cv->column;
	if (cv->wrap && (mod == 1) && (oldcol == 0))
		cv->CurrentVcol ++;
	len = wcbyte (s);
	p = &(cv->Vindex[cv->Vlen]);
	cv->Vlen += len;
	for (i = 0 ; i < len ; i ++)
		*(p ++) = cv->CurrentVcol ++;
	if (cv->wrap) {
		col = cv->CurrentVcol % cv->column;
		if (col == 1) {
			put_space (1);
			Backspace ();
		}
		if ((mod == 1) && (oldcol + len > (unsigned int)cv->column)
		 && ((col & 1) == 0)) {
			cv->CurrentVcol ++;
		}
	}
	*p = cv->CurrentVcol;
	if (cv->CurrentVcol > cv->MaxVcol)
		cv->MaxVcol = cv->CurrentVcol;
	Flush ();
}



IprintU (s, index)
wchar16_t	*s;
int	index;
{
	Cgoto (index);
	printU (s);
	Flush ();
}



IprintR (s, index)
wchar16_t	*s;
int	index;
{
	Cgoto (index);
	printR (s);
	Flush ();
}



printU (s)
wchar16_t	*s;
{
	under_in ();
	SJ_print (s);
	under_out ();
}



printR (s)
wchar16_t	*s;
{
	reverse_in ();
	SJ_print (s);
	reverse_out ();
}



Csave ()
{
	register Conversion	*cv;

	cv = GetConversion ();
	CursorRead (&(cv->SavedRow), &(cv->SavedCol));
	cv->CurrentVcol = cv->SavedCol;
	cv->Vlen = 0;
	cv->Vindex[cv->Vlen] = cv->MaxVcol = cv->CurrentVcol;
}

SetVcol (index)
int	index;
{
	register Conversion	*cv;

	cv = GetConversion ();
	cv->Vlen = index;
	cv->CurrentVcol = cv->Vindex[cv->Vlen];
}

Cload ()
{
	register Conversion	*cv;

	cv = GetConversion ();
	CursorSet (cv->SavedRow, cv->SavedCol);
}

Cclear (redraw)
int	redraw;
{
	register Conversion	*cv;
	register int	spaces;
	u_short		row, col;

	if (redraw) {
		cv = GetConversion ();
		CursorRead (&row, &col);
		spaces = (row - cv->SavedRow) * cv->column
			+ (col - cv->SavedCol) + 1;
		Cload ();
		put_space (spaces);
	}
	Cload ();
	Flush ();
}

Cgoto (index)
register int	index;
{
	register Conversion	*cv;
	register int	row, col;

	cv = GetConversion ();
	SetVcol (index);
	row = cv->SavedRow + (int)(cv->CurrentVcol / cv->column);
	col = cv->CurrentVcol % cv->column;
	CursorSet (row, col);
}

ClearToMax ()
{
	register Conversion	*cv;
	register int	space;

	cv = GetConversion ();
	space = cv->MaxVcol - cv->CurrentVcol;
	put_space (space);
}

Ccheck ()
{
	register Conversion	*cv;
	register int	row, tmp, diff;

	if (Cscroll ()) {
		cv = GetConversion ();
		tmp =  (int)(cv->CurrentVcol / cv->column);
		diff = cv->CurrentVcol - (tmp * cv->column);
		if (diff == 0)
			tmp --;
		row = cv->SavedRow + tmp;
		if (row >= (unsigned int)cv->line)
			cv->SavedRow -= (row - cv->line);
	}
}

spaceR (s)
register wchar16_t	*s;
{
	register wchar16_t	*t;
	wchar16_t		tmp[40];
	t = tmp;
	while (*s != '\0') {
		if (*s == ' ') {
			*t = '\0';
			printR (tmp);
			t = tmp;
			put_space (1);
		}
		else
			*(t ++) = *s;
		s ++;
	}
	*t = '\0';
	printR (tmp);
	Flush ();
}



clear_guide_line ()
{
	u_short	row, col;

	StartGuide (&row, &col);
	Clear_EOL ();
	EndGuide (row, col);
	Flush ();
}

print_guide_line ()
{
	u_short	row, col;

	StartGuide (&row, &col);
	spaceR (Guide);
	Clear_EOL ();
	EndGuide (row, col);
	Flush ();
}

set_guide_line (mode)
int	mode;
{
	


	switch (mode) {
	case KEY_NORMAL:
		wscpy (Nguide, WCSJrun);
		Guide = Nguide;
		break;
	case KEY_CONV:
		Guide = Cguide;
		break;
	default:
		break;
	}
}

disp_mode ()
{
	register Conversion	*cv;
	int	c_code;
	register wchar16_t	*Pmode, *Bmode;
	cv = GetConversion ();
	switch (cv->Imode) {
	case MODE_CODE:
		c_code = GetCcode();
		switch (c_code) {
		case CODE_SJIS:
			Pmode = WCGCshift;
			break;
		case CODE_EUC:
			Pmode = WCGCeuc;
			break;
		case CODE_JIS:
			Pmode = WCGCjis;
			break;
		case CODE_KUTEN:
			Pmode = WCGCkuten;
			break;
		case CODE_EUC2:
			Pmode = WCGCeuc2;
			break;
		case CODE_JIS2:
			Pmode = WCGCjis2;
			break;
		case CODE_KUTEN2:
			Pmode = WCGCkuten2;
			break;
		}
		break;
	case MODE_ZHIRA:
		Pmode = WCMode_Zhira;
		break;
	case MODE_ZKATA:
		Pmode = WCMode_Zkata;
		break;
	case MODE_HKATA:
		Pmode = WCMode_Hkata;
		break;
	case MODE_ZALPHA:
		Pmode = WCMode_Zalpha;
		break;
	case MODE_HALPHA:
		Pmode = WCMode_Halpha;
		break;
	default:
		break;
	}
	switch (cv->BuffMode) {
	case BUFF:
		Bmode = WCMode_buff;
		break;
	case UNBUFF:
		Bmode = WCMode_unbuff;
		break;
	default:
		break;
	}
	wscpy (Cguide, Bmode);
	wscat (Cguide, Pmode);
	wscpy (Mguide, Cguide);
	if (Hlevel > 1)
		wscat (Cguide, WCGmode);
	print_guide_line ();
}

TopGuide ()
{
	u_short	row, col;

	StartGuide (&row, &col);
}

guide_print (s1, s2)
wchar16_t	*s1, *s2;
{
	u_short	row, col;

	StartGuide (&row, &col);
	if (s1)
		printR (s1);
	SJ_print (s2);
	Clear_EOL ();
	EndGuide (row, col);
	Flush ();
}
