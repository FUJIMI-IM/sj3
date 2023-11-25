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
 * $SonyRCSfile: stat_conv.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:28 $
 */



#include "common.h"
#include "key.h"
#include "rk.h"



extern int	keyvalue;

int rkerrbell = 0;
static int	delkey = -1;
static int	bskey;

#define SPAN_FLUSH		0
#define SPAN_END		1

save_span (mode)
register int	mode;
{
	register Conversion	*cv;

	cv = GetConversion ();
	switch (mode) {
	case SPAN_FLUSH:
		cv->span_point = 0;
		cv->ha_point = 0;
		cv->out_point = 0;
		save_span_p ();
		break;
	case SPAN_END:
		if (cv->out_point < cv->out_epoint) {
			Clear_EOL();
			cv->out_point = cv->out_epoint;
                        cv->ha_point = cv->ha_epoint;
		}
		break;
	default:
		break;
	}
}

save_span_p ()
{
	register Conversion	*cv;

	cv = GetConversion ();
	cv->span_p[cv->span_point] = &(cv->out_buf[cv->out_point]);
	cv->span_l[cv->span_point] = 0;
}

isspaned ()
{
	register Conversion	*cv;

	cv = GetConversion ();
	if (cv->out_buf[0] != '\0')
		return (1);
	else
		return (0);
}



buffers_clear ()
{
	register Conversion	*cv;

	cv = GetConversion ();
	pointer_clear ();
	if (flush_conversion)
		cv->CurBun = -1;
	else
		cv->CurBun = 0;
}

pointer_clear ()
{
	register Conversion	*cv;

	cv = GetConversion ();
	cv->ha_point = cv->ha_epoint = 0;
	*(cv->Halpha) = '\0';
	os_point = 0;
	cv->out_point = cv->out_epoint = 0;
	*(cv->out_buf) = '\0';
	cv->Kanji = 0;
        save_span (SPAN_FLUSH);
	sj3_rkclear();
}

typedef struct	_SaveSet {
	wchar16_t	out_buf[BUFFLENGTH * 2];
	u_short out_mode[BUFFLENGTH * 2];
	u_short out_point;
	u_short ha_point;
	wchar16_t	Halpha[BUFFLENGTH];
	u_char	in_mode[BUFFLENGTH * 2];
} SaveSet;

static SaveSet		save_set0;

StoreSaveSet ()
{
	register Conversion	*cv;
	register SaveSet	*sv;

	cv = GetConversion ();
	if (cv->out_point) {
                sv = &save_set0;
		sv->out_point = cv->out_point;
		sv->ha_point = cv->ha_point;
		memcpy (sv->out_mode, cv->out_mode, 
			cv->out_point * sizeof(u_short));
		memcpy (sv->in_mode, cv->in_mode,
			cv->out_point * sizeof(u_char));
		wscpy (sv->Halpha, cv->Halpha);
		wscpy (sv->out_buf, cv->out_buf);
	}
}

RestoreSaveSet ()
{
	register Conversion	*cv;
	register SaveSet	*sv;

        sv = &save_set0;
	if (sv->out_point) {
		cv = GetConversion ();
		cv->out_point = sv->out_point;
		cv->ha_point = sv->ha_point;
		memcpy (cv->out_mode, sv->out_mode, 
			sv->out_point * sizeof(u_short));
		memcpy (cv->in_mode, sv->in_mode, 
			sv->out_point * sizeof(u_char));
		wscpy (cv->Halpha, sv->Halpha);
		wscpy (cv->out_buf, sv->out_buf);
	}
}

history_empty ()
{
	SaveSet		*sv;

        sv = &save_set0;
	if (sv->out_point)
		return (0);
	else
		return (1);
}

History ()
{
	Conversion	*cv;
	SaveSet		*sv;

        sv = &save_set0;
	if (sv->out_point) {
		RestoreSaveSet ();
		cv = GetConversion ();
		if (flush_conversion)
			cv->CurBun = -1;
		else
			cv->CurBun = 0;
	}
}

ClearBuffer ()
{
	Clear_EOL();
	Ccheck ();
	Cclear (1);
	all_set_up ();
}



SetCflag ()
{
	register Conversion	*cv;

	cv = GetConversion ();
	cv->Cflag = 1;
}

ClearCflag ()
{
	register Conversion	*cv;

	cv = GetConversion ();
	cv->Cflag = 0;
}

IsCflag ()
{
	register Conversion	*cv;

	cv = GetConversion ();
	return (cv->Cflag);
}

exec_kettei ()
{
	register Conversion	*cv;
	int		len;
	wchar16_t		kanji[BUFFLENGTH * 2];

	cv = GetConversion ();
	save_span(SPAN_END);
	if (cv->Kanji != 0) {
		len = Bkanji (kanji);
		Ccheck ();
		Cclear (1);
		master_write (kanji, len);
		if (cv->word_regist) {
			cv->word_regist = 0;
		}
		else {
			if (is_bstudy ()) {
				cl_gakusyuu ();
			}
		}
	} else if (isspaned ()) {
		if (BuffYes ()) {
			Ccheck ();
			Cclear (1);
		}
		if (!BuffYes ())
			vbackchar (cv->out_point);
		if (cv->out_point) 
			master_write (cv->out_buf, cv->out_point);
                
		if (BuffYes ())
			master_flush ();
		StoreSaveSet ();
	}
	buffers_clear ();
	ResetHenkan ();
	SetCflag ();
}

set_rkebell(word)
struct wordent  word[];
{
	if (match (word[1].word_str, WCOn))
		rkerrbell = 1;
	if (match (word[1].word_str, WCOff))
		rkerrbell = 0;
}



ChangeBuffMode ()
{
	register Conversion	*cv;

	cv = GetConversion ();
	if (cv->BuffMode == BUFF) {
		sj3_rkebell(0);
		cv->BuffMode = UNBUFF;
	} else {
		sj3_rkebell(rkerrbell);
		cv->BuffMode = BUFF;
	}
}

BuffYes ()
{
	register Conversion	*cv;

	cv = GetConversion ();
	if (cv->BuffMode == BUFF)
		return (1);
	else
		return (0);
}



#define STAT_HENKAN	1
#define STAT_MUHENKAN	2

IsHenkan ()
{
	Conversion	*cv;

	cv = GetConversion ();
	return (cv->henkanflag);
}

IsMuhenkan ()
{
	Conversion	*cv;

	cv = GetConversion ();
	return (cv->henkanflag & STAT_MUHENKAN);
}

SetMuhenkan ()
{
	Conversion	*cv;

	cv = GetConversion ();
	cv->henkanflag |= STAT_MUHENKAN;
}

SetHenkan ()
{
	Conversion	*cv;

	cv = GetConversion ();
	cv->henkanflag = STAT_HENKAN;
}

ResetHenkan ()
{
	Conversion	*cv;

	cv = GetConversion ();
	cv->henkanflag = 0;
}

all_set_up ()
{
	buffers_clear ();	
	SetCflag ();		
	ResetHenkan ();
}


static int	convertflag;	
static int	small;

stat_conv_on ()
{
	convertflag = 1;	
}

stat_init ()
{
	stat_conv_on ();
	all_set_up ();
	disp_mode ();		
	small = 0;
}

get_pseq ()
{
	return (write_pseq (1));
}

stat_conv ()
{
	register Conversion	*cv;
	extern int	allways_buff;	
	register int	inc;	
        register int    i;
        u_short         svlen;  

	cv = GetConversion ();
	cv->word_regist = 0;
	while (convertflag) {
		if ((inc = inkey ()) == EOF)
			break;

		
		
		
		cv = GetConversion ();
		if (allways_buff)
			cv->BuffMode = BUFF;
more:
		switch (keyvalue) {
		

		case KEY_NORMAL:
			small = 0;
			if (inc == EOF)
				break;
			if (IsHenkan ()) {
				if (is_bsdel (inc)) {
					Bdelete ();
					break;
				} else if (inc < ' ' && inc != '\n' && 
					   inc != '\r' && inc != '\t' && 
					   !IsESC (inc) && !isintr (inc)){
					beep ();
					break;
				} else {
					exec_kettei ();
				}
			}
                
                        save_obuf();
                        svlen = cv->Vlen;
                        while (exec_romaji(inc)) {
				if (os_point > 0 && svlen != cv->Vlen) {
                                        i = svlen - cv->Vlen;
                                        svlen = cv->Vlen;
                                        vprintU(osave_buf, 1);
                                        if (i > 0)
                                                Clear_EOL();
                                        Cgoto(svlen);
				}
                                inc = inkey();
                                if (keyvalue != KEY_NORMAL &&
				    (exec_romaji(0)==0||keyvalue == KEY_CONV)){
                                        restore_obuf();
                                        goto more;
				}
                        }
                        restore_obuf();
                        if (is_del(inc) && (!IsCflag() && isspaned())) {
				if (cv->out_point < cv->out_epoint) {
                                        move_cur(KEY_RIGHT);
                                        inc = bskey;
                                        goto more;
	                        }
			} else
                                exec_ctrl(inc);
			break;
		case KEY_KETTEI:
			small = 0;
			if (!IsCflag ()) {
				exec_kettei ();
			} else if (cv->Imode != MODE_HALPHA || BuffYes ()) {
				if (cv->Imode != MODE_HALPHA)
					exec_mode (KEY_HALPHA);
				if (BuffYes ())
					ChangeBuffMode ();
				disp_mode ();
			}
			break;
		case KEY_CONV:
			small = 0;
			if (!no_flush)
				exec_kettei ();
			convertflag = 0;
			break;
		case KEY_CANCEL:
			small = 0;
			if (!IsCflag ())
				ClearBuffer ();
                        else if ((inc = get_pseq ()) != 0)  
                                exec_ctrl(inc);
			break;
		case KEY_RECONV:
			small = 0;
			if (IsCflag ()) {
				if (history_empty ()) {
                                        if ((inc = get_pseq ()) != 0)
                                                exec_ctrl(inc);
                                        break;
				} else {
					History ();
					Csave ();
					ClearCflag ();
				}
			} else {
				if (IsHenkan ()) {
					History ();
					ResetHenkan ();
				} else {
					beep ();
					break;
					
				}
			}
			exec_henkan ();
			SetHenkan ();
			break;
		case KEY_HENKAN:
			small = 0;
			if (!IsCflag ()) {
				if (IsMuhenkan ()) {
					exec_henkan ();
					SetHenkan ();
				}
				if (IsHenkan ()) {
					exec_douon ();
				} else {
					save_span (SPAN_END);
					Ccheck ();
					StoreSaveSet ();
					exec_henkan ();
					SetHenkan ();
				}
			} else if ((inc = write_pseq (3)) >= ' ' && inc <= '~')
				exec_ctrl(inc);
			else if (cv->Imode != MODE_ZHIRA || !BuffYes ()) {
				if (cv->Imode != MODE_ZHIRA) {
					exec_mode (KEY_ZHIRA);
				}
				if (!BuffYes ()) {
					ChangeBuffMode ();
				}
				disp_mode ();
			}
			break;
		case KEY_MUHENKAN:
			small = 0;
			if (flush_conversion && !IsCflag () && 
			    cv->CurBun == -1) {
				if (IsHenkan ()) {
					exec_muhenkan (0);
					SetHenkan ();
				} else {
					save_span (SPAN_END);
					Ccheck ();
					exec_muhenkan (1);
					SetHenkan ();
					SetMuhenkan ();
				}
				break;
			}
		case KEY_TOGGLE:
		case KEY_ZHIRA:
		case KEY_HALPHA:
		case KEY_ZALPHA:
		case KEY_HKATA:
		case KEY_ZKATA:
		case KEY_CODE:
			small = 0;
			if (cv->CurBun == -1)
				exec_kettei ();
			else if (IsHenkan ()) {
				Bchange (keyvalue);
				break;
			}
			if (keyvalue == KEY_TOGGLE)
				keyvalue = toggle_mode (cv->Imode);
			exec_mode (keyvalue);
			disp_mode ();
			break;
		case KEY_WRAP:
			small = 0;
			if (IsHenkan ())
				wrap_douon (0);
			else if ((inc = get_pseq ()) != 0)
				exec_ctrl(inc);
			break;
		case KEY_WRAPBACK:
			small = 0;
			if (IsHenkan ())
				wrap_douon (1);
			else if ((inc = get_pseq ()) != 0)
				exec_ctrl(inc);
			break;
		case KEY_LEFT:
		case KEY_RIGHT:
		case KEY_UP:
		case KEY_DOWN:
			if (IsHenkan ()) {
				switch (keyvalue) {
				case KEY_LEFT:
					Bleft ();
					small = 0;
					break;
				case KEY_RIGHT:
					Bright ();
					small = 0;
					break;
				case KEY_UP:
					small = Bbig ();
					break;
				case KEY_DOWN:
					small = Bsmall (small);
					break;
				}
			} else if (cv->Imode == MODE_CODE) {
				small = 0;
				if (BuffYes () && IsCflag ()) {
					Csave ();
					ClearCflag ();
				}
				kigou ();
				if (flush_conversion)
					exec_kettei ();
			} else if (!IsCflag() && (keyvalue == KEY_LEFT ||
						  keyvalue == KEY_RIGHT)) {
				small = 0;
				move_cur(keyvalue);
                        } else if (!BuffYes() && cv->Imode == MODE_HALPHA) {
                                small = 0;
                                write_pseq(0);

			} else if (IsCflag() && (inc = get_pseq ()) != 0) {
				small = 0;
				exec_ctrl(inc);
			}
			break;
		case KEY_TOP:
		case KEY_END:
			if (IsHenkan ()) {
				switch (keyvalue) {
				case KEY_TOP:
					Btop ();
					small = 0;
					break;
				case KEY_END:
					Blast ();
					small = 0;
					break;
				}
			} else if (!IsCflag()) {
				small = 0;
				move_cur(keyvalue);
			} else if ((inc = get_pseq ()) != 0) {
				small = 0;
				exec_ctrl(inc);
			}
			break;
		case KEY_EDIT:
		case KEY_TOROKU:
		case KEY_SYOUKYO:
			small = 0;
			if (IsCflag () && (inc = get_pseq ()) != 0)
				exec_ctrl(inc);
		case KEY_ETC:
		case KEY_HELP:
		case KEY_SJRC:
		case KEY_RECONNECT:
			small = 0;
                        i = exec_etc (keyvalue);
			disp_mode ();
                        if (i != KEY_NORMAL) {
                                keyvalue = i;
				goto more;
			}
			break;
		case KEY_BUFFER:
			small = 0;
			if (BuffYes ())
				exec_kettei ();
			ChangeBuffMode ();
			disp_mode ();
			break;
		case KEY_HA_KET:
			small = 0;
			if (!IsCflag ()) {
			    if (cv->Imode != MODE_HALPHA || BuffYes ()) {
				if (BuffYes ()) {
					exec_kettei ();
					ChangeBuffMode ();
				}
				if (cv->Imode != MODE_HALPHA)
					exec_mode (KEY_HALPHA);
				disp_mode ();
			    }
			} else if ((inc = get_pseq ()) != 0)
				exec_ctrl(inc);
			break;
		case KEY_FLUSH:
			small = 0;
			if (!IsCflag ()) {
				exec_kettei ();
			} else if ((inc = get_pseq ()) != 0)
				exec_ctrl(inc);
			break;
		default:
			small = 0;
			if ((inc = get_pseq ()) != 0)
				exec_ctrl(inc);
			break;
		}
		if (keyvalue == KEY_CONV) {
			if (!no_flush)
				exec_kettei ();
			convertflag = 0;
		}
	}
}


save_obuf()
{
        register Conversion     *cv;
        register int    i, cur;

        cv = GetConversion();

	if (cv->out_point < cv->out_epoint) {

                
                cur = cv->out_point;
		if (!(cv->out_mode[cur] & OMODE_MASK)) {
                        chhstr(cv->Halpha, cv->out_buf, cv->out_mode, cur);
                        cv->ha_epoint = wslen(cv->Halpha);
		}
                cv->ha_point = getipos(cur);
                hs_point = 0;
                for (i = cv->ha_point; i < cv->ha_epoint; i++)
                        hsave_buf[hs_point++] = cv->Halpha[i];
                hsave_buf[hs_point] = '\0';

                
                os_point = 0;
                for (i = cv->out_point; i < cv->out_epoint; i++) {
                        osave_buf[os_point] = cv->out_buf[i];
                        osave_mode[os_point] = cv->out_mode[i];
                        isave_mode[os_point] = cv->in_mode[i];
                        os_point++;
                }
                osave_buf[os_point] = '\0';
        }
}


restore_obuf()
{
        register Conversion     *cv;
        register int    i;

        cv = GetConversion();

        cv->out_epoint = cv->out_point;
        cv->ha_epoint = cv->ha_point;

        
	if (os_point > 0) {
		for (i = 0; i < (unsigned int)os_point; i++) {
                        cv->out_buf[cv->out_epoint] = osave_buf[i];
                        cv->out_mode[cv->out_epoint] = osave_mode[i];
                        cv->in_mode[cv->out_epoint] = isave_mode[i];
                        cv->out_epoint++;
		}
                cv->out_buf[cv->out_epoint] = '\0';
                for (i = 0; i < (unsigned int)hs_point; i++)
                        cv->Halpha[cv->ha_epoint++] = hsave_buf[i];
                cv->Halpha[cv->ha_epoint] = '\0';
        }
}

exec_ctrl(c)
register int c;
{
	register Conversion *cv;
	wchar16_t	s[3];
	extern wchar16_t   pars_seq[];
	
	cv = GetConversion ();

	*s = c;
	s[1] = 0;
	exec_kettei();
        if (c == ' ' && keyvalue == KEY_HENKAN && cv->Imode & MODE_ZEN) {
                s[0] = 0xa1a1;
                s[1] = 0;
                master_write (s, 1);
        } else
                master_write (s, 1);
 
        
        if (IsGoto (c) && (BuffYes() || cv->Imode != MODE_HALPHA)) {
                cv->BuffMode = UNBUFF;
                cv->Imode = MODE_HALPHA;
                disp_mode();
                sj3_rkebell(0);
        }

	if (IsTrap (c)) {	
		*s = inkey ();
		if (keyvalue != KEY_NORMAL)
			*s = pars_seq[0];
		master_write (s, 1);
	}
}

exec_mode (key)
register int	key;
{
	register Conversion	*cv;

	cv = GetConversion ();
	if (key == KEY_MUHENKAN)
		key = eval_muhenkan ();
        if (key == KEY_CODE) {
                if (cv->Imode == MODE_CODE)
                        ChangeCode();
                else
                        cv->Imode = MODE_CODE;
        } else
                cv->Imode = eval_mode(key);
}

toggle_mode (mod)
register int	mod;
{
	register int	value;

	switch (mod) {
	case MODE_ZHIRA:
		value = KEY_ZKATA;
		break;
	case MODE_ZKATA:
		value = KEY_HKATA;
		break;
	case MODE_HKATA:
		value = KEY_ZALPHA;
		break;
	case MODE_ZALPHA:
		value = KEY_HALPHA;
		break;
	default:
		value = KEY_ZHIRA;
		break;
	}
	return (value);
}

eval_mode (key)
register int	key;
{
	int		mod;

	switch (key) {
	case KEY_ZHIRA:
		mod = MODE_ZHIRA;
		break;
	case KEY_ZKATA:
		mod = MODE_ZKATA;
		break;
	case KEY_HKATA:
		mod = MODE_HKATA;
		break;
	case KEY_ZALPHA:
		mod = MODE_ZALPHA;
		break;
	default:
		mod = MODE_HALPHA;
		break;
	}
	return (mod);
}

IsESC (c)
wchar16_t c;
{
	if (c == ESC)
		return (1);
	else
		return (0);
}

Strncpy (d, s, len)
wchar16_t *d, *s;
int	len;
{
	if ((len > 0) && (s != (wchar16_t *) NULL))
	        (void)wsncpy(d, s, len);
}

is_bsdel (c)
register int	c;
{
	if (is_bs (c) || is_del (c))
		return (1);
	else
		return (0);
}

clear_del ()
{
	delkey = -1;
}

set_bs (c)
int	c;
{
	bskey = c;
}

set_del (c)
int	c;
{
	delkey = c;
}

is_bs (c)
register int	c;
{
	if (c == bskey)
		return (1);
	else
		return (0);
}

is_del (c)
register int	c;
{
	if (c == delkey)
		return (1);
	else
		return (0);
}

static Conversion	*saved_conversion;

SaveConversion ()
{
	saved_conversion = GetConversion ();
}

AnotherConversion ()
{
	if (saved_conversion == GetConversion ())
		return (0);
	else
		return (1);
}
