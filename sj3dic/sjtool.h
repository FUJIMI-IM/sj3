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

/*
 * $SonyRCSfile: sjtool.h,v $  
 * $SonyRevision: 1.2 $ 
 * $SonyDate: 1996/02/15 02:22:59 $
 */

#if defined(__NetBSD__)
#define LACKOF_SETLOCALE
#endif

#define	LONGLENGTH	256
#define	MAXLINE		256
#define	MAXWORD		256
#define	WORDN		32

#ifndef	SYS_SJIS
#	define	SYS_SJIS	0
#endif
#ifndef	SYS_EUC
#	define	SYS_EUC		1
#endif

#define	TRUE		-1
#define	FALSE		!(TRUE)

#define	EXEC_TEXT	1
#define	EXEC_DICT	2
#define	EXEC_CONV	3
#define	EXEC_INIT	10
#define	EXEC_VERBOSE	11
#define	EXEC_SERVER	12
#define	EXEC_FORCE	13

#define	SingleShift2	0x8e
