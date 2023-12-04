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

typedef	struct	jiritu {
	struct jiritu	*jsort;		
	TypeDicSeg	jseg;		
	TypeDicOfs	jofsst;		
	TypeDicOfs	jofsed;		
	u_short		flags;		
	TypeClass	class;		
	TypeDicID	dicid;		
	u_char		jlen;		
	TypeGram	hinsi;		
	u_char		sttofs;		
	u_char		stbofs;		
	u_char		count;		
	u_char		numlen;		
} JREC;




typedef struct bunsetu {
	JREC	*jnode;			
	struct	bunsetu	*clsort;	
	u_char		gobiln;		
	u_char		cllen;		

	TypeCnct	right;		
	u_char		cl2len;		
	u_char		kubun;		
#if __STDC__
	u_int		fzk_ka : 1;	
#else
	u_char		fzk_ka : 1;	
#endif
} CLREC;




typedef	struct	kouho {
	CLREC		*clrec;		
	TypeDicOfs	offs;		
	TypeStyNum	styno;		
	u_char		rank;		
#if __STDC__
	u_int		sttfg  :  1;	
	u_int		sttkj  :  1;	
	u_int		ka_fg  :  1;	
	u_int		ka_kj  :  1;	
	u_int		mode   :  4;	
#else
	u_char		sttfg  :  1;	
	u_char		sttkj  :  1;	
	u_char		ka_fg  :  1;	
	u_char		ka_kj  :  1;	
	u_char		mode   :  4;	
#endif
} KHREC;




typedef	struct	conj {
	u_char		len;		
	TypeCnct	right;		
} CREC;




typedef	struct	fuzoku {
	u_char		*yomip;		
	u_char		*fzkp;		

} FREC;



typedef struct study_in {
	TypeDicOfs	offset;		
	TypeDicSeg	seg;		
	TypeStyNum	styno;		
	TypeDicID	dicid;		
#if __STDC__
	u_int		sttkj  :  1;	
	u_int		ka_kj  :  1;	
	u_int		nmflg  :  1;	
#else
	u_char		sttkj  :  1;	
	u_char		ka_kj  :  1;	
	u_char		nmflg  :  1;	
#endif
} STDYIN;




typedef struct study_out {
	STDYIN		stdy1;		
	TypeGram	hinshi;		
	u_char		len;		
#if __STDC__
	u_int		sttlen :  2;	
	u_int		sttfg  :  1;	
	u_int		ka_fg  :  1;	
#else
	u_char		sttlen :  2;	
	u_char		sttfg  :  1;	
	u_char		ka_fg  :  1;	
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
	u_short	*clstdyidx;	

	short	clstdylen;	
	u_char	*clstdydic;	
} STDY;

#endif
