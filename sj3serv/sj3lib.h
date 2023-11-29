/*-
 * SPDX-License-Identifier: MIT-open-group
 *
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
 */

#ifndef SJ3_SJ3LIB_H_
#define SJ3_SJ3LIB_H_

#include <sys/types.h>

#define SJ3_NORMAL_END 0
#define SJ3_SERVER_DEAD (1 << 0)

#define SJ3_CONNECT_ERROR (1 << 1)
#define SJ3_ALREADY_CONNECTED (1 << 2)
#define SJ3_CANNOT_OPEN_MDICT (1 << 3)
#define SJ3_CANNOT_OPEN_UDICT (1 << 4)
#define SJ3_CANNOT_OPEN_STUDY (1 << 5)
#define SJ3_CANNOT_MAKE_UDIR (1 << 6)
#define SJ3_CANNOT_MAKE_UDICT (1 << 7)
#define SJ3_CANNOT_MAKE_STUDY (1 << 8)

#define SJ3_DISCONNECT_ERROR (1 << 1)
#define SJ3_NOT_CONNECTED (1 << 2)
#define SJ3_NOT_OPENED_MDICT (1 << 3)
#define SJ3_NOT_OPENED_UDICT (1 << 4)
#define SJ3_NOT_OPENED_STUDY (1 << 5)
#define SJ3_CLOSE_MDICT_ERROR (1 << 6)
#define SJ3_CLOSE_UDICT_ERROR (1 << 7)
#define SJ3_CLOSE_STUDY_ERROR (1 << 8)

#define SJ3_DICT_ERROR 1
#define SJ3_DICT_LOCKED 2
#define SJ3_BAD_YOMI_STR 3
#define SJ3_BAD_KANJI_STR 4
#define SJ3_BAD_HINSI_CODE 5

#define SJ3_WORD_EXIST 6
#define SJ3_DOUON_FULL 7
#define SJ3_DICT_FULL 8
#define SJ3_INDEX_FULL 9
#define SJ3_TOUROKU_FAILED 10

#define SJ3_WORD_NOT_EXIST 6
#define SJ3_SYOUKYO_FAILED 10

#define SJ3_IKKATU_YOMI 512

#define SJ3_BUNSETU_YOMI 128
#define SJ3_BUNSETU_KANJI 512

#ifndef SJ3_WORD_ID_SIZE
#define SJ3_WORD_ID_SIZE 32
#endif

typedef struct studyrec {
	unsigned char dummy[SJ3_WORD_ID_SIZE];
} SJ3_STUDYREC;

typedef struct bunsetu {
	int srclen;
	int destlen;
	unsigned char *srcstr;
	unsigned char *deststr;
	struct studyrec dcid;
} SJ3_BUNSETU;

typedef struct douon {
	unsigned char ddata[SJ3_BUNSETU_KANJI];
	int dlen;
	SJ3_STUDYREC dcid;
} SJ3_DOUON;

#define SJ3_H_NRMNOUN 1
#define SJ3_H_PRONOUN 12
#define SJ3_H_LNAME 21
#define SJ3_H_FNAME 22
#define SJ3_H_LOCNAME 24
#define SJ3_H_PREFIC 25
#define SJ3_H_RENTAI 26
#define SJ3_H_CONJUNC 27
#define SJ3_H_SUBNUM 29
#define SJ3_H_NUMERAL 30
#define SJ3_H_PREFIX 31
#define SJ3_H_POSTFIX 36
#define SJ3_H_ADVERB 45
#define SJ3_H_ADJECT 60
#define SJ3_H_ADJVERB 71
#define SJ3_H_SILVERB 80
#define SJ3_H_ZILVERB 81
#define SJ3_H_ONEVERB 90
#define SJ3_H_KAVERB 91
#define SJ3_H_GAVERB 92
#define SJ3_H_SAVERB 93
#define SJ3_H_TAVERB 94
#define SJ3_H_NAVERB 95
#define SJ3_H_BAVERB 96
#define SJ3_H_MAVERB 97
#define SJ3_H_RAVERB 98
#define SJ3_H_WAVERB 99
#define SJ3_H_SINGLE 189

typedef struct sj3_client_env {
	int fd;
	int serv_dead_flg;
	int stdy_size;
	short svr_version;
	short cli_version;
	char default_char[2];
} SJ3_CLIENT_ENV;

#ifndef SJ3_NAME_LENGTH
#define SJ3_NAME_LENGTH 128
#endif
typedef struct sj3_who_struct {
	int fd;
	char hostname[SJ3_NAME_LENGTH];
	char username[SJ3_NAME_LENGTH];
	char progname[SJ3_NAME_LENGTH];
} SJ3_WHO_STRUCT;

#define MBCODE_SJIS 1
#define MBCODE_EUC 2

/* level1.c */
void sj3_set_timeout(int timeout);
int sj3_make_connection(SJ3_CLIENT_ENV *client, char *serv, char *user,
    char *prog);
int sj3_erase_connection(SJ3_CLIENT_ENV *client);
long sj3_open_dictionary(SJ3_CLIENT_ENV *client, char *dictname,
    char *password);
int sj3_close_dictionary(SJ3_CLIENT_ENV *client, long dicid);
int sj3_open_study_file(SJ3_CLIENT_ENV *client, char *stdyname, char *password);
int sj3_close_study_file(SJ3_CLIENT_ENV *client);
int sj3_get_id_size(SJ3_CLIENT_ENV *client);
int sj3_lock_server(SJ3_CLIENT_ENV *client);
int sj3_unlock_server(SJ3_CLIENT_ENV *client);
int sj3_ikkatu_henkan(SJ3_CLIENT_ENV *client, u_char *src, u_char *dst,
    int dstsiz, int mb_flag);
int sj3_bunsetu_henkan(SJ3_CLIENT_ENV *client, u_char *yomi, int len,
    u_char *kanji, int mb_flag);
int sj3_bunsetu_jikouho(SJ3_CLIENT_ENV *client, u_char *kanji, int mode,
    int mb_flag);
int sj3_bunsetu_maekouho(SJ3_CLIENT_ENV *client, u_char *kanji, int mode,
    int mb_flag);
int sj3_bunsetu_kouhosuu(SJ3_CLIENT_ENV *client, u_char *yomi, int len,
    int mb_flag);
int sj3_bunsetu_zenkouho(SJ3_CLIENT_ENV *client, u_char *yomi, int len,
    SJ3_DOUON *douon, int mb_flag);
int sj3_tango_gakusyuu(SJ3_CLIENT_ENV *client, SJ3_STUDYREC *stdy);
int sj3_bunsetu_gakusyuu(SJ3_CLIENT_ENV *client, u_char *yomi1, u_char *yomi2,
    SJ3_STUDYREC *stdy, int mb_flag);
int sj3_tango_touroku(SJ3_CLIENT_ENV *client, long dicid, u_char *yomi,
    u_char *kanji, int code, int mb_flag);
int sj3_tango_sakujo(SJ3_CLIENT_ENV *client, long dicid, u_char *yomi,
    u_char *kanji, int code, int mb_flag);
int sj3_tango_syutoku(SJ3_CLIENT_ENV *client, int dicid, u_char *buf,
    int mb_flag);
int sj3_tango_jikouho(SJ3_CLIENT_ENV *client, int dicid, u_char *buf,
    int mb_flag);
int sj3_tango_maekouho(SJ3_CLIENT_ENV *client, int dicid, u_char *buf,
    int mb_flag);
int sj3_make_dict_file(SJ3_CLIENT_ENV *client, char *path, int idxlen,
    int seglen, int segnum);
int sj3_make_study_file(SJ3_CLIENT_ENV *client, char *path, int stynum,
    int clstep, int cllen);
int sj3_make_directory(SJ3_CLIENT_ENV *client, char *path);
int sj3_access(SJ3_CLIENT_ENV *client, char *path, int mode);
int sj3_who(SJ3_CLIENT_ENV *client, SJ3_WHO_STRUCT *ret, int num);
int sj3_quit(SJ3_CLIENT_ENV *client);
int sj3_kill(SJ3_CLIENT_ENV *client);
int sj3_version(SJ3_CLIENT_ENV *client, char *dst, int dstsiz);

/* sj.c */
int sj3_open(char *host, char *user);
int sj3_open_with_list(char *host, char *user, int dicts_num, char **dicts,
    int *error_num, int **error_index);
int sj3_close(void);
int sj3_getkan(u_char *yomi, SJ3_BUNSETU *bun, u_char *knj, int knjsiz);
int sj3_getkan_euc(u_char *yomi, SJ3_BUNSETU *bun, u_char *knj, int knjsiz);
int sj3_getkan_mb(u_char *yomi, SJ3_BUNSETU *bun, u_char *knj, int knjsiz);
int sj3_douoncnt(u_char *yomi);
int sj3_douoncnt_euc(u_char *yomi);
int sj3_douoncnt_mb(u_char *yomi);
int sj3_getdouon(u_char *yomi, SJ3_DOUON *dou);
int sj3_getdouon_euc(u_char *yomi, SJ3_DOUON *dou);
int sj3_getdouon_mb(u_char *yomi, SJ3_DOUON *dou);
int sj3_gakusyuu(SJ3_STUDYREC *dcid);
int sj3_gakusyuu2(u_char *yomi1, u_char *yomi2, SJ3_STUDYREC *dcid);
int sj3_gakusyuu2_euc(u_char *yomi1, u_char *yomi2, SJ3_STUDYREC *dcid);
int sj3_gakusyuu2_mb(u_char *yomi1, u_char *yomi2, SJ3_STUDYREC *dcid);
int sj3_touroku(u_char *yomi, u_char *kanji, int code);
int sj3_touroku_euc(u_char *yomi, u_char *kanji, int code);
int sj3_touroku_mb(u_char *yomi, u_char *kanji, int code);
int sj3_syoukyo(u_char *yomi, u_char *kanji, int code);
int sj3_syoukyo_euc(u_char *yomi, u_char *kanji, int code);
int sj3_syoukyo_mb(u_char *yomi, u_char *kanji, int code);
int sj3_getdict(u_char *buf);
int sj3_getdict_euc(u_char *buf);
int sj3_getdict_mb(u_char *buf);
int sj3_nextdict(u_char *buf);
int sj3_nextdict_euc(u_char *buf);
int sj3_nextdict_mb(u_char *buf);
int sj3_prevdict(u_char *buf);
int sj3_prevdict_euc(u_char *buf);
int sj3_prevdict_mb(u_char *buf);
int sj3_lockserv(void);
int sj3_unlockserv(void);

#endif
