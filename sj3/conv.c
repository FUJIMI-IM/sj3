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
 * $SonyRCSfile: conv.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:11 $
 */



#include <sys/errno.h>
#include "wchar16.h"
#include "select.h"
#include "key.h"
#include "inkey.h"
#include "common.h"


#define MSEC	300



int	keyvalue;				

char	*funckey[] = {				
		"\033OH",	"\033OP",	"\033OQ",
		"\033OR",	"\033OS",	"\033OT",
		"\033OU",	"\033OV",	"\033OW",
		"\033OX",	"\033OY",	"\033OZ",
		"\033[C",	"\033OC",	"\033[D",
		"\033OD",	"\033[A",	"\033OA",
		"\033[B",	"\033OB",	"\033[H",
		"\033[4~",	"\033[11~",	"\033[8~",
		"\033[12~",	"\033[13~",	"\033[9~",
		"\033[14~",	"\033[15~",	"\033[17~",
		"\033[18~",	"\033[19~",	"\033[20~",
		"\033[21~",	"\033[7~",
	};

int	funcvalue[] = {				
		KEY_CONV,	KEY_HENKAN,	KEY_MUHENKAN,
		KEY_ETC,	KEY_KETTEI,	KEY_CODE,
		KEY_HALPHA,	KEY_ZALPHA,	KEY_HKATA,
		KEY_ZKATA,	KEY_ZHIRA,	KEY_OTHER,
		KEY_RIGHT,	KEY_RIGHT,	KEY_LEFT,
		KEY_LEFT,	KEY_UP,		KEY_UP,
		KEY_DOWN,	KEY_DOWN,	KEY_CONV,
		KEY_HENKAN,	KEY_HENKAN,	KEY_MUHENKAN,
		KEY_MUHENKAN,	KEY_ETC,	KEY_KETTEI,
		KEY_KETTEI,	KEY_CODE,	KEY_HALPHA,
		KEY_ZALPHA,	KEY_HKATA,	KEY_ZKATA,
		KEY_ZHIRA,	KEY_CONV,
	};

int	keynumber = sizeof(funckey)/sizeof(char*); 



extern	int	Direct;				
extern	int	master;				
extern	wchar16_t	Ukey[UKEYNUM][SEQLEN+1];
extern	int	Uvalue[];
extern	int	Unumber;



convert_stat ()
{
	stat_init ();
	stat_conv ();
}

inputprocess ()
{
	register int	c;
	wchar16_t		obuf[2];

	if (Direct) {
		set_guide_line (KEY_CONV);
		convert_stat ();
	}
	set_guide_line (KEY_NORMAL);
	print_guide_line ();
	for (;;) {
		if ((c = inkey ()) == EOF)
			continue;
		switch (keyvalue) {
		case KEY_NORMAL:
			obuf[0] = c;
			obuf[1] = 0;
			SJ_write (obuf, 1);
			break;
		case KEY_CONV:
			set_guide_line (KEY_CONV);
			convert_stat ();
			set_guide_line (KEY_NORMAL);
			print_guide_line ();
			break;
		default:
			write_pseq (0);
			break;
		}
	}
}


wchar16_t		ibuf[KEYBUFSIZ];	
wchar16_t		pars_seq[KEYBUFSIZ];
int		buf_count = 0;		
int		pars_n;
static int	escape = 0;

inkey ()
{
	register Conversion	*cv;
	register int	i, j;
        register wchar16_t c;

	cv = current_conversion;
	keyvalue = KEY_NORMAL;
	pars_n = 0;
	if (buf_count) {
		if (escape && cv->Cflag)
			master_flush ();
		c = *ibuf;
	}
	else {
		if ((c = SJ_getchar ()) == (wchar16_t) EOF)
		    
			return ((short) c); 
		*ibuf = c;
		buf_count = 1;
	}

	
	if (*ibuf == ESC) {
		keyvalue = sequence (ibuf, &buf_count);
		escape = 1;
	}
	else {
		escape = 0;
	}

	if (keyvalue == KEY_NORMAL)
		keyvalue = parse_command (ibuf, &buf_count);

	if (keyvalue == KEY_NORMAL) {
		c = *ibuf;
		for (i=1 ; i < buf_count ; i++)
			ibuf[i - 1] = ibuf[i];
		buf_count--;
	}

	if (buf_count > KEYBUFLIM) {		
		for (i = buf_count - 1; i >= 0 ; i --) {
			if (ibuf[i] == ESC) {
				
				for (j = i ; j < buf_count ; j ++)
					ibuf[j-i] = ibuf[j];
				buf_count -= i;
				return(c);
			}
		}
		
		buf_count = 0;
	}
	return (c);
}

sequence (buf, count)
wchar16_t         *buf;
register int	*count;
{
	


	register int	n;
	SELECT_FD	ifds;
	struct timeval	t;
	extern int	errno;
	register wchar16_t *s;
	int		value;


	if (*count > 1 && (value = parse_escape (buf, count, 1)) != KEY_MORE)
		return (value);

	s = buf + *count;
	for (;;) {
		t.tv_sec = 0;
		t.tv_usec = UsecWeight (0);	
		FD_ZERO (&ifds);
		FD_SET (STDIN, &ifds);
		if (select (STDIN + 1, &ifds, 0, 0, &t) <= 0) {
			if (errno == EINTR) {
				errno = 0;
				continue;
			} else {
				return (parse_escape (buf, count, 0));
			}
		}
		if ((n = SJ_read (s, KEYBUFLIM)) <= 0) {
			return (parse_escape (buf, *count, 0));
		}
		s += n;
		*count += n;
		if ((value = parse_escape (buf, count, 1)) == KEY_MORE)
			continue;
		else {
			return (value);
		}
	}
}



parse_escape (s, count, more)
register wchar16_t *s;
register int	*count;
int		more;
{
	register int	i, j;
	wchar16_t		temp[KEYBUFSIZ];
	int		possible;

	possible = 0;

	

	for (i = 0 ; i < Unumber ; i ++) {

		
		for (j=0 ; j < *count ; j++) {
			if (s[j] != Ukey[i][j])
				break;
		}

		
		if (j == wslen (&Ukey[i][0])) {

			
			Strncpy (pars_seq, s, j);
			pars_seq[j] = (wchar16_t) '\0';
			pars_n = j;

			
			*count -= j;

			
			if (*count > 0) {
				Strncpy (temp, &s[j], *count);
				temp[*count] = (wchar16_t) '\0';
                                wscpy(s, temp);
			}

			return (Uvalue[i]);
		}
		else if (j == *count)
			possible = 1;
	}

	

	for (i = 0 ; i < keynumber ; i ++) {

		
		for (j = 0 ; j < *count ; j ++) {
			if (s[j] != (u_char)funckey[i][j])
				break;
		}

		
		if (j == strlen (funckey[i])) {

			
			Strncpy (pars_seq, s, j);
			pars_seq[j] = (wchar16_t) '\0';
			pars_n = j;

			
			*count -= j;

			
			if (*count > 0) {
				Strncpy (temp, &s[j], *count);
				temp[*count] = (wchar16_t) '\0';
				wscpy (s, temp);
			}

			return (funcvalue[i]);
		}
		else if (j == *count)
			possible = 1;
	}

	if (*count == 1)
		return (KEY_NORMAL);
	else if (more && possible)
		return (KEY_MORE);
	else {
		Strncpy (pars_seq, s, *count);
		pars_seq[*count] = (wchar16_t) '\0';
		pars_n = *count;
		*count = 0;
		*s = '\0';
		return (KEY_OTHER);
	}
}

write_pseq (mod)
int	mod;
{
	

	if (mod == 3) {
		if (pars_n > 0) {
			pars_n = 0;
			return (*pars_seq);
		}
	}
	if (mod == 2) {
		if (pars_n == 1) {
			pars_n = 0;
			return (*pars_seq);
		}
		pars_n = 0;
		return (0);
	}
	if (mod == 1) {
		if (pars_n == 1) {
			pars_n = 0;
			return (*pars_seq);
		}
		else if (IsGoto (*pars_seq)) {
			Strncpy (&ibuf[buf_count], &pars_seq[1], pars_n - 1);
			buf_count += pars_n - 1;
			pars_n = 0;
			return (*pars_seq);
		}
	}
	if (pars_n > 0)
		SJ_through (pars_seq, pars_n);
	pars_n = 0;
	return (0);
}

