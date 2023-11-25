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
 * $SonyRCSfile: code.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:09 $
 */



#include "sj_sysvdef.h"
#include "wchar16.h"
#include "kctype.h"
#if defined(__sony_news) && defined(SVR4)
#include "jctype.h"
#endif
#include "sj2.h"
#include "key.h"
#include "rk.h"

static u_char cbuf[5];
static int	cur_code = CODE_EUC;
extern int      current_locale;
static int ccnt = 0;

codeconv(c, rkstr)
wchar16_t c;
u_int *rkstr;
{
	register u_short ccode, i;
        u_short CheckCcode();
	extern int erase_char;

	if (c == erase_char) {
		if (ccnt) {
			ccnt--;
			*rkstr++ = erase_char + ERRCODE;
		} else {
			*rkstr++ = erase_char;
		}
	} else if (iswcntrl(c) || (WcIsKANJI(c) && ccnt == 0)) {
		*rkstr++ = c;
		if (iswcntrl(c))
			ccnt = 0;
	  } else if ((ccode = CheckCcode(c)) > 0) {
	  if (ccnt == 4) {
	  if ((cur_code != CODE_EUC) || (cur_code != CODE_EUC2)) {
		  if ((cur_code == CODE_KUTEN) || (cur_code == CODE_KUTEN2))
		      ccode += 0x2020;
		  ccode = jis2euc(ccode & 0x7f7f);
	  }
		        if ((cur_code == CODE_KUTEN2) || 
			    (cur_code == CODE_JIS2) || (cur_code == CODE_EUC2))
				ccode &= WcX0212MASK;
			ccnt--;
			for (; ccnt > 0; ccnt--)
				*rkstr++ = erase_char + ERRCODE;
			*rkstr++ = SetMojilen(4) + ccode;
		} else
			*rkstr++ = c + ERRCODE;
	} else
		*rkstr++ = ERRBEL;
	*rkstr = RKEND;
}

u_short
CheckCcode (c)
wchar16_t c;
{
        register u_char c1;   
        register u_short code;

        if (!WcIsASCII(c) ||  !isxdigit(WcLowByte(c)) ||
            (cur_code == CODE_KUTEN && !isdigit(WcLowByte(c))))
                return(0);
        cbuf[ccnt] = WcLowByte(c);
        cbuf[ccnt + 1] = '\0';

        if ((cur_code == CODE_KUTEN) || (cur_code == CODE_KUTEN2)) {
		if (ccnt % 2) {
                        code = (cbuf[ccnt-1] - '0') * 10 + cbuf[ccnt] - '0';
                        if (code < 1 || code > 94)
                                return(0);
                        if (ccnt >= 3)
                                code += ((cbuf[0]-'0') * 10 + cbuf[1]-'0') << 8;
		} else
                        code = 1;
	} else {
                code = (u_short)strtol((char *)cbuf, (char **)NULL, 16);
                if (ccnt % 2)
                        c1 = code & 0xff;
                else
                        c1 = code & 0xf;
	switch(cur_code) { 
	      case CODE_SJIS: 
                        if (ccnt == 0 && (c1 == 8 || c1 == 9 || c1 >= 0xe))
                                break;
                        if (ccnt == 1 && issjis1(c1))
                                break;
                        if (ccnt == 2 && c1 >= 4)
                                break;
                        if (ccnt == 3 && issjis2(c1))
                                break;
                        return(0);
	      case CODE_EUC:
  	      case CODE_EUC2:
                        if (ccnt % 2 && iseuc(c1))
                                break;
                        if (!(ccnt % 2) && (c1 >= 0xa && c1 <= 0xf))
                                break;
                        return(0);
	      case CODE_JIS:
   	      case CODE_JIS2:
                        if (ccnt % 2 && isjjis(c1))
                                break;
                        if (!(ccnt % 2) && (c1 >= 2 && c1 <= 7))
                                break;
                        return(0);
	}
}
        ccnt++;
        return(code);
}

u_short
strtocode()
{
        register int c;
        u_short code;

	while (ccnt < 4) {
                c = inkey();
		if (AnotherConversion ()) {
                        unget_key(c);
                        ccnt = 0;
                        return(-1);
		}
		if ((code = CheckCcode(c)) == 0) {
                        ccnt = 0;
                        beep();
                        return(0);
		}
	}
        ccnt = 0;
	switch (cur_code) {
	      case CODE_SJIS :
                return(sjis2jis(code));
	      case CODE_KUTEN :
   	      case CODE_KUTEN2:
                return(code + 0x2020);
	      default :
                return(code & 0x7f7f);
	}
}

ChangeCode ()
{
	if (cur_code == CODE_KUTEN2)
		cur_code = CODE_SJIS;
	else {
		if (current_locale == LC_CTYPE_SHIFTJIS) {
			if( cur_code == CODE_KUTEN) {
			        cur_code = CODE_SJIS;
				return;
			}
			if(cur_code == CODE_SJIS) {
				cur_code++;
			} else {
				cur_code += 2;
			}
		} else
		        cur_code++;
	}
}

SetCode(code)
int code;
{
	cur_code = code;
}

GetCcode()
{
	return(cur_code);
}

