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
 * $SonyRCSfile: rk.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:48 $
 */



#include <sys/types.h>
#include "wchar16.h"

#define	SPACE		0x20
#define	TAB		0x09
#define	EOL		0x0a
#define	NSTR		'\0'
#ifndef ESC
#define	ESC		0x1b
#endif
#define	QUOTA		'\''
#define	BSL		'\\'
#define	MASK		0xff

#define	MAXLEN		256
#define	MAXWLEN		16
#define	MAXLLEN		64
#define	MAXCODE		256

#define	CONTINUE	0
#define	NOMACH		-1
#define	FAIL		-1
#define	RKEND		0xffffffff
#define ERRBEL		0xfffffffc
#define	ERRCODE		0x80000000
#define	RKMASK		0x0000ffff
#define	RKZEN		0xff00

#define SetMojilen(c)	((c << 16) & 0x00ff0000)
#define GetMojilen(c)	((c & ERRCODE) ? (1) : ((c >> 16) & 0xff)) 

typedef	struct rktable_w16 {
	wchar16_t *r_key;
	u_short *k_yomi;
	wchar16_t *r_str;
        struct rktable_w16 *next;
} RkTablW16;

typedef	struct rktable {
	u_char *r_key;
	u_short *k_yomi;
	u_char *r_str;
	struct rktable *next;
} RkTabl;

