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

#include "wchar16.h"

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




#define MODE_ZEN	0x10		
#define MODE_CONV	0x04		
#define MODE_ALPHA	0x02		
#define MODE_KATA	0x01		
#define MODE_CODE	0x08 + MODE_ZEN	
#define CODE_SJIS	0		
#define CODE_JIS	1		
#define CODE_JIS2       2               
#define CODE_EUC	3		
#define CODE_EUC2       4               
#define CODE_KUTEN	5		
#define CODE_KUTEN2	6		
#define OMODE_ZEN	0x80		
#define OMODE_MASK	0x1f		

#define MODE_HALPHA	MODE_ALPHA		
#define MODE_ZALPHA	MODE_ZEN + MODE_ALPHA	
#define MODE_HKATA	MODE_CONV		
#define MODE_ZHIRA	MODE_ZEN + MODE_CONV	
#define MODE_ZKATA	MODE_ZHIRA + MODE_KATA	



#ifdef NOTDEF	
#define	OTHER	0
#define	NEWS	1
#define	ONEWS	2
#define	JTERM	3
#define OJTERM	4
#define	NWP511	5
#define	NWP517	6
#endif 

#define OTHER_KB	0
#define NEWS_KB		1

#define MINCOL	72



#define STDIN	0
#define STDOUT	1



#define BS	'\010'
#define ESC	'\033'
#define DEL	'\177'



#define UNBUFF	0
#define BUFF	1



/*
 * Remove warning.
 * Patched by Hidekazu Kuroki(hidekazu@cs.titech.ac.jp)		1996/8/10
 */
#if defined(TRUE) && defined(FALSE)
#if (TRUE != 0) || (TRUE != -1)
#undef TRUE
#undef FALSE
#define TRUE	0
#define FALSE	-1
#endif
#else
#define TRUE	0
#define FALSE	-1
#endif


struct	wordent {
		wchar16_t word_str[MAXWORD];
	};


struct valtbl {
	wchar16_t *keyword;
	int  ivalue;
};

#define aprintf	printf
