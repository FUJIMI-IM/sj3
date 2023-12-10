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
 * $SonyRCSfile: libif.c,v $  
 * $SonyRevision: 1.3 $ 
 * $SonyDate: 1996/02/23 11:04:02 $
 */


#include "sj_sysvdef.h"
#ifdef SVR4
#include <sys/euc.h>
#endif
#include "wchar16.h"
#include <locale.h>
#include "kctype.h"
#include "common.h"


extern        char    user_name[];
extern        char    *cur_serv, **serv_list;
extern        char    **dict_list;
extern        int     dict_num;
char  *chmyhname();

extern        int     current_locale;
int sjislen(), euclen();

int (*len_func[3])() = {0, sjislen, euclen};


#define YOMILEN 32
#define KLEN    64





SJ2_henkan (yomiout, bun, kanjiout, kanjilen)
struct bunsetu_sj3	*bun;
wchar16_t		*yomiout, *kanjiout;
int		kanjilen;
{
	int retv;
	SJ3_BUNSETU sbun[BUFFLENGTH];
	int i, sentou, saigo;
	unsigned char yomi[BUFFLENGTH * 3];
	unsigned char kanji[BUFFLENGTH * 6];

	(void) wcstombs((char *) yomi, yomiout, BUFFLENGTH*3);
	if (current_locale == LC_CTYPE_SHIFTJIS) {
		retv = sj3_getkan(yomi, sbun, kanji, BUFFLENGTH * 6);
	} else {
		retv = sj3_getkan_euc(yomi, sbun, kanji, BUFFLENGTH * 6);
	}

	if (retv <= 0) return(retv);
	(void) mbstowcs(kanjiout, (char *)kanji, kanjilen);

	for(i = 0; i < retv; i++) {
		sentou = (*len_func[current_locale])(yomi, sbun[i].srcstr - yomi);
		saigo  = (*len_func[current_locale])(sbun[i].srcstr, sbun[i].srclen);
		bun[i].srclen = saigo;
		bun[i].srcstr = yomiout + sentou;
		sentou = (*len_func[current_locale])(kanji, sbun[i].deststr - kanji);
		saigo  = (*len_func[current_locale])(sbun[i].deststr, sbun[i].destlen);
		bun[i].destlen = saigo;
                bun[i].deststr = kanjiout + sentou;
		(void) memcpy(&(bun[i].dcid), &(sbun[i].dcid), sizeof(sbun[i].dcid));
	}

	return(retv);
}
int
sjislen(str, max)
unsigned char *str;
int max;
{
	int len = 0, bytes = 1;

	if (!max) return 0;
	while ((bytes <= max) && *str) {
		if (issjis1(*str)) {
			str++;
			bytes++;
			if (!*str) break;
			len++;
		} else {
			len++;
		}
		str++;
		bytes++;
	}
	return len;
}

euclen(str, max)
unsigned char *str;
int max;
{
	int len = 0, bytes = 1;

	if (!max) return 0;
	while ((bytes <= max) && *str) {
		if (*str == SS3) {
			str++;
			bytes++;
			if (!*str) break;
			str++;
			bytes++;
			if (!*str) break;
			len++;
		} else	if ((*str == SS2) || (*str & 0x80)) {
			str++;
			bytes++;
			if (!*str) break;
			len++;
		} else {
			len++;
		}
		str++;
		bytes++;
	}
	return len;
}



SJ2_getdouon (s, d, n, choice, reconv)
wchar16_t		*s;
struct douon_sj3	*d;
int		n;
int		choice;
int		reconv;
{
	int i, ret;
	SJ3_DOUON d_sjis[DOUON_N];
	unsigned char yomi[BUFFLENGTH * 3];

	(void) wcstombs((char *) yomi, s, BUFFLENGTH*3);

	if (current_locale == LC_CTYPE_SHIFTJIS) {
		ret = sj3_getdouon(yomi, d_sjis);
	} else {
		ret = sj3_getdouon_euc(yomi, d_sjis);
	}		
	for (i = 0; i < ret; i++) {
		(void) memcpy(&(d[i].dcid), 
			      &(d_sjis[i].dcid), SJ3_WORD_ID_SIZE);
		d[i].wlen = (int) mbstowcs(d[i].ddata, (char *)d_sjis[i].ddata, 
					   SJ3_BUNSETU_KANJI*2);
		d[i].dlen = wcbyte(d[i].ddata);
 	    	d[i].valid = (char)1;
	}
 	if (ret < DOUON_N) {
 		(void) memset(&(d[ret].dcid), 0, SJ3_WORD_ID_SIZE);
 		d[ret].wlen = wslen(s);
 		for (i = 0; i < d[ret].wlen; i++) {
 			d[ret].ddata[i] = sj_tokata(s[i]);
 		}
 		d[ret].ddata[i] = (wchar_t) 0;
 		d[ret].dlen = wcbyte(s);
 	    	d[ret].valid = (char)0;
 	}
 	return(ret + 1);
}

SJ2_study (dcid)
struct studyrec	*dcid;
{
	sj3_gakusyuu(dcid);
}

SJ2_clstudy (yomi1, yomi2, dcid)
wchar16_t		*yomi1, *yomi2;
struct studyrec	*dcid;
{
	unsigned char y1[BUFFLENGTH*2];
	unsigned char y2[BUFFLENGTH*2];

	(void) wcstombs((char *) y1, yomi1, BUFFLENGTH*2);
	(void) wcstombs((char *) y2, yomi2, BUFFLENGTH*2);

	if (current_locale == LC_CTYPE_SHIFTJIS) {
		sj3_gakusyuu2(y1, y2, dcid);
	} else {
		sj3_gakusyuu2_euc(y1, y2, dcid);
	}
}

SJ2_toroku (yomi, kanji, hinshi)
wchar16_t	*yomi, *kanji;
int	hinshi;
{
	unsigned char  y[(YOMILEN+1)*3];
        unsigned char  k[(KLEN+1)*3];
	int ret;

	(void)wcstombs((char *) y, yomi, (YOMILEN+1)*3);
	(void)wcstombs((char *) k, kanji, (KLEN+1)*3);
	if (current_locale == LC_CTYPE_SHIFTJIS) {
		ret = sj3_touroku (y, k, hinshi);
	} else {
		ret = sj3_touroku_euc (y, k, hinshi);
	}
	return(ret);
}

SJ2_syoukyo (yomi, kanji, hinshi)
wchar16_t	*yomi, *kanji;
int	hinshi;
{
	unsigned char  y[(YOMILEN+1)*3];
        unsigned char  k[(KLEN+1)*3];
	int ret;

	(void)wcstombs((char *) y, yomi, (YOMILEN+1)*3);
	(void)wcstombs((char *) k, kanji, (KLEN+1)*3);
	if (current_locale == LC_CTYPE_SHIFTJIS) {
		ret = sj3_syoukyo (y, k, hinshi);
	} else {
		ret = sj3_syoukyo_euc (y, k, hinshi);
	}
	return(ret);
}

sj_check_error (err)
int	err;
{
	if (err & SJ3_SERVER_DEAD)
		SJ_warnning(WCServer_Dead);
	if (err & SJ3_ALREADY_CONNECTED)
		SJ_warnning(WCConnected);
	if (err & SJ3_CONNECT_ERROR)
		SJ_warnning(WCNconnect);
	if (err & SJ3_CANNOT_OPEN_MDICT)
		SJ_warnning(WCNopen_Mdic);
	if (err & SJ3_CANNOT_MAKE_UDIR)
		SJ_warnning(WCNmake_Udir);
	if (err & SJ3_CANNOT_MAKE_UDICT)
		SJ_warnning(WCNmake_Udic);
	if (err & SJ3_CANNOT_OPEN_UDICT)
		SJ_warnning(WCNopen_Udic);
	if (err & SJ3_CANNOT_MAKE_STUDY)
		SJ_warnning(WCNmake_Study);
	if (err & SJ3_CANNOT_OPEN_STUDY)
		SJ_warnning(WCNopen_Study);
}

SJ2_reconnect ()
{
	int err;

        sj3_close(); 
	if (dict_num && dict_list) 
	  err = sj3_open_with_list(chmyhname(cur_serv), user_name, dict_num, dict_list, NULL, NULL);
	else 
	  err = sj3_open(chmyhname(cur_serv), user_name);
        return(err);
}


SJ2_henkan_end ()
{
	sj3_close ();
}

SJ2_henkan_init ()
{
        int i, err;
	int *err_dict = NULL;
	int  err_num = 0;

        sj3_set_timeout(5);

	if (dict_num && dict_list) 
	  err = sj3_open_with_list(chmyhname(cur_serv), user_name, dict_num, 
				   dict_list, &err_num, &err_dict);
	else
	  err = sj3_open(chmyhname(cur_serv), user_name);
        i = 0;
	while (err && i < MAXSERVER - 1 && serv_list[i + 1] != NULL) {
		if (dict_num && dict_list) 
		  err = sj3_open_with_list(chmyhname(serv_list[++i]), user_name,
					   dict_num, dict_list, &err_num, &err_dict);
		else
		  err = sj3_open(chmyhname(cur_serv), user_name);
	}
	if (err) {
		printf("Warning ");	
		sj_check_error(err);
		printf("\n\r");
		set_dcflag(1);
		return(1);
	} else if (err_num && err_dict) {
		printf("Warning "); 
		for( i = 0; i < err_num; i++) {
			printf("%s ", dict_list[err_dict[i]]);
		}
		printf(" files can't open\n");
                cur_serv = serv_list[i];
                return (0);
	} else {
                cur_serv = serv_list[i];
                return (0);
        }
}

sj3_autocon() 
{
        int i, err;
  
	if (dict_num && dict_list) {
		if (!(err = sj3_open_with_list(chmyhname(cur_serv), user_name, dict_num,
					       dict_list, NULL, NULL)))
		    return(0);
	} else {
		if (!(err = sj3_open(chmyhname(cur_serv), user_name)))
		    return(0);
	}
        i = 0;
        while (serv_list[i] != NULL && i < MAXSERVER) {
                if (strcmp(cur_serv, serv_list[i]) != 0) {
			if (dict_num && dict_list) {
				err = sj3_open_with_list(chmyhname(serv_list[i]), user_name,
							 dict_num, dict_list, NULL, NULL);
			} else {
				err = sj3_open(chmyhname(cur_serv), user_name);
			}
                        if (!err) {
                                cur_serv = serv_list[i];
                                break;
                        }
                }
                i++;
        }
        return(err);
}
