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

#ifndef SJ3_STRUCT_H_
#define SJ3_STRUCT_H_

#include <sys/select.h>
#include <sys/types.h>
#include <stdio.h>

#include "const.h"
#include "sj_dict.h"
#include "sj_struct.h"
#include "sj_var.h"

typedef struct global {
	JREC		*Jmaxjptr;
	CLREC		*Jmaxclptr;
	JREC		*Jjrt1st;
	CLREC		*Jclt1st;
	JREC		*Jjrt2nd;
	CLREC		*Jclt2nd;
	CLREC		*Jselcl;

	short		 Jselectid;
	short		 Jtrank;
	short		 Jnrank;

	unsigned char	*Jinputyomi;
	unsigned char	*Jcnvstart;
	short		 Jcnvlen;
	unsigned char	*Jystart;
	unsigned char	 Jhyomi[MaxPhInputLen + 1];
	unsigned char	 Jorgyomi[MaxClInputLen*2 + 1];

	unsigned char	 Jkanjibuf[MinPhOutputLen];
	unsigned char	*Jkanjipos;
	unsigned char	*Jkanjitmp;
	short		 Jkanjilen;

	unsigned char	 Jdicinl;
	unsigned char	 Jdicsaml;
	TypeDicSeg	 Jprevseg;

	unsigned char	 Jfzkcount;
	unsigned char	 Jfzk_ka_flg;
	FREC		 Jfzktbl[MAXFREC];

	unsigned char	 Jheadcode;
	unsigned char	 Jheadlen;

	unsigned char	 Jgobilen;

	TypeGram	 Jprevclgrm;
	TypeCnct	 Jprevclrow;
	unsigned char	 Jnextcllen;

	short		 Jkhcount;
	short		 Jnkhcount;
	KHREC		 Jkouhotbl[MaxKouhoNumber];
	STDYIN		*Jstdytop;

	DICT		*Jcurdict;
	DICTL		*Jdictlist;
	unsigned char	*Jdicbuf;
	unsigned char	*Jidxbuf;
	TypeIdxOfs	*Jidxofs;
	unsigned char	*Jaskknj[MaxKnjAskNumber];
	unsigned char	*Jaskknj_k[MaxKnjAskNumber];

	STDY		*Jcurstdy;

	unsigned short	 Jsuuji_wbuf[NumWordBuf];
	unsigned char	 Jsuuji_ubuf[NumKetaLength];
	TypeClass	 Jsuuji_class;
	unsigned char	 Jsuuji_comma;
	unsigned char	 Jsuuji_keta;
	unsigned char	*Jsuuji_yptr;
	unsigned char	 Jsuuji_len;
	unsigned char	 Jsuuji_exit;
	unsigned short	*Jsuuji_wkeep;
	unsigned char	*Jsuuji_ukeep;

	unsigned char	 Jpeepyomi[MaxWdYomiLen * 2 + 1];
	unsigned char	 Jpeepknj[MaxWdKanjiLen + 1];
	TypeGram	 Jpeepgrm;
	unsigned char	*Jpeepdptr;
	unsigned char	*Jpeephptr;
	unsigned char	*Jpeepkptr;
	TypeDicSeg	 Jpeepidx;
} Global;

typedef struct dictfile {
	DICT		 dict;
	int		 refcnt;
	fd_set		 lock;
	FILE		*fp;
	int		 fd;

	unsigned char	*buffer;
	long		 bufsiz;

	long		 idxstrt;
	long		 segstrt;

	TypeIdxOfs	*ofsptr;

	struct dictfile	*link;
} DictFile;

typedef struct stdyfile {
	STDY		 stdy;
	int		 refcnt;
	ino_t		 inode;
	FILE		*fp;
	int		 fd;

	unsigned char	*header;

	struct stdyfile	*link;
} StdyFile;

typedef struct workarea {
	int		 refcnt;
	int		 lock;

	struct workarea	*link;

	Global		 global;
} WorkArea;

typedef struct client {
	int		 fd;
	char		 host[HostNameLen];
	char		 user[UserNameLen];
	char		 prog[ProgNameLen];

	WorkArea	*work;
	StdyFile	*stdy;
	short		 version;
	unsigned char	 def_char[2];
	uid_t		 uid;
	struct client	*next;
} Client;

#endif /* SJ3_STRUCT_H_ */
