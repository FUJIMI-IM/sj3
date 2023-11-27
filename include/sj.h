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
 * $SonyRCSfile: sj.h,v $  
 * $SonyRevision: 1.2 $ 
 * $SonyDate: 1996/02/23 11:04:02 $
 */


#include "sj_sysvdef.h"
#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef SVR4
#ifdef __sony_news
#include "/usr/include/sys/ioctl.h"
#endif
#include <sys/ttold.h>
#include <sys/termio.h>
#else
#include <sys/ioctl.h>
#endif
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/param.h>
#ifdef HUPCL
#undef HUPCL
#endif


#include <pwd.h>
#ifdef SVR4
#include <sac.h>
#include <utmpx.h>
#else
#include <utmp.h>
#endif
#if defined(__sony_news) && defined(SVR4)
#include <jctype.h>
#endif

#if defined(BSD4_3) || defined(SVR4)
#define BSD43
#endif 

#ifdef __sony_news
#ifdef KM_EUC		
#ifndef EUC
#define EUC
#endif 
#endif 
#else
#ifndef EUC
#define EUC
#endif
#endif



#ifdef NOTDEF
#define WBUFF		512
#endif
#define BUFFLENGTH	128
#define	LONGLENGTH	256
#define	SHORTLENGTH	32
#define DOUONLEN	32
#define DOUON_N		257  /* 256 + 1 (Zenkaku Kana) */
#define SHORTBUFFSIZE	16
#define RSIZE		256

#define TCBUFSIZE	1024
#define AREASIZE	256

#define MAXLINE		256
#define MAXWORD		256
#define WORDN		32

#define KB_KANA		0
#define KB_ROMA		1


#define       MAXSERVER       16      

