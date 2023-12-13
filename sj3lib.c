/*
 * Copyright (c) 1991-1994  Sony Corporation
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

#include "config.h"

#include <sys/file.h>
#include <sys/types.h>

#include <ctype.h>
#include <locale.h>
#include <pwd.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "const.h"
#include "sj3err.h"
#include "sj_hinsi.h"

#include "sj3lib.h"

extern int	sj3_error_number;
#define SYS_SJIS 0
#define SYS_EUC 1
#define SYS_NOTDEF -1
static int     _sys_code = SYS_NOTDEF;

char			*sj3_user_dir = "user";
static char		*path_delimiter = "/";
static SJ3_CLIENT_ENV	 client = { -1, 0, 0, 0, 0, {0, 0} };
static long		 mdicid = 0;
static long		 udicid = 0;
static int		 defuse = 0;
static long		*dicid_list = NULL;
static int		 dicid_num = 0;

static unsigned char buf1[YomiBufSize];
static unsigned char buf2[YomiBufSize];
static unsigned char kbuf[KanjiBufSize];

static int
set_sys_code(void)
{
	char *loc;

	loc = setlocale(LC_CTYPE, NULL);
	if (strcmp(loc, "ja_JP.SJIS") == 0)
		return SYS_SJIS;

	return SYS_EUC;
}

static int
make_dirs(char *path)
{
	char	 tmp[PathNameLen];
	char	*p;
	int	 i;

	for (p = path; *p; p++) {
		if (*p != *path_delimiter)
			continue;

		strncpy(tmp, path, (i = p - path));
		tmp[i] = '\0';
		if (sj3_access(&client, tmp, F_OK) != ERROR)
			continue;
		if (sj3_error_number == SJ3_ServerDown)
			return ERROR;

		if (sj3_make_directory(&client, tmp) == ERROR)
			return ERROR;
	}
	return 0;
}

int
sj3_open(char *host, char *user)
{
	char	 tmp[PathNameLen];
	char	*p;
	int	 err = SJ3_NORMAL_END;

	if (client.fd != -1)
		return SJ3_ALREADY_CONNECTED;

	sprintf(tmp, "%d.sj3lib", getpid());
	if (sj3_make_connection(&client, host, user, tmp) == ERROR) {
		if (sj3_error_number == SJ3_ServerDown)
			goto server_dead;
		client.fd = -1;
		return SJ3_CONNECT_ERROR;
	}

	if (client.stdy_size > SJ3_WORD_ID_SIZE) {
		sj3_erase_connection(&client);
		return SJ3_CONNECT_ERROR;
	}

	mdicid = sj3_open_dictionary(&client, MainDictionary, NULL);
	if (mdicid == 0) {
		if (sj3_error_number == SJ3_ServerDown)
			goto server_dead;
		err |= SJ3_CANNOT_OPEN_MDICT;
	}

	strcpy(tmp, sj3_user_dir);
	if (tmp[strlen(tmp) - 1] != *path_delimiter)
		strcat(tmp, path_delimiter);
	strcat(tmp, user);
	strcat(tmp, path_delimiter);
	if (make_dirs(tmp) == ERROR) {
		if (sj3_error_number == SJ3_ServerDown)
			goto server_dead;
		return (err | SJ3_CANNOT_MAKE_UDIR);
	}

	for (p = tmp; *p; p++)
		;
	strcpy(p, UserDictionary);
	if (sj3_access(&client, tmp, F_OK) == ERROR) {
		if (sj3_error_number == SJ3_ServerDown)
			goto server_dead;
		if (sj3_make_dict_file(&client, tmp, DefIdxLen,
		    DefSegLen, DefSegNum) == ERROR) {
			if (sj3_error_number == SJ3_ServerDown)
				goto server_dead;
			err |= SJ3_CANNOT_MAKE_UDICT;
		}
	}
	udicid = sj3_open_dictionary(&client, tmp, "");
	if (udicid == 0) {
		if (sj3_error_number == SJ3_ServerDown)
			goto server_dead;
		err |= SJ3_CANNOT_OPEN_UDICT;
	}

	strcpy(p, StudyFile);
	if (sj3_access(&client, tmp, F_OK) == ERROR) {
		if (sj3_error_number == SJ3_ServerDown)
			goto server_dead;
		if (sj3_make_study_file(&client, tmp, DefStyNum, DefClStep,
		    DefClLen) == ERROR) {
			if (sj3_error_number == SJ3_ServerDown)
				goto server_dead;
			err |= SJ3_CANNOT_MAKE_STUDY;
		}
	}
	if (sj3_open_study_file(&client, tmp, "") == ERROR) {
		if (sj3_error_number == SJ3_ServerDown)
			goto server_dead;
		err |= SJ3_CANNOT_OPEN_STUDY;
	}

	return err;

server_dead:
	mdicid = udicid = 0;
	return SJ3_SERVER_DEAD;
}

int
sj3_open_with_list(char *host, char *user, int dicts_num,
    char **dicts, int *error_num, int **error_index)
{
	char	 tmp[PathNameLen];
	char	*p;
	int	 err = SJ3_NORMAL_END;
	int	 i, err_id_num = 0;
	long	*dict_idx = NULL;

	if (client.fd != -1)
		return SJ3_ALREADY_CONNECTED;

	sprintf(tmp, "%d.sj3lib", getpid());
	if (sj3_make_connection(&client, host, user, tmp) == ERROR) {
		if (sj3_error_number == SJ3_ServerDown)
			goto server_dead;
		client.fd = -1;
		return SJ3_CONNECT_ERROR;
	}

	if (client.stdy_size > SJ3_WORD_ID_SIZE) {
		sj3_erase_connection(&client);
		return SJ3_CONNECT_ERROR;
	}

	if ((dicts_num > 0) && dicts) {
		if (!(dict_idx = (long *)malloc(sizeof(long) * dicts_num *
		    2))) {
			return SJ3_CONNECT_ERROR;
		}
		memset(dict_idx, 0, sizeof(long) * dicts_num * 2);
		dicid_num = dicts_num;
		dicid_list = dict_idx;
		err_id_num = 0;
		for (i = 0; i < dicts_num; i++) {
			dict_idx[i] = sj3_open_dictionary(&client, dicts[i],
			    NULL);
			if (dict_idx[i] == 0) {
				if (sj3_error_number == SJ3_ServerDown)
					goto server_dead;
				dict_idx[dicts_num + err_id_num] = i;
				err_id_num++;
			} else if (mdicid == 0) {
				mdicid = dict_idx[i];
			}
		}

		if (err_id_num == dicts_num) {
			err |= SJ3_CANNOT_OPEN_MDICT;
			mdicid = 0;
		}
		if (error_num)
			*error_num = err_id_num;
		if (error_index)
			*error_index = (int *)&(dict_idx[dicts_num]);
	}

	strcpy(tmp, sj3_user_dir);
	if (tmp[strlen(tmp) - 1] != *path_delimiter)
		strcat(tmp, path_delimiter);
	strcat(tmp, user);
	strcat(tmp, path_delimiter);
	if (make_dirs(tmp) == ERROR) {
		if (sj3_error_number == SJ3_ServerDown)
			goto server_dead;
		return (err | SJ3_CANNOT_MAKE_UDIR);
	}

	for (p = tmp; *p; p++)
		;
	strcpy(p, UserDictionary);
	if (sj3_access(&client, tmp, F_OK) == ERROR) {
		if (sj3_error_number == SJ3_ServerDown)
			goto server_dead;
		if (sj3_make_dict_file(&client, tmp, DefIdxLen,
		    DefSegLen, DefSegNum) == ERROR) {
			if (sj3_error_number == SJ3_ServerDown)
				goto server_dead;
			err |= SJ3_CANNOT_MAKE_UDICT;
		}
	}

	udicid = sj3_open_dictionary(&client, tmp, "");
	if (udicid == 0) {
		if (sj3_error_number == SJ3_ServerDown)
			goto server_dead;
		err |= SJ3_CANNOT_OPEN_UDICT;
	}

	strcpy(p, StudyFile);
	if (sj3_access(&client, tmp, F_OK) == ERROR) {
		if (sj3_error_number == SJ3_ServerDown)
			goto server_dead;
		if (sj3_make_study_file(&client, tmp, DefStyNum, DefClStep,
		    DefClLen) == ERROR) {
			if (sj3_error_number == SJ3_ServerDown)
				goto server_dead;
			err |= SJ3_CANNOT_MAKE_STUDY;
		}
	}
	if (sj3_open_study_file(&client, tmp, "") == ERROR) {
		if (sj3_error_number == SJ3_ServerDown)
			goto server_dead;
		err |= SJ3_CANNOT_OPEN_STUDY;
	}

	return err;

server_dead:
	if (dict_idx) {
		free(dict_idx);
		dict_idx = NULL;
		if (error_num)
			*error_num = 0;
		if (error_index)
			*error_index = NULL;
	}
	mdicid = udicid = 0;
	return SJ3_SERVER_DEAD;
}

int
sj3_close(void)
{
	int	err = SJ3_NORMAL_END;
	int	i;

	if (client.fd == -1)
		return SJ3_NOT_CONNECTED;

	if (mdicid == 0)
		err |= SJ3_NOT_OPENED_MDICT;
	else if (dicid_list) {
		for (i = 0; i < dicid_num; i++) {
			if (sj3_close_dictionary(&client, dicid_list[i]) ==
			    ERROR) {
				if (sj3_error_number == SJ3_ServerDown)
					goto server_dead;
				err |= SJ3_CLOSE_MDICT_ERROR;
			}
		}
		free(dicid_list);
		dicid_list = NULL;
		dicid_num = 0;
	} else if (sj3_close_dictionary(&client, mdicid) == ERROR) {
		if (sj3_error_number == SJ3_ServerDown)
			goto server_dead;
		err |= SJ3_CLOSE_MDICT_ERROR;
	}
	mdicid = 0;

	if (udicid == 0)
		err |= SJ3_NOT_OPENED_UDICT;
	else if (sj3_close_dictionary(&client, udicid) == ERROR) {
		if (sj3_error_number == SJ3_ServerDown)
			goto server_dead;
		err |= SJ3_CLOSE_UDICT_ERROR;
	}
	udicid = 0;

	if (sj3_close_study_file(&client) == ERROR) {
		switch (sj3_error_number) {
		case SJ3_ServerDown:
			goto server_dead;

		case SJ3_StdyFileNotOpened:
			err |= SJ3_NOT_OPENED_STUDY;
			break;

		default:
			err |= SJ3_CLOSE_STUDY_ERROR;
			break;
		}
	}

	if (sj3_erase_connection(&client)) {
		if (sj3_error_number == SJ3_ServerDown)
			goto server_dead;
		err |= SJ3_DISCONNECT_ERROR;
	}

	return err;

server_dead:
	if (dicid_list) {
		free(dicid_list);
		dicid_list = NULL;
	}
	dicid_num = 0;
	mdicid = udicid = 0;
	return SJ3_SERVER_DEAD;
}

int
sj3_getkan(unsigned char *yomi, SJ3_BUNSETU *bun, unsigned char *knj, int knjsiz)
{
	unsigned char	*src;
	int	 buncnt = 0;
	int	 len;
	int	 stysiz = client.stdy_size;

	if ((len = strlen(yomi)) > SJ3_IKKATU_YOMI)
		return 0;

	while (*yomi) {
		len = sj3_ikkatu_henkan(&client, yomi, knj, knjsiz,
		    MBCODE_SJIS);
		if (len == ERROR) {
			if (client.fd < 0) {
				mdicid = udicid = 0;
				return -1;
			}

			return 0;
		} else if (len == 0)
			break;

		src = knj;
		while (*src) {
			bun->srclen = *src++;
			memcpy(&(bun->dcid), src, stysiz);
			src += stysiz;
			bun->destlen = strlen(src);
			bun->srcstr = yomi;
			bun->deststr = knj;
			while (*src)
				*knj++ = *src++;
			knjsiz -= bun->destlen;
			src++;
			yomi += bun->srclen;
			bun++;
			buncnt++;
		}
		*knj = 0;
	}

	if (*yomi) {
		bun->srclen = strlen(yomi);
		bun->srcstr = yomi;
		bun->destlen = 0;
		bun->deststr = NULL;
		memset(&(bun->dcid), '\0', sizeof(bun->dcid));
		buncnt++;
	}

	return buncnt;
}

int
sj3_getkan_euc(unsigned char *yomi, SJ3_BUNSETU *bun, unsigned char *knj, int knjsiz)
{
	unsigned char		*src, *yp, *kp, *khp;
	int		 buncnt = 0, i;
	int		 len, flag, mflag = 0;
	int		 stysiz = client.stdy_size;
	int		 sentou, saigo;
	int		 knjorg = knjsiz;
	SJ3_BUNSETU	*hbun = bun;

	if ((len = strlen(yomi)) > SJ3_IKKATU_YOMI)
		return 0;
	if (client.svr_version == 1) {
		defuse = 0;
		len = sj3_str_euctosjis(buf1, sizeof(buf1), yomi,
		    client.default_char, &defuse);
		if ((len < 0) || defuse)
			return 0;
		yp = buf1;
		if (knjsiz > sizeof(kbuf)) {
			mflag = 1;
			kp = khp = (unsigned char *)malloc(knjsiz);
		} else {
			kp = khp = kbuf;
		}
		flag = MBCODE_SJIS;
	} else {
		yp = yomi;
		kp = knj;
		flag = MBCODE_EUC;
	}

	while (*yp) {
		len = sj3_ikkatu_henkan(&client, yp, kp, knjsiz, flag);
		if (len == ERROR) {
			if (client.fd < 0) {
				mdicid = udicid = 0;
				return -1;
			}

			return 0;
		} else if (len == 0)
			break;

		src = kp;
		while (*src) {
			bun->srclen = *src++;
			memcpy(&(bun->dcid), src, stysiz);
			src += stysiz;
			bun->destlen = strlen(src);
			bun->srcstr = yp;
			bun->deststr = kp;
			while (*src)
				*kp++ = *src++;
			knjsiz -= bun->destlen;
			src++;
			yp += bun->srclen;
			bun++;
			buncnt++;
		}
		*kp = 0;
	}

	if (*yp) {
		bun->srclen = strlen(yp);
		bun->srcstr = yp;
		bun->destlen = 0;
		bun->deststr = NULL;
		memset(&(bun->dcid), '\0', sizeof(bun->dcid));
		buncnt++;
	}

	if (client.svr_version == 1) {
		kp = khp;
		yp = buf1;
		bun = hbun;
		defuse = 0;
		len = sj3_str_sjistoeuc(knj, knjorg, kp, client.default_char,
		    &defuse);
		if ((len < 0) || defuse)
			return 0;
		for (i = 0; i < buncnt; i++) {
			sentou = sj3_sjistoeuclen(yp, bun[i].srcstr - yp);
			saigo = sj3_sjistoeuclen(bun[i].srcstr, bun[i].srclen);
			bun[i].srclen = saigo;
			bun[i].srcstr = yomi + sentou;
			sentou = sj3_sjistoeuclen(kp, bun[i].deststr - kp);
			saigo = sj3_sjistoeuclen(bun[i].deststr,
			    bun[i].destlen);
			bun[i].destlen = saigo;
			bun[i].deststr = knj + sentou;
		}
		if (mflag) {
			free(khp);
			mflag = 0;
		}
	}

	return buncnt;
}

int
sj3_getkan_mb(unsigned char *yomi, SJ3_BUNSETU *bun, unsigned char *knj, int knjsiz)
{
	if (_sys_code == SYS_NOTDEF)
		_sys_code = set_sys_code();
	if (_sys_code == SYS_EUC)
		return sj3_getkan_euc(yomi, bun, knj, knjsiz);
	else
		return sj3_getkan(yomi, bun, knj, knjsiz);
}

int
sj3_douoncnt(unsigned char *yomi)
{
	int	i;

	if ((i = strlen(yomi)) > SJ3_BUNSETU_YOMI)
		return 0;

	i = sj3_bunsetu_kouhosuu(&client, yomi, i, MBCODE_SJIS);
	if (i == ERROR) {
		if (client.fd < 0) {
			mdicid = udicid = 0;
			return -1;
		}

		return 0;
	}

	return i;
}

int
sj3_douoncnt_euc(unsigned char *yomi)
{
	int	 i, l, flag;
	unsigned char	*yp;

	if ((i = strlen(yomi)) > SJ3_BUNSETU_YOMI)
		return 0;

	if (client.svr_version == 1) {
		defuse = 0;
		l = sj3_str_euctosjis(buf1, sizeof(buf1), yomi,
		    client.default_char, &defuse);
		if ((l < 0) || defuse)
			return 0;
		yp = buf1;
		flag = MBCODE_SJIS;
	} else {
		yp = yomi;
		flag = MBCODE_EUC;
	}
	i = sj3_bunsetu_kouhosuu(&client, yp, i, flag);
	if (i == ERROR) {
		if (client.fd < 0) {
			mdicid = udicid = 0;
			return -1;
		}

		return 0;
	}

	return i;
}

int
sj3_douoncnt_mb(unsigned char *yomi)
{
	if (_sys_code == SYS_NOTDEF)
		_sys_code = set_sys_code();
	if (_sys_code == SYS_EUC)
		return sj3_douoncnt_euc(yomi);
	else
		return sj3_douoncnt(yomi);
}

int
sj3_getdouon(unsigned char *yomi, SJ3_DOUON *dou)
{
	int	i;

	if ((i = strlen(yomi)) > SJ3_BUNSETU_YOMI)
		return 0;

	i = sj3_bunsetu_zenkouho(&client, yomi, i, dou, MBCODE_SJIS);
	if (i == ERROR) {
		if (client.fd < 0) {
			mdicid = udicid = 0;
			return -1;
		}
		return 0;
	}

	return i;
}

int
sj3_getdouon_euc(unsigned char *yomi, SJ3_DOUON *dou)
{
	int	i, j, l;

	if ((i = strlen(yomi)) > SJ3_BUNSETU_YOMI)
		return 0;

	if (client.svr_version == 1) {
		defuse = 0;
		l = sj3_str_euctosjis(buf1, sizeof(buf1), yomi,
		    client.default_char, &defuse);
		if ((l < 0) || defuse)
			return 0;
		i = sj3_bunsetu_zenkouho(&client, buf1, i, dou, MBCODE_SJIS);
		if (i == ERROR) {
			if (client.fd < 0) {
				mdicid = udicid = 0;
				return -1;
			}
			return 0;
		}
		for (j = 0; j < i; j++) {
			defuse = 0;
			l = sj3_str_sjistoeuc(kbuf, sizeof(kbuf),
			    dou[j].ddata, client.default_char, &defuse);
			if ((l < 0) || defuse)
				return 0;
			(void)memcpy(dou[j].ddata, kbuf, l + 1);
			dou[j].dlen = l;
		}
	} else {
		i = sj3_bunsetu_zenkouho(&client, yomi, i, dou, MBCODE_EUC);
		if (i == ERROR) {
			if (client.fd < 0) {
				mdicid = udicid = 0;
				return -1;
			}
			return 0;
		}
	}
	return i;
}

int
sj3_getdouon_mb(unsigned char *yomi, SJ3_DOUON *dou)
{
	if (_sys_code == SYS_NOTDEF)
		_sys_code = set_sys_code();
	if (_sys_code == SYS_EUC)
		return sj3_getdouon_euc(yomi, dou);
	else
		return sj3_getdouon(yomi, dou);
}

int
sj3_gakusyuu(SJ3_STUDYREC *dcid)
{
	if (sj3_tango_gakusyuu(&client, dcid) == ERROR) {
		if (client.fd < 0) {
			mdicid = udicid = 0;
			return -1;
		}
		return 1;
	}
	return 0;
}

int
sj3_gakusyuu2(unsigned char *yomi1, unsigned char *yomi2, SJ3_STUDYREC *dcid)
{
	if (sj3_bunsetu_gakusyuu(&client, yomi1, yomi2, dcid, MBCODE_SJIS) ==
	    ERROR) {
		if (client.fd < 0) {
			mdicid = udicid = 0;
			return -1;
		}
		return 1;
	}
	return 0;
}

int
sj3_gakusyuu2_euc(unsigned char *yomi1, unsigned char *yomi2, SJ3_STUDYREC *dcid)
{
	int l, flag;
	unsigned char *y1p, *y2p;

	if (client.svr_version == 1) {
		defuse = 0;
		l = sj3_str_euctosjis(buf1, sizeof(buf1), yomi1,
		    client.default_char, &defuse);
		if ((l < 0) || defuse)
			return 1;
		defuse = 0;
		l = sj3_str_euctosjis(buf2, sizeof(buf2), yomi2,
		    client.default_char, &defuse);
		if ((l < 0) || defuse)
			return 1;
		y1p = buf1;
		y2p = buf2;
		flag = MBCODE_SJIS;
	} else {
		y1p = yomi1;
		y2p = yomi2;
		flag = MBCODE_EUC;
	}
	if (sj3_bunsetu_gakusyuu(&client, y1p, y2p, dcid, flag) == ERROR) {
		if (client.fd < 0) {
			mdicid = udicid = 0;
			return -1;
		}
		return 1;
	}
	return 0;
}

int
sj3_gakusyuu2_mb(unsigned char *yomi1, unsigned char *yomi2, SJ3_STUDYREC *dcid)
{
	if (_sys_code == SYS_NOTDEF)
		_sys_code = set_sys_code();
	if (_sys_code == SYS_EUC)
		return sj3_gakusyuu2_euc(yomi1, yomi2, dcid);
	else
		return sj3_gakusyuu2(yomi1, yomi2, dcid);
}

int
sj3_touroku(unsigned char *yomi, unsigned char *kanji, int code)
{
	if (sj3_tango_touroku(&client, udicid, yomi, kanji, code,
	    MBCODE_SJIS)) {
		if (client.fd < 0) {
			mdicid = udicid = 0;
			return -1;
		}
		switch (sj3_error_number) {
		case SJ3_NoSuchDict:
		case SJ3_ReadOnlyDict: return SJ3_DICT_ERROR;
		case SJ3_DictLocked: return SJ3_DICT_LOCKED;
		case SJ3_BadYomiString: return SJ3_BAD_YOMI_STR;
		case SJ3_BadKanjiString: return SJ3_BAD_KANJI_STR;
		case SJ3_BadHinsiCode: return SJ3_BAD_HINSI_CODE;
		case SJ3_AlreadyExistWord: return SJ3_WORD_EXIST;
		case SJ3_NoMoreDouonWord: return SJ3_DOUON_FULL;
		case SJ3_NoMoreUserDict: return SJ3_DICT_FULL;
		case SJ3_NoMoreIndexBlock: return SJ3_INDEX_FULL;
		default: return SJ3_TOUROKU_FAILED;
		}
	}

	return 0;
}

int
sj3_touroku_euc(unsigned char *yomi, unsigned char *kanji, int code)
{
	int l, flag;
	unsigned char *yp;
	unsigned char *kp;
	if (client.svr_version == 1) {
		defuse = 0;
		l = sj3_str_euctosjis(buf1, sizeof(buf1), yomi,
		    client.default_char, &defuse);
		if ((l < 0) || defuse)
			return SJ3_BAD_YOMI_STR;
		defuse = 0;
		l = sj3_str_euctosjis(kbuf, sizeof(kbuf), kanji,
		    client.default_char, &defuse);
		if ((l < 0) || defuse)
			return SJ3_BAD_KANJI_STR;
		yp = buf1;
		kp = kbuf;
		flag = MBCODE_SJIS;
	} else {
		yp = yomi;
		kp = kanji;
		flag = MBCODE_EUC;
	}
	if (sj3_tango_touroku(&client, udicid, yp, kp, code, flag)) {
		if (client.fd < 0) {
			mdicid = udicid = 0;
			return -1;
		}
		switch (sj3_error_number) {
		case SJ3_NoSuchDict:
		case SJ3_ReadOnlyDict: return SJ3_DICT_ERROR;
		case SJ3_DictLocked: return SJ3_DICT_LOCKED;
		case SJ3_BadYomiString: return SJ3_BAD_YOMI_STR;
		case SJ3_BadKanjiString: return SJ3_BAD_KANJI_STR;
		case SJ3_BadHinsiCode: return SJ3_BAD_HINSI_CODE;
		case SJ3_AlreadyExistWord: return SJ3_WORD_EXIST;
		case SJ3_NoMoreDouonWord: return SJ3_DOUON_FULL;
		case SJ3_NoMoreUserDict: return SJ3_DICT_FULL;
		case SJ3_NoMoreIndexBlock: return SJ3_INDEX_FULL;
		default: return SJ3_TOUROKU_FAILED;
		}
	}

	return 0;
}

int
sj3_touroku_mb(unsigned char *yomi, unsigned char *kanji, int code)
{
	if (_sys_code == SYS_NOTDEF)
		_sys_code = set_sys_code();
	if (_sys_code == SYS_EUC)
		return sj3_touroku_euc(yomi, kanji, code);
	else
		return sj3_touroku(yomi, kanji, code);
}

int
sj3_syoukyo(unsigned char *yomi, unsigned char *kanji, int code)
{
	if (sj3_tango_sakujo(&client, udicid, yomi, kanji, code, MBCODE_SJIS)) {
		if (client.fd < 0) {
			mdicid = udicid = 0;
			return -1;
		}
		switch (sj3_error_number) {
		case SJ3_NoSuchDict:
		case SJ3_ReadOnlyDict: return SJ3_DICT_ERROR;
		case SJ3_DictLocked: return SJ3_DICT_LOCKED;
		case SJ3_BadYomiString: return SJ3_BAD_YOMI_STR;
		case SJ3_BadKanjiString: return SJ3_BAD_KANJI_STR;
		case SJ3_BadHinsiCode: return SJ3_BAD_HINSI_CODE;
		case SJ3_NoSuchWord: return SJ3_WORD_NOT_EXIST;
		default: return SJ3_SYOUKYO_FAILED;
		}
	}
	return 0;
}

int
sj3_syoukyo_euc(unsigned char *yomi, unsigned char *kanji, int code)
{
	int l, flag;
	unsigned char *yp;
	unsigned char *kp;
	if (client.svr_version == 1) {
		defuse = 0;
		l = sj3_str_euctosjis(buf1, sizeof(buf1), yomi,
		    client.default_char, &defuse);
		if ((l < 0) || defuse)
			return SJ3_BAD_YOMI_STR;
		defuse = 0;
		l = sj3_str_euctosjis(kbuf, sizeof(kbuf), kanji,
		    client.default_char, &defuse);
		if ((l < 0) || defuse)
			return SJ3_BAD_KANJI_STR;
		yp = buf1;
		kp = kbuf;
		flag = MBCODE_SJIS;
	} else {
		yp = yomi;
		kp = kanji;
		flag = MBCODE_EUC;
	}
	if (sj3_tango_sakujo(&client, udicid, yp, kp, code, flag)) {
		if (client.fd < 0) {
			mdicid = udicid = 0;
			return -1;
		}
		switch (sj3_error_number) {
		case SJ3_NoSuchDict:
		case SJ3_ReadOnlyDict: return SJ3_DICT_ERROR;
		case SJ3_DictLocked: return SJ3_DICT_LOCKED;
		case SJ3_BadYomiString: return SJ3_BAD_YOMI_STR;
		case SJ3_BadKanjiString: return SJ3_BAD_KANJI_STR;
		case SJ3_BadHinsiCode: return SJ3_BAD_HINSI_CODE;
		case SJ3_NoSuchWord: return SJ3_WORD_NOT_EXIST;
		default: return SJ3_SYOUKYO_FAILED;
		}
	}
	return 0;
}

int
sj3_syoukyo_mb(unsigned char *yomi, unsigned char *kanji, int code)
{
	if (_sys_code == SYS_NOTDEF)
		_sys_code = set_sys_code();
	if (_sys_code == SYS_EUC)
		return sj3_syoukyo_euc(yomi, kanji, code);
	else
		return sj3_syoukyo(yomi, kanji, code);
}

int
sj3_getdict(unsigned char *buf)
{
	if (sj3_tango_syutoku(&client, udicid, buf, MBCODE_SJIS)) {
		if (client.fd < 0) {
			mdicid = udicid = 0;
			return -1;
		}
		return 1;
	}
	return 0;
}

int
sj3_getdict_euc(unsigned char *buf)
{
	int l, ll, slen;

	if (client.svr_version == 1) {
		if (sj3_tango_syutoku(&client, udicid, buf, MBCODE_SJIS)) {
			if (client.fd < 0) {
				mdicid = udicid = 0;
				return -1;
			}
			return 1;
		}
		defuse = 0;
		slen = strlen(buf) + 1;
		l = sj3_str_sjistoeuc(kbuf, sizeof(kbuf), buf,
		    client.default_char, &defuse);
		if ((l < 0) || defuse)
			return 1;
		l++;
		ll = sj3_str_sjistoeuc(&(kbuf[l]), sizeof(kbuf) - l,
		    &(buf[slen]), client.default_char, &defuse);
		if ((ll < 0) || defuse)
			return 1;
		ll++;
		slen += strlen(&(buf[slen])) + 1;
		l = l + ll;
		memcpy(&(kbuf[l]), &(buf[slen]), 4);
		l += 4;
		memcpy(buf, kbuf, l);
	} else {
		if (sj3_tango_syutoku(&client, udicid, buf, MBCODE_EUC)) {
			if (client.fd < 0) {
				mdicid = udicid = 0;
				return -1;
			}
			return 1;
		}
	}
	return 0;
}

int
sj3_getdict_mb(unsigned char *buf)
{
	if (_sys_code == SYS_NOTDEF)
		_sys_code = set_sys_code();
	if (_sys_code == SYS_EUC)
		return sj3_getdict_euc(buf);
	else
		return sj3_getdict(buf);
}

int
sj3_nextdict(unsigned char *buf)
{
	if (sj3_tango_jikouho(&client, udicid, buf, MBCODE_SJIS)) {
		if (client.fd < 0) {
			mdicid = udicid = 0;
			return -1;
		}
		return 1;
	}
	return 0;
}

int
sj3_nextdict_euc(unsigned char *buf)
{
	int l, ll, slen;

	if (client.svr_version == 1) {
		if (sj3_tango_jikouho(&client, udicid, buf, MBCODE_SJIS)) {
			if (client.fd < 0) {
				mdicid = udicid = 0;
				return -1;
			}
			return 1;
		}
		defuse = 0;
		slen = strlen(buf) + 1;
		l = sj3_str_sjistoeuc(kbuf, sizeof(kbuf), buf,
		    client.default_char, &defuse);
		if ((l < 0) || defuse)
			return 1;
		l++;
		ll = sj3_str_sjistoeuc(&(kbuf[l]), sizeof(kbuf) - l,
		    &(buf[slen]), client.default_char, &defuse);
		if ((ll < 0) || defuse)
			return 1;
		ll++;
		slen += strlen(&(buf[slen])) + 1;
		l = l + ll;
		memcpy(&(kbuf[l]), &(buf[slen]), 4);
		l += 4;
		memcpy(buf, kbuf, l);
	} else {
		if (sj3_tango_jikouho(&client, udicid, buf, MBCODE_EUC)) {
			if (client.fd < 0) {
				mdicid = udicid = 0;
				return -1;
			}
			return 1;
		}
	}
	return 0;
}

int
sj3_nextdict_mb(unsigned char *buf)
{
	if (_sys_code == SYS_NOTDEF)
		_sys_code = set_sys_code();
	if (_sys_code == SYS_EUC)
		return sj3_nextdict_euc(buf);
	else
		return sj3_nextdict(buf);
}

int
sj3_prevdict(unsigned char *buf)
{
	if (sj3_tango_maekouho(&client, udicid, buf, MBCODE_SJIS)) {
		if (client.fd < 0) {
			mdicid = udicid = 0;
			return -1;
		}
		return 1;
	}
	return 0;
}

int
sj3_prevdict_euc(unsigned char *buf)
{
	int l, ll, slen;

	if (client.svr_version == 1) {
		if (sj3_tango_maekouho(&client, udicid, buf, MBCODE_SJIS)) {
			if (client.fd < 0) {
				mdicid = udicid = 0;
				return -1;
			}
			return 1;
		}
		defuse = 0;
		slen = strlen(buf) + 1;
		l = sj3_str_sjistoeuc(kbuf, sizeof(kbuf), buf,
		    client.default_char, &defuse);
		if ((l < 0) || defuse)
			return 1;
		l++;
		ll = sj3_str_sjistoeuc(&(kbuf[l]), sizeof(kbuf) - l,
		    &(buf[slen]), client.default_char, &defuse);
		if ((ll < 0) || defuse)
			return 1;
		ll++;
		slen += strlen(&(buf[slen])) + 1;
		l = l + ll;
		memcpy(&(kbuf[l]), &(buf[slen]), 4);
		l += 4;
		memcpy(buf, kbuf, l);
	} else {
		if (sj3_tango_maekouho(&client, udicid, buf, MBCODE_EUC)) {
			if (client.fd < 0) {
				mdicid = udicid = 0;
				return -1;
			}
			return 1;
		}
	}
	return 0;
}

int
sj3_prevdict_mb(unsigned char *buf)
{
	if (_sys_code == SYS_NOTDEF)
		_sys_code = set_sys_code();
	if (_sys_code == SYS_EUC)
		return sj3_prevdict_euc(buf);
	else
		return sj3_prevdict(buf);
}

int
sj3_lockserv(void)
{
	if (sj3_lock_server(&client) == ERROR) {
		if (client.fd < 0) {
			mdicid = udicid = 0;
			return -1;
		}
		return 1;
	}
	return 0;
}

int
sj3_unlockserv(void)
{
	if (sj3_unlock_server(&client)) {
		if (client.fd < 0) {
			mdicid = udicid = 0;
			return -1;
		}
		return 1;
	}
	return 0;
}
