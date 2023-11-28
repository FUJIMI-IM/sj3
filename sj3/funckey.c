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
 * $SonyRCSfile: funckey.c,v $  
 * $SonyRevision: 1.3 $ 
 * $SonyDate: 1997/01/23 11:20:53 $
 *
 * $Id: funckey.c,v 1.3 2004/07/04 08:38:33 hiroo Exp $
 */


#include <string.h>
#include "wchar16.h"
#include "sj2.h"
#include "key.h"
#include "Paths.h"
#include "sj3.h"

struct cvtkey {
	char *key_word;
	char key_seq[SEQLEN];
} cvtkeytbl[] = {
	{ "F1", "" },
	{ "F2", "" },
	{ "F3", "" },
	{ "F4", "" },
	{ "F5", "" },
	{ "F6", "" },
	{ "F7", "" },
	{ "F8", "" },
	{ "F9", "" },
	{ "F10", "" },
	{ "F11", "" },
	{ "F12", "" },
	{ "F13", "" },
	{ "F14", "" },
	{ "F15", "" },
	{ "F16", "" },
	{ "F17", "" },
	{ "F18", "" },
	{ "F19", "" },
	{ "F20", "" },
	{ "Select", "" },
	{ "Kanji", "" },
	{ "Cancel", "" },
	{ "Execute", "" },
	{ "Up", "" },
	{ "Down", "" },
	{ "Right", "" },
	{ "Left", "" },
	{ NULL, "" }
};

extern struct wcvtkey wcvtkeytbl[]; 

int	Uvalue[UKEYNUM];
wchar16_t	Ukey[UKEYNUM][SEQLEN + 1];
int	Unumber = 0;

int	comvalue[COMNUM];
wchar16_t	comkey[COMNUM];
int	comnumber = 0;

#define ETCKEYS	10
int	u_etckeys = 0;
int	Uetckey[ETCKEYS];
int	Uetcval[ETCKEYS];


extern wchar16_t *comstr[];

int	comstrvalue[KEY_OTHER] = {
		KEY_CONV,	KEY_HENKAN,	KEY_MUHENKAN,
		KEY_KETTEI,	KEY_ZHIRA,	KEY_HALPHA,
		KEY_ZALPHA,	KEY_HKATA,	KEY_ZKATA,
		KEY_CODE,	KEY_ETC,	KEY_RIGHT,
		KEY_LEFT,	KEY_UP,		KEY_DOWN,
		KEY_EDIT,	KEY_TOGGLE,	KEY_WRAPBACK,
		KEY_WRAP,	KEY_SJRC,	KEY_BUFFER,
		KEY_TOROKU,	KEY_SYOUKYO,	KEY_HELP,
		KEY_HA_KET,	KEY_FLUSH,	KEY_DEBUG,
		KEY_CANCEL,	KEY_RECONV,	KEY_IGNORE,
		KEY_TOP,	KEY_END,	KEY_RECONNECT,
		KEY_KILL,
		KEY_OTHER
	};


void
mk_cvtkey(char* term)
{
	char *p;
	char line[MAXLINE], rstr[16];
	FILE *fd;
	char cvtkey_file[LONGLENGTH];
	struct cvtkey *ckeyp;
	struct wcvtkey *wckeyp;

	strcpy(cvtkey_file, DEFKEYFILE);
	strcpy(line, term);
	if((p = strchr(line, '-')) != NULL)
		*p = '\0';
	strcat(cvtkey_file, line);

	if ((fd = fopen(cvtkey_file, "r")) != NULL) {
		while ((p = fgets(line, MAXLINE, fd)) != NULL) {
			if (*p == '#' ||*p == '\n' ||*p == '\0')
				continue;
			if ((p = getkey2(p, rstr)) == NULL)
				continue;
			ckeyp = cvtkeytbl;
                        wckeyp = wcvtkeytbl;
			while(ckeyp->key_word != NULL) {
				if (strcmp(ckeyp->key_word, rstr) == 0) {
					p = getkey2(p, ckeyp->key_seq);
					
					(void) mbstowcs(wckeyp->key_seq,
							ckeyp->key_seq, SEQLEN);
					break;
				}
				ckeyp++;
				wckeyp++;
			}
		}
		fclose(fd);
	}
}

char *
getkey2(char* istr, char* ostr)
{
	char *p;
	int i;

	while (*istr == ' ' || *istr == '\t')
		istr++;

	p = istr;
	i = 0;
	while (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\0') {
		*ostr++ = *p;
		if (++i > SEQLEN) {
			i = 0;
			break;
		}
		p++;
	}
	*ostr = '\0';
	if (i > 0)
		return(p);
	return(NULL);
}


void
clear_ukeys (void)
{
	Unumber = 0;
}


void
set_func (struct wordent word[])
{
	int	i, num;
	wchar16_t *p, *s;
	struct wcvtkey *ckeyp;

	if (Unumber >= UKEYNUM)
		return;
	for (i = 0 ; i < KEY_OTHER && *comstr[i] != '\0' ; i ++) {
		if (match (word[1].word_str, comstr[i])) {
			p = word[2].word_str;
			if (iswupper(*p)) {
				ckeyp = wcvtkeytbl;
				while(ckeyp->key_word != NULL) {
					if (wscmp(p, ckeyp->key_word) == 0) {
						p =  ckeyp->key_seq;
						if (*p == '\0')
							return;
						break;
					}
					ckeyp++;
				}
			}
			s = Ukey[Unumber];
			if ((num = eval_keys (p, s)) > 1) {
				if (Unumber < UKEYNUM)
					Uvalue[Unumber++] = comstrvalue[i];
			} else if (num >= 0) {
				if (comnumber < COMNUM) {
					comkey[comnumber] = *s;
					comvalue[comnumber++] = comstrvalue[i];
				}
			}
			return;
		}
	}
}


wchar16_t
eval_keys (wchar16_t* s, wchar16_t* p)
{
	wchar16_t	c, i;

	if (wslen (s) > SEQLEN)	
		return (-1);		
	i = 0;
	while ((c = *(s ++)) != '\0') {
		if (c == '^') {
			c = *(s ++);
			if ('@' <= c && c <= '_')
				c = c - '@';
			else if (c == '?')
				c = DEL;
			else
				c = *(--s);
		}
		i++;
		*(p ++) = c;
	}
	*p = '\0';
	return (i);
}


void
clear_key (void)
{
	clear_commkeys ();
	clear_etckeys ();
	clear_ukeys ();		
}


void
clear_commkeys (void)
{
	comnumber = 0;
}

int
eval_key (wchar16_t* s)
{
	wchar16_t	c;

	c = *s;

	
	if (c == '^') {
		c = s[1];
		if ('@' <= c && c <= '_')
			return (c - '@');
		else if (c == '?')
			return (DEL);
	}

	
	else if (c <= '~')
		return (c);

	return (-1);
}


void
clear_etckeys (void)
{
	u_etckeys = 0;
}


int
set_etckeys (struct wordent word[])
{
	int	i;

	if (u_etckeys >= ETCKEYS)
		return (1);
	for (i = 0 ; i < KEY_OTHER && *comstr[i] != '\0' ; i ++) {
		if (match (word[1].word_str, comstr[i])) {
			Uetcval[u_etckeys] = comstrvalue[i];
			break;
		}
	}
	if (i >= KEY_OTHER)
		return (1);
	for (i = 0 ; i < KEY_OTHER && *comstr[i] != '\0' ; i ++) {
		if (match (word[2].word_str, comstr[i])) {
			Uetckey[u_etckeys] = comstrvalue[i];
			break;
		}
	}
	if (i >= KEY_OTHER)
		return (1);
	u_etckeys ++;
	return (0);
}


wchar16_t
parse_command (wchar16_t* s, int* count)
{
	extern wchar16_t	pars_seq[];
	extern int	pars_n;
	int	i, j;
	wchar16_t c;

	c = *s;
	for (i = 0; i < comnumber ; i ++) {
		if (c == comkey[i]) {
			for (j = 1 ; j < *count ; j ++)
				s[j - 1] = s[j];
			(*count)--;
			*pars_seq = c;
			pars_seq[1] = 0;
			pars_n = 1;
			return (comvalue[i]);
		}
	}
	return (KEY_NORMAL);
}

