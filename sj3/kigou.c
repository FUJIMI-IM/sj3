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
 * $SonyRCSfile: kigou.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:19 $
 */



#include "common.h"
#include "kctype.h"
#include "key.h"

#define BEGIN_CODE	0x2121		
#define END_CODE	0x7f7e
#define INCRESE		16

static int      lastcode        = BEGIN_CODE;

static u_short	row, col;

kigou ()
{
	extern int	keyvalue;
        register int    increse, code, basecode;
        int             inc, ki, low;
        u_short         num, CheckCcode(), strtocode();
	int		rval;

	increse = INCRESE;
	disp_mode ();
	basecode = lastcode & 0xfff0;
	code = lastcode;
	StartGuide (&row, &col);
	Clear_EOL ();
	guide_kigou (basecode, code, increse);

	rval = 0;
	ki = 1;
	SaveConversion ();
	while (ki) {
		inc = inkey ();
		if (AnotherConversion ()) {
			unget_key (inc);
			break;
		}
top:
		switch (keyvalue) {
		case KEY_CONV:
			rval = 1;
			ki = 0;
			break;
			case KEY_RIGHT:
			if (code >= END_CODE) {
                                code = BEGIN_CODE;
                                basecode = code & 0xfff0;
			} else {
                                low = ++code & 0xff;
				if (low > 0x7e) {
                                        code &= 0xff00;
                                        code += 0x121;
                                        low = 0x21;
				}
				if (code >= basecode + increse) {
                                        if (low <= 0x21)
                                                basecode = code & 0xfff0;
                                        else
                                                basecode += increse;
				}
			}
                        guide_kigou (basecode, code, increse);
                        break;
		      case KEY_LEFT:
			if (code <= BEGIN_CODE) {
                                code = END_CODE;
                                basecode = code & 0xfff0;
			} else {
                                low = --code & 0xff;
				if (low < 0x21) {
                                        code &= 0xff00;
                                        code -= 0x82; 
                                        low = 0x7e;
				}
				if (code < basecode) {
                                        if (low >= 0x7e)
                                                basecode = code & 0xfff0;
                                        else
                                                basecode -= increse;
				}
			}
                        guide_kigou (basecode, code, increse);
                        break;
		      case KEY_UP:
			if (basecode <= BEGIN_CODE) {
                                code = basecode = END_CODE & 0xfff0;
			} else {
                                basecode -= increse;
                                low = basecode & 0xff;
				if (low == 0x20) {
                                        code = (basecode & 0xff00) + 0x21;
				} else {
					if (low < 0x20) {
                                                basecode &= 0xff00;
                                                basecode -= 0x90;
					}
                                        code = basecode;
				}
			}
                        guide_kigou (basecode, code, increse);
                        break;
		      case KEY_DOWN:
			if (basecode >= (END_CODE & 0xfff0)) {
                                code = BEGIN_CODE;
                                basecode = code & 0xfff0;
			} else {
                                basecode += increse;
                                low = basecode & 0xff;
				if (low > 0x70) {
                                        basecode &= 0xff00;
                                        basecode += 0x120;
                                        code = basecode + 1;
				} else
                                        code = basecode;
			}
                        guide_kigou (basecode, code, increse);
                        break;
		case KEY_HENKAN:
			input_kigou (basecode, code, increse);
			break;
		case KEY_KETTEI:
			input_kigou (basecode, code, increse);
			ki = 0;
			break;
		default:
			if ((keyvalue = IsCKey (inc)) != KEY_NORMAL)
				goto top;
				  if (CheckCcode(inc)) {
					  if ((num = strtocode()) > 0) {
                                        code = num;
                                        basecode = code & 0xfff0;
                                        guide_kigou(basecode, code, increse);
					  }
				  }
			else if (inc == ESC)
				ki = 0;
			else if (inc == '\n' || inc == '\r' || inc == ' ')
				input_kigou (basecode, code, increse);
			else if (IsRedraw (inc))
				guide_kigou (basecode, code, increse);
			else
				beep ();
			break;
		}
	}

	EndGuide (row, col);
	disp_mode ();
	return (rval);
}

IsRedraw (c)
int	c;
{
	if (c == 0x0c)
		return (1);
	else
		return (0);
}

IsCKey (inc)
register int	inc;
{
	switch (inc) {
	case 'l':
		return (KEY_RIGHT);
	case 'k':
		return (KEY_UP);
	case 'j':
		return (KEY_DOWN);
	case 'h':
		return (KEY_LEFT);
	default:
		return (KEY_NORMAL);
	}
}

guide_kigou (bcode, code, increse)
register int	bcode, code, increse;
{
	register int	i;
	extern wchar16_t	Mguide[];
	wchar16_t          wtmp[20];
	char		tmp[20];
        u_short         prcode;

	TopGuide ();
	printR (Mguide);
	put_space (1);
	put_space (1);
	for (i = 0 ; i < increse ; i ++)
		pkigou (bcode + i);
        prcode = code;
	if (((i = GetCcode()) == CODE_KUTEN) || (i == CODE_KUTEN2)) {
                prcode -= 0x2020;
                sprintf (tmp, "  [%02d%02d]", prcode >> 8, prcode & 0xff);
	} else {  
                if (i ==  CODE_SJIS)
                        prcode = jis2sjis(prcode);
                else if ((i ==  CODE_EUC) || (i == CODE_EUC2))
                        prcode |= 0x8080;
                sprintf (tmp, "  [%04x]", prcode);
	}   
	(void) mbstowcs(wtmp, tmp, 20);
	SJ_print (wtmp);
	backspace (8 + (bcode + increse - code) * 2);
	Flush ();
}


pkigou (code)
register int    code;
{
	wchar16_t	tmp[3];
        u_short ccode;

	if (jis2euc(code) == 0) {
		ccode = BEGIN_CODE;
	} else {
		ccode = code;
	}
        if ((GetCcode() == CODE_EUC2) || (GetCcode() == CODE_JIS2) ||
	    (GetCcode() == CODE_KUTEN2)) {
		ccode = WcJIS2X0212(ccode);
        } else {
		ccode = WcJIS2X0208(ccode);
        }     
        tmp[0] = ccode;
	tmp[1] = '\0';
        SJ_print (tmp);
}

input_kigou (basecode, code, increse)
register int	basecode, code, increse;
{
	wchar16_t	s[2];
        u_short ccode, svlen;
        register Conversion *cv;

	if (jis2euc(code) == 0) {
                beep();
                return;
	}
        if ((GetCcode() == CODE_EUC2) || (GetCcode() == CODE_JIS2) ||
	    (GetCcode() == CODE_KUTEN2)) {
		ccode = WcJIS2X0212(code);
        } else {
		ccode = WcJIS2X0208(code);
        }     
        EndGuide (row, col);
        s[0] = ccode;
	s[1] = '\0';
	if (BuffYes ()) {
                cv = GetConversion();
                save_obuf();
                (void)exec_romaji(s[0]);
		if (os_point > 0) {
                        svlen = cv->Vlen;
                        vprintU(osave_buf, 1);
                        Cgoto(svlen);
		}
                restore_obuf();
	} else {
		master_out (s, 2);
		master_flush(); 
	}
	StartGuide (&row, &col);
	guide_kigou (basecode, code, increse);
	lastcode = code;
}
