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
 * $SonyRCSfile: romaji.c,v $  
 * $SonyRevision: 1.2 $ 
 * $SonyDate: 1997/01/23 11:09:37 $
 */



#include "common.h"
#include "rk.h"
#include "key.h"

extern int rkerrbell;



exec_romaji(c)
wchar16_t	c;
{
	Conversion	*cv;
	wchar16_t *p;
	unsigned short c1, cc;
	unsigned int c2;
	unsigned short op;
	unsigned char mode;
	int len, i;
	unsigned int rkstr[MAXLLEN], *rkp;
	extern char erase_str[];

	cv = GetConversion ();
	op = cv->out_point;

	if (!is_bs(c) && BuffYes()) {
		if ((unsigned int) cv->ha_point + hs_point >= BUFFLENGTH - 2 &&
			!iswcntrl(c)) {
			beep();
			return(1);
		}
		cv->Halpha[cv->ha_point++] = c;
	}
		
	if ((mode = cv->Imode) & MODE_CONV) {
		sj3_rkconvc(c, rkstr);
	} else if (mode == MODE_CODE) {
		codeconv(c, rkstr);
	} else {
		rkstr[0] = SetMojilen(1) + c;
		rkstr[1] = RKEND;
	}
		
	rkp = rkstr;
	i = 1;
	while(*rkp != RKEND) {
		c1 = ((c2 = *rkp++) & RKMASK);
		if (c2 == ERRBEL) {
			beep();
			cv->ha_point--;
			break;
		}
		if (is_bs(c1)) {
			if (op <= 0) {
				if (os_point <= 0) {
					master_write(erase_str, 1);
               			}
			} else {
				if (WcIsZENKAKU(cv->out_buf[op - 1])) {
					vbackchar(2);
					op -=1;
				} else {
					vbackchar(1);
					op--;
				}
				cv->out_point = op;
				if (BuffYes() && is_bs(c))  {
					if (!(cv->out_mode[op] & OMODE_MASK))
						chhstr(cv->Halpha, cv->out_buf,
						       cv->out_mode, op);
					cv->ha_point = getipos(op);
				}
			}
			continue;
		}
		if (WcIsZENKAKU(c1)) {
			if (op >= 1 && (cc = addten(op - 1, c1)) != 0) {
				c1 = cc;
				vbackchar(2);
				op -= 1;
				cv->out_point = op;
				len = cv->out_mode[op] + GetMojilen(c2);
				c2 = SetMojilen(len);
			} else {
				if (!GetMojilen(c2) && (mode == MODE_CODE)) 
				  c2 = SetMojilen(1);
			}
			if (mode == MODE_ZKATA) {
				c1 = sj_tokata(c1);
			} else if (mode == MODE_HKATA) {
				c1 = sj_zen2han(c1);
			}
		} else { 
			if ((c2 & ERRCODE) && is_bs(c))
				cv->Halpha[cv->ha_point++] = c1;
		        if (WcIsKANJI(c1)) {
				c2 = SetMojilen(1);
			} else if (iswcntrl(c1) && !(c2 & ERRCODE)) {
				if (BuffYes())
					cv->ha_point--;
				sj3_rkreset();
				i = 0;
				break;
			} else if (mode & MODE_ZEN) {
				c1 = sj_han2zen(c1);
				if (mode == MODE_ZKATA)
					c1 = sj_tokata(c1);
				if (op >=1 && (cc = addten(op-1, c1)) != 0){
					c1 = cc;
					vbackchar(2);
					op --;
					cv->out_point = op;
					len = cv->out_mode[op] 
						      + GetMojilen(c2);
					c2 = SetMojilen(len);
				}
			}
		}
		if ((unsigned int)cv->out_point + os_point >= BUFFLENGTH * 2 - 2) {
			beep();
			op = cv->out_point;
			break;
		}
		if (WcIsZENKAKU(c1)) {
			cv->out_buf[op] = c1;
			cv->in_mode[op] = mode;
			cv->out_mode[op++] = GetMojilen(c2);
			if (mode == MODE_HKATA || WcIsKANA(cv->out_buf[op - 1])) {
				cv->out_buf[op - 1] = c1 >> 8;
				cv->out_buf[op] = c1 &MASK;
				cv->in_mode[op] = mode;
				cv->out_mode[op++] = 0;
				sj3_rkreset();
			}
		} else {
			cv->out_buf[op] = c1;
			cv->in_mode[op] = mode;
			cv->out_mode[op++] = GetMojilen(c2);
		}
		if (!BuffYes() && !(c2 & ERRCODE)) {
			cv->out_buf[op] = '\0';
			master_out(cv->out_buf, op);
			sj3_rkreset();
			op = 0;
		}
	}
	cv->out_buf[op] = '\0';
	cv->Halpha[cv->ha_point] = '\0';
	if (is_bs(c) && op <= 0 && os_point <= 0) {
		ResetHenkan();
		if (BuffYes()) {
			SetCflag();
			buffers_clear();
		}
	}
	else if ((len = op - cv->out_point) > 0) {
		p = &cv->out_buf[cv->out_point];
		if (BuffYes() && IsCflag ()) {
			Csave ();
			ClearCflag ();
		}
		if (mode & MODE_ZEN) {
			if (!BuffYes() && len == 2)
				Csave();
			vprintU(p, 1);
		} else {
			if (!BuffYes() && len == 1)
				Csave();
			vprintU(p, 0);
		}
	}
	cv->out_point = op;
	return(i);
}

move_cur(kval)
int kval;
{
	Conversion *cv;
	int i;

	cv = GetConversion();
	switch(kval) {
	case KEY_TOP:
		cv->out_point = 0;
		break;
	case KEY_END:
		cv->out_point = cv->out_epoint;
		cv->ha_point = cv->ha_epoint;
		break;
	case KEY_LEFT:
		if ((i = cv->out_point) > 0) {
			i--;
			if (i > 0 && (WcIsZENKAKU(cv->out_buf[i])))
				i--;
			cv->out_point = i;
		}
		break;
	case KEY_RIGHT:
		if ((i = cv->out_point) < cv->out_epoint) {
			i++;
			if (i < cv->out_epoint && (WcIsZENKAKU(cv->out_buf[i])))
				i++;
			cv->out_point = i;
			if (cv->out_point >= cv->out_epoint)
				cv->ha_point = cv->ha_epoint;
		}
		break;
	}
	if (cv->out_point != cv->Vlen) {
		Cgoto(cv->out_point);
		if (cv->out_point == cv->out_epoint) {
			os_point = 0;
			hs_point = 0;
		}
	}
}

addten(pos, c)
unsigned short pos, c;
{
	unsigned short prefix;
	Conversion *cv;

	cv = GetConversion();
	prefix = cv->out_buf[pos];
	return(sj_addten(prefix, c));
}

getipos(pos)
unsigned short pos;
{
	Conversion *cv;

	cv = GetConversion();
	return(getipos2(cv->out_mode, pos));
}

getipos2(omode, pos)
unsigned short *omode;
unsigned short pos;
{
	unsigned short ipos;
	int i;

	ipos = 0;
	for (i = 0; i < (unsigned int)pos; i++) {
		ipos += omode[i] & OMODE_MASK;
	}
	return(ipos);
}

sj_rkconv(s1, s2, len)
wchar16_t *s1, *s2;
int len;
{
	int i, rlen;
	unsigned int *kp;
	int res, klen;
	wchar16_t roma[SHORTBUFFSIZE + 1];
	unsigned int kstr[MAXLLEN];

	sj3_rkebell(0);
	klen = 0;
	res = 0;
	while (len > 0) {
		if (len + res > SHORTBUFFSIZE) {
			i = SHORTBUFFSIZE - res;
			len -= i;
		} else {
			i = len;
			len = 0;
		}
		Strncpy(&roma[res], s2, i);
		s2 += i;
		roma[res + i] = '\0';
		rlen = sj3_rkconv2(roma, kstr, i + res - 1);
		res = wslen(roma);
		if (rlen > 0) {
			kp = kstr;
			while (*kp != 0) {
				*s1++ = (wchar16_t) *kp++;
				klen++;
			}
		}
	}
	if (res > 0) {
		roma[res] = 0x0a;
		roma[res + 1] = '\0';
		rlen = sj3_rkconv2(roma, kstr, res);
		if (rlen > 0) {
			kp = kstr;
			while ((*kp & RKMASK) != 0x0a && *kp != 0) {
				*s1++ = (wchar16_t) *kp++;
				klen++;
			}
		} else {
			klen += res;
			roma[res] = '\0';
			wscpy(s1, roma);
			s1 += res;
		}
	}
	*s1 = '\0';
	sj3_rkebell(rkerrbell);
	return(klen);
}

sj_rkconv2(s1, s2, omode, len)
wchar16_t *s1, *s2;
unsigned short *omode;
int len;
{
	int i, rlen;
	unsigned int *kp;
	int res, klen, mlen;
	unsigned short cc, c, prefix;
	wchar16_t roma[SHORTBUFFSIZE + 1];
	unsigned int kstr[BUFFLENGTH];

	sj3_rkebell(0);
	klen = 0;
	res = 0;
	prefix = 0;
	while (len > 0) {
		if (len + res > SHORTBUFFSIZE) {
			i = SHORTBUFFSIZE - res;
			len -= i;
		} else {
			i = len;
			len = 0;
		}
		Strncpy(&roma[res], s2, i);
		s2 += i;
		roma[res + i] = '\0';
		rlen = sj3_rkconv2(roma, kstr, i + res - 1);
/*
 * Remove warning.
 * Patched by Hidekazu Kuroki(hidekazu@cs.titech.ac.jp)		1996/8/10
 */
		res = strlen((char *)roma);
		if (rlen <= 0)
			continue;
		kp = kstr;
		while ((c = *kp & RKMASK) != 0) {
			if (c & RKZEN) {
				if ((cc = sj_addten(prefix, c)) != 0) {
					omode -= 1;
					s1 -= 1;
					*omode += GetMojilen(*kp);
					c = cc;
					prefix = 0;
				} else {
					*omode = GetMojilen(*kp);
					klen += 1;
					prefix = c;
				}
				*s1++ = (wchar16_t) c;
				omode++;
				kp++;
			} else {
				mlen = GetMojilen(*kp);
				*kp = sj_han2zen(c) + SetMojilen(mlen);
			}
		}
	}
	if (res > 0) {
		roma[res] = 0x0a;
		roma[res + 1] = '\0';
		rlen = sj3_rkconv2(roma, kstr, res);
		if (rlen > 0) {
			kp = kstr;
			while ((c = *kp & RKMASK) != 0x0a && c != 0) {
				if (*kp & RKZEN) {
					if ((cc = sj_addten(prefix, c)) != 0) {
						omode -= 1;
						s1 -= 1;
						*omode += GetMojilen(*kp);
						c = cc;
						prefix = 0;
					} else {
						*omode = GetMojilen(*kp);
						klen += 1;
						prefix = c;
					}
					*s1++ = (wchar16_t) c;
					omode++;
					kp++;
				} else {
					mlen = GetMojilen(*kp);
					*kp = sj_han2zen(c) + SetMojilen(mlen);
				}
			}
		} else {
			for (i = 0; i < res; i++) {
			        if (WcIsKANJI(roma[i])) {
					c = roma[i];
					mlen = 1;
				} else {
					c = sj_han2zen(roma[i]);
					mlen = 1;
				}
				if ((cc = sj_addten(prefix, c)) != 0) {
					s1 -= 1;
					omode -= 1;
					*omode += mlen;
					c = cc;
					prefix = 0;
				} else {
					prefix = c;
					klen += 2;
					*omode = mlen;
				}
				*s1++ = c;
				omode++;
			}
		}
	}
	*s1 = '\0';
	sj3_rkebell(rkerrbell);
	return(klen);
}



exec_romaji2(c, hbuf, kbuf, omode, cur, hcur)
wchar16_t	c, *hbuf, *kbuf;
unsigned short *omode;
int cur, hcur;
{
	unsigned short c1, cc;
	unsigned int c2;
	int i;
	int ocur, len, retv;
	unsigned int rkstr[MAXLLEN], *rkp;

	retv = 1;
	ocur = cur;
	if (hcur >= 0 && !is_bs(c))
		hbuf[hcur++] = c;
	sj3_rkconvc(c, rkstr);
		
	rkp = rkstr;
	while(*rkp != RKEND) {
		c1 = ((c2 = *rkp++) & RKMASK);
		if (c2 == ERRBEL) {
			if (c != 0)
				beep();
			hcur--;
			break;
		}
		if (is_bs(c1)) {
			if (cur <= 0) {
				beep();
			} else {
				vbackchar(2);
				cur -= 1;
				ocur = cur;
			}
			if (hcur >= 0 && is_bs(c)) {
				for (i = cur; i > 0 ; i--){
				    if (omode[i] & OMODE_MASK)
					break;
				}
				hcur -= omode[i];
				while (i < cur) {
				    len = sj_zentohan(&hbuf[hcur],&kbuf[i],1);
				    omode[i] = len;
				    hcur += len;
				    i ++;
				}
			}
			continue;
		}
		if (!WcIsZENKAKU(c1)) {
			if (hcur >= 0 && (c2 & ERRCODE) && is_bs(c))
				hbuf[hcur++] = c1;
 		        if (c1 == 0) {
				hcur--;
				break;
			} else {
				if (c2 & ERRCODE)
					retv = 0;
				c1 = sj_han2zen(c1);
			}
		}
		if (cur >= 1) {
			cc = kbuf[cur - 1];
			if ((cc = sj_addten(cc, c1)) != 0){
				c1 = cc;
				vbackchar(2);
				cur --;
				ocur = cur;
				if (hcur >= 0) {
					len = omode[cur] + GetMojilen(c2);
					c2 = SetMojilen(len);
				}
			}
		}
		kbuf[cur] = c1;
		if (hcur >= 0) {
			omode[cur] = GetMojilen(c2);
		}
		cur ++;
	}
	kbuf[cur] = '\0';
	if (hcur >= 0)
		hbuf[hcur] = '\0';
	if (cur > ocur) 
		vprintU(&kbuf[ocur], 1);
	return(retv);
}


