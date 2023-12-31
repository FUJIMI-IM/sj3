/*
 * Copyright (c) 2023 FUJIMI-IM project
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef SJ3_FUNCS_H_
#define SJ3_FUNCS_H_

#include "sj_struct.h"

#ifndef SJ3_RK_H_	// "rk.h" not included
typedef void RkTablW16;
#endif
#ifndef _WCHARH		// "wchar16.h" not included
typedef unsigned short wchar16_t;
#endif

/* cmpstr.c */
int cmpstr(unsigned char *, unsigned char *);

/* codecnv.c */
int init_code(void);
void printout_mb(FILE *, unsigned char *);
void printout(FILE *, unsigned char *);
void normal_out(char *, ...);

/* comuni.c */
unsigned char *put_string(unsigned char *);
unsigned char *put_ndata(unsigned char *, int);

/* dictdisp.c */
void dictdisp(char *output);

/* dictmake.c */
void  dictmake(char *input);

/* fuzoku.c */
void setclrec(JREC *, unsigned char *, TypeCnct);
void srchfzk(JREC *, unsigned char *, TypeCnct, int);

/* hinsi.c */
char *hns2str(int);
int str2hns(char *);

/* init.c */
void mkidxtbl(DICT *);
void initwork(void);

/* main.c */
void server_terminate(void);

/* memory2.c */
JREC *free_jlst(JREC *);
CLREC *free_clst(CLREC *, int);
void free_clall(CLREC *);
void free_jall(JREC *);
void freework(void);

/* mk2claus.c */
void mk2claus(void);

/* mvmemd.c */
void mvmemd(unsigned char *, unsigned char *, int);

/* mvmemi.c */
void mvmemi(unsigned char *, unsigned char *, int);

/* rk_conv.c */
int sj3_rkinit_mb(char *);
char *getkey(char *, wchar16_t *, int *);
char *rkgetyomi(char *, wchar16_t *, int *);
void cltable(void);
int chk_rstr(wchar16_t *, wchar16_t *, int, int);
int stradd(wchar16_t **, wchar16_t *, int);
int kstradd(wchar16_t **, wchar16_t *, int);
RkTablW16 *mktable(wchar16_t *, int);
void sj3_rkclear(void);
void sj3_rkreset(void);
int sj3_rkconv2(wchar16_t *, unsigned int *, int);
int sj3_rkinit_sub(char *, int (*)(void));
int sj3_rkconv_w16(wchar16_t *, wchar16_t *);
int rkmatch(wchar16_t *, wchar16_t *, int);

/* sj2code.c */
int sj2cd_chr(unsigned char *, unsigned char *);

/* sj3_rkcv.c */
int sj3_hantozen_w16(wchar16_t *, wchar16_t *);
int sj_hantozen(wchar16_t *, wchar16_t *, int);
unsigned short sj_zen2han(unsigned short);
int sj3_zentohan_w16(wchar16_t *, wchar16_t *);
int sj_zentohan(wchar16_t *, wchar16_t *, int);

/* selclrec.c */
void selclrec(void);

/* sjrc.c */
int getsjrc(void);
void setrc(char *, FILE *);
int much(char *, char *);
int IsTerminator(unsigned char);
int isTerminator(unsigned char);
int IsEscape(unsigned char);
int IsDelimitor(unsigned char);

/* terminat.c */
int terminate(TypeCnct, unsigned char *);

/* wc16_str.c */
int sj3_wslen16(wchar16_t *);
int sj3_wscmp16(wchar16_t *, wchar16_t *);
int sj3_wsncmp16(wchar16_t *, wchar16_t *, int);
wchar16_t *sj3_wscpy16(wchar16_t *, wchar16_t *);
int sj3_mbstowcs16(wchar16_t *, unsigned char *, int);
int sj3_wcstombs16(unsigned char *, wchar16_t *, int);

#endif /* SJ3_FUNCS_H_ */
