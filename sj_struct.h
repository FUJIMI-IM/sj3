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
 * $SonyRCSfile: sj_struct.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:16 $
 *
 * $Id$
 */


#ifndef	_SJ_STRUCT
#define	_SJ_STRUCT  1

#include <sys/types.h>
#include "sj_typedef.h"

typedef	struct	jrec {
	struct jrec	*jsort;
	TypeDicSeg	jseg;		
	TypeDicOfs	jofsst;		
	TypeDicOfs	jofsed;		
	unsigned short	flags;
	TypeClass	class;		
	TypeDicID	dicid;		
	unsigned char	jlen;
	TypeGram	hinsi;		
	unsigned char	sttofs;
	unsigned char	stbofs;
	unsigned char	count;
	unsigned char	numlen;
} JREC;




typedef struct clrec {
	JREC	*jnode;			
	struct	clrec	*clsort;
	unsigned char	gobiln;
	unsigned char	cllen;

	TypeCnct	right;		
	unsigned char	cl2len;
	unsigned char	kubun;
#if __STDC__
	unsigned int	fzk_ka : 1;
#else
	unsigned char	fzk_ka : 1;
#endif
} CLREC;




typedef	struct	khrec {
	CLREC		*clrec;		
	TypeDicOfs	offs;		
	TypeStyNum	styno;		
	unsigned char	rank;
#if __STDC__
	unsigned int	sttfg  :  1;
	unsigned int	sttkj  :  1;
	unsigned int	ka_fg  :  1;
	unsigned int	ka_kj  :  1;
	unsigned int	mode   :  4;
#else
	unsigned char	sttfg  :  1;
	unsigned char	sttkj  :  1;
	unsigned char	ka_fg  :  1;
	unsigned char	ka_kj  :  1;
	unsigned char	mode   :  4;
#endif
} KHREC;




typedef	struct	crec {
	unsigned char	len;
	TypeCnct	right;		
} CREC;




typedef	struct	frec {
	unsigned char	*yomip;
	unsigned char	*fzkp;

} FREC;



typedef struct stdyin {
	TypeDicOfs	offset;		
	TypeDicSeg	seg;		
	TypeStyNum	styno;		
	TypeDicID	dicid;		
#if __STDC__
	unsigned int	sttkj  :  1;
	unsigned int	ka_kj  :  1;
	unsigned int	nmflg  :  1;
#else
	unsigned char	sttkj  :  1;
	unsigned char	ka_kj  :  1;
	unsigned char	nmflg  :  1;
#endif
} STDYIN;




typedef struct stdyout {
	STDYIN		stdy1;		
	TypeGram	hinshi;		
	unsigned char	len;
#if __STDC__
	unsigned int	sttlen :  2;
	unsigned int	sttfg  :  1;
	unsigned int	ka_fg  :  1;
#else
	unsigned char	sttlen :  2;
	unsigned char	sttfg  :  1;
	unsigned char	ka_fg  :  1;
#endif
} STDYOUT;


struct dictfile; /* defined later in Struct.h refering DICT */
typedef int	(*IFuncDF)(struct dictfile*);
typedef int	(*IFuncDFTD)(struct dictfile*, TypeDicSeg);

typedef	struct	dict {
	TypeDicID	dicid;		

	TypeIdxOfs	idxlen;		

	TypeDicOfs	seglen;		
	TypeDicSeg	segunit;	
	TypeDicSeg	maxunit;	

	IFuncDF		getofs;		
	IFuncDF		getidx;		
	IFuncDFTD	getdic;		
	IFuncDFTD	putidx;		
	IFuncDFTD	putdic;		
	IFuncDFTD	rszdic;
} DICT;




typedef	struct	dictl {
	DICT		*dict;
	struct dictl	*next;
} DICTL;




typedef struct	stdy {
	short	stdycnt;	
	short	stdymax;	
	STDYIN	*stdydic;	

	short	clstdystep;	
	unsigned short	*clstdyidx;

	short	clstdylen;	
	unsigned char	*clstdydic;
} STDY;

#endif
