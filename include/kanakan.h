/*
 * Copyright (c) 2004  Hiroo Ono <hiroo+sj3 at oikumene.gcd.org>
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
 * $Id: kanakan.h,v 1.2 2004/07/04 08:11:10 hiroo Exp $
 */

#ifndef _KANAKAN_H_
#define _KANAKAN_H_  1

#include "sj_struct.h"
#include "Struct.h"

/* adddic.c */
u_int adddic(u_char *yomi, u_char *kanji, TypeGram hinsi);

/* addelcmn.c */
void set_size(unsigned char *p, int size, int plen, int nlen);
unsigned int addel_arg(unsigned char *yp, unsigned char *kp, TypeGram grm, unsigned char *nyp, int len);
int cvtknj(unsigned char *yomi, unsigned char *knj, unsigned char *dst);
int srchkana(unsigned char **ptr, int *saml);
int srchgram(unsigned char *ptr, unsigned char **dst, TypeGram hinsi);
int srchkanji(unsigned char **dst, unsigned char *knj, int klen);
TypeIdxOfs count_uidx(void);
void chg_uidx(TypeDicSeg seg, unsigned char *yomi, int len);

/* alloc.c */
JREC *alloc_jrec(void);
void free_jrec(JREC *p);
CLREC *alloc_clrec(void);
void free_clrec(CLREC *p);

/* charsize.c */
int codesize(u_char code);

/* cl2knj.c */
int cl2knj(u_char *yomi, int len, u_char *kouho);
int nextcl(u_char *kouho, int mode);
int prevcl(u_char *kouho, int mode);
int selectnum(void);

/* clstudy.c */
int clstudy(u_char *yomi1, u_char *yomi2, STDYOUT *stdy);
void delclsub(u_char *target);
void mkclidx(void);

/* cvtdict.c */
void cvtdict(KHREC *krec, CLREC *clrec, int flg);
void cvtminasi(int len);
void cvtwakachi(CLREC *clrec);

/* cvtkanji.c */
void cvtphknj(void);
void cvtkouho(KHREC *krec);
void setstyrec(KHREC *krec);

/* deldic.c */
u_int deldic(u_char *yomi, u_char *kanji, TypeGram hinsi);

/* depend.c */
DictFile *opendict(char *name, char *passwd);
int closedict(DictFile *dfp);
void lock_dict(DictFile *p, int fd);
void unlock_dict(DictFile *p, int fd);
int is_dict_locked(DictFile *p);
StdyFile *openstdy(char *name, char *passwd);
int closestdy(StdyFile *sfp);
int putstydic(void);
int putcldic(void);
int makedict(char *path, int idxlen, int seglen, int segnum);
int makestdy(char *path, int stynum, int clstep, int cllen);
void sj_closeall(void);
int set_dictpass(DictFile *dp, char *pass);
int set_stdypass(char *pass);
int set_dictcmnt(DictFile *dp, char *cmnt);
int set_stdycmnt(char *cmnt);
void get_stdysize(int *stynum, int *clstep, int *cllen);

/* dict.c */
void get_askknj(void);
int seldict(TypeDicID id);
u_char *get_idxptr(TypeDicSeg seg);

/* getkanji.c */
u_char *getkan_none(u_char *s, u_char *d, u_char *ym, int yl, int flg);
u_char *getkan_hira(u_char *s, u_char *d, u_char *ym, int yl, int flg);
u_char *getkan_kata(u_char *s, u_char *d, u_char *ym, int yl, int flg);
u_char *getkan_knj(u_char *s, u_char *d, u_char *ym, int yl, int flg);
u_char *getkan_ofs(u_char *s, u_char *d, u_char *ym, int yl, int flg);
u_char *getkan_norm(u_char *s, u_char *d, u_char *ym, int yl, int flg);
u_char *getkan_ascii(u_char *s, u_char *d, u_char *ym, int yl, int flg);
int getkanji(u_char *ym, int yl, u_char *ptr, u_char *buf);

/* getrank.c */
void getrank(void);

/* hzstrlen.c */
int euc_codesize(unsigned char c);
int hzstrlen(unsigned char *ptr, int len);

/* mkbunset.c */
void mkbunsetu(void);
CLREC* argclrec(int len);

/* mkjiritu.c */
void mkjiritu(int mode);
JREC *argjrec(int len, JREC *rec);

/* mkkouho.c */
u_char *makekan_none(u_char *s, u_char *d, int flg);
u_char *makekan_1byte(u_char *s, u_char *d, int flg);
u_char *makekan_knj(u_char *s, u_char *d, int flg);
u_char *makekan_ofs(u_char *s, u_char *d, int flg);
u_char *makekan_norm(u_char *s, u_char *d, int flg);
u_char *makekan_ascii(u_char *s, u_char *d, int flg);
void mkkouho(void);
int sel_sjmode(JREC *jrec);

/* mknumber.c */
void num_type00(u_char *s1, u_char *s2, JREC *jrec);
void num_type01(u_char *s1, u_char *s2, JREC *jrec);
void num_type02(u_char *s1, u_char *s2, JREC *jrec);
void num_type03(u_char *s1, u_char *s2, JREC *jrec);
void num_type04(u_char *s1, u_char *s2, JREC *jrec);
void num_type05(u_char *s1, u_char *s2, JREC *jrec);
void num_type06(u_char *s1, u_char *s2, JREC *jrec);
void num_type07(u_char *s1, u_char *s2, JREC *jrec);
void num_type08(u_char *s1, u_char *s2, JREC *jrec);
void num_type09(u_char *s1, u_char *s2, JREC *jrec);
void num_type10(u_char *s1, u_char *s2, JREC *jrec);
void num_type11(u_char *s1, u_char *s2, JREC *jrec);
void num_type12(u_char *s1, u_char *s2, JREC *jrec);
void num_type13(u_char *s1, u_char *s2, JREC *jrec);
void num_type14(u_char *s1, u_char *s2, JREC *jrec);

/* peepdic.c */
int getusr(unsigned char *buf);
int nextusr(unsigned char *buf);
int prevusr(unsigned char *buf);

/* ph2knj.c */
int ph2knj(unsigned char *zyomi, unsigned char *kanji, int knjlen);

/* ph_khtbl.c */
int ph_khtbl(CLREC *clrec);

/* setconj.c */
int setconj(TypeGram hinsi, JREC *jrec, CREC *crec);

/* setjrec.c */
int setj_atrb(u_char* p);
int setj_ofs(u_char* p);
int setj_knj(u_char* p);
int setj_norm1(u_char* p);
int setj_norm2(u_char* p);
int setj_norm3(u_char* p);
void setjrec(u_char* tagp, int mode);
void setnumrec(u_char* tagp, JREC* rec, TypeGram gram);
void setcrec(u_char* tagp);

/* setkouho.c */
void setkouho(CLREC* clrec, TypeDicOfs offs, int mode);
void ph_setkouho(CLREC* clrec, TypeDicOfs offs, STDYIN* sptr);
int hiraknj_atrb(u_char*, int*);
int hiraknj_ofs (u_char*, int*);
int hiraknj_knj (u_char*, int*);
int hiraknj_hask(u_char*, int*);
int hiraknj_kask(u_char*, int*);
int hiraknj_norm(u_char*, int*);
int hiraknj_hira(u_char*, int*);

/* setubi.c */
unsigned char *getstb(TypeGram hinsi);
void setubi(JREC *rec, unsigned char *stbtbl);

/* skipkstr.c */
u_char* skipkstr(u_char* ptr);

/* srchdict.c */
int yomicmp(u_char *ptr1, u_char *ptr2, u_char *saml);
u_char *srchdict(u_char *tagp);

/* srchidx.c */
TypeDicSeg srchidx(TypeDicSeg low, int len);

/* srchnum.c */
void srchnum(void);
void setwdnum(u_char *p, int len, u_short *wd);
int setucnum(u_char *p, int len, u_char *ud);

/* study.c */
int study(STDYOUT *stdy);
STDYIN *srchstdy(TypeDicSeg seg, TypeDicOfs ofs, TypeDicID dicid);

/* wakachi.c */
void wakachi(void);

#endif /* _KANAKAN_H_ */
