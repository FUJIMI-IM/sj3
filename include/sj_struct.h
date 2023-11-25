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
 */





#ifndef	_SJ_STRUCT

#define	_SJ_STRUCT



typedef	struct	jiritu {
	struct jiritu	*jsort;		
	TypeDicSeg	jseg;		
	TypeDicOfs	jofsst;		
	TypeDicOfs	jofsed;		
	Ushort		flags;		
	TypeClass	class;		
	TypeDicID	dicid;		
	Uchar		jlen;		
	TypeGram	hinsi;		
	Uchar		sttofs;		
	Uchar		stbofs;		
	Uchar		count;		
	Uchar		numlen;		
} JREC;




typedef struct bunsetu {
	JREC	*jnode;			
	struct	bunsetu	*clsort;	
	Uchar		gobiln;		
	Uchar		cllen;		

	TypeCnct	right;		
	Uchar		cl2len;		
	Uchar		kubun;		
#if __STDC__
	unsigned	fzk_ka : 1;	
#else
	Uchar		fzk_ka : 1;	
#endif
} CLREC;




typedef	struct	kouho {
	CLREC		*clrec;		
	TypeDicOfs	offs;		
	TypeStyNum	styno;		
	Uchar		rank;		
#if __STDC__
	unsigned	sttfg  :  1;	
	unsigned	sttkj  :  1;	
	unsigned	ka_fg  :  1;	
	unsigned	ka_kj  :  1;	
	unsigned	mode   :  4;	
#else
	Uchar		sttfg  :  1;	
	Uchar		sttkj  :  1;	
	Uchar		ka_fg  :  1;	
	Uchar		ka_kj  :  1;	
	Uchar		mode   :  4;	
#endif
} KHREC;




typedef	struct	conj {
	Uchar		len;		
	TypeCnct	right;		
} CREC;




typedef	struct	fuzoku {
	Uchar		*yomip;		
	Uchar	TFar	*fzkp;		

} FREC;



typedef struct study_in {
	TypeDicOfs	offset;		
	TypeDicSeg	seg;		
	TypeStyNum	styno;		
	TypeDicID	dicid;		
#if __STDC__
	unsigned	sttkj  :  1;	
	unsigned	ka_kj  :  1;	
	unsigned	nmflg  :  1;	
#else
	Uchar		sttkj  :  1;	
	Uchar		ka_kj  :  1;	
	Uchar		nmflg  :  1;	
#endif
} STDYIN;




typedef struct study_out {
	STDYIN		stdy1;		
	TypeGram	hinshi;		
	Uchar		len;		
#if __STDC__
	unsigned	sttlen :  2;	
	unsigned	sttfg  :  1;	
	unsigned	ka_fg  :  1;	
#else
	Uchar		sttlen :  2;	
	Uchar		sttfg  :  1;	
	Uchar		ka_fg  :  1;	
#endif
} STDYOUT;




typedef	struct	dict {
	TypeDicID	dicid;		

	TypeIdxOfs	idxlen;		

	TypeDicOfs	seglen;		
	TypeDicSeg	segunit;	
	TypeDicSeg	maxunit;	

	IFunc		getofs;		
	IFunc		getidx;		
	IFunc		getdic;		
	IFunc		putidx;		
	IFunc		putdic;		
        IFunc           rszdic;
} DICT;




typedef	struct	dictl {
	DICT		*dict;
	struct dictl	*next;
} DICTL;




typedef struct	stdy {
	Short	stdycnt;	
	Short	stdymax;	
	STDYIN	*stdydic;	

	Short	clstdystep;	
	Ushort	*clstdyidx;	

	Short	clstdylen;	
	Uchar	*clstdydic;	
} STDY;

#endif
