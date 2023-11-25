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
 * $SonyRCSfile: Struct.h,v $  
 * $SonyRevision: 1.3 $ 
 * $SonyDate: 1994/10/21 05:13:34 $
 */




typedef struct global {

	

	JREC	*Jmaxjptr;	
	CLREC	*Jmaxclptr;	
	JREC	*Jjrt1st;	
	CLREC	*Jclt1st;	
	JREC	*Jjrt2nd;	
	CLREC	*Jclt2nd;	
	CLREC	*Jselcl;	

	

	Short	Jselectid;
	Short	Jtrank;
	Short	Jnrank;		

	

	Uchar	*Jinputyomi;		
	Uchar	*Jcnvstart;		
	Short	Jcnvlen;		
	Uchar	*Jystart;		
	Uchar	Jhyomi[MaxPhInputLen+1];	
	Uchar	Jorgyomi[MaxClInputLen*2+1];	


	

	Uchar	Jkanjibuf[MinPhOutputLen];	
	Uchar	*Jkanjipos;		
	Uchar	*Jkanjitmp;		
	Short	Jkanjilen;		


	

	Uchar		Jdicinl;	
	Uchar		Jdicsaml;	
	TypeDicSeg	Jprevseg;	


	

	Uchar	Jfzkcount;		
	Uchar	Jfzk_ka_flg;		
	FREC	Jfzktbl[MAXFREC];	


	

	Uchar	Jheadcode;		
	Uchar	Jheadlen;		


	

	Uchar	Jgobilen;		


	

	TypeGram	Jprevclgrm;		
	TypeCnct	Jprevclrow;		
	Uchar		Jnextcllen;		


	

	Short	Jkhcount;			
	Short	Jnkhcount;			
	KHREC	Jkouhotbl[MaxKouhoNumber];	
	STDYIN	*Jstdytop;


	

	DICT		*Jcurdict;		
	DICTL		*Jdictlist;		
	Uchar		*Jdicbuf;
	Uchar		*Jidxbuf;
	TypeIdxOfs	*Jidxofs;
	Uchar	*Jaskknj[MaxKnjAskNumber];	
	Uchar	*Jaskknj_k[MaxKnjAskNumber];	

	STDY	*Jcurstdy;

	

	Ushort		Jsuuji_wbuf[NumWordBuf];
	Uchar		Jsuuji_ubuf[NumKetaLength];
	TypeClass	Jsuuji_class;
	Uchar		Jsuuji_comma;
	Uchar		Jsuuji_keta;
	Uchar		*Jsuuji_yptr;
	Uchar		Jsuuji_len;
	Uchar		Jsuuji_exit;
	Ushort		*Jsuuji_wkeep;
	Uchar		*Jsuuji_ukeep;


	

	Uchar		Jpeepyomi[MaxWdYomiLen * 2 + 1];
	Uchar		Jpeepknj[MaxWdKanjiLen + 1];
	TypeGram	Jpeepgrm;
	Uchar		*Jpeepdptr;
	Uchar		*Jpeephptr;
	Uchar		*Jpeepkptr;
	TypeDicSeg	Jpeepidx;

} Global;




typedef	struct	dictfile {
	DICT		dict;		
	int		refcnt;		
	fd_set		lock;		
	FILE		*fp;		
	int		fd;		

	Uchar		*buffer;	
	long		bufsiz;		

	long		idxstrt;	
	long		segstrt;	

	TypeIdxOfs	*ofsptr;	

	struct dictfile	*link;		
} DictFile;




typedef struct	stdyfile {
	STDY		stdy;		
	int		refcnt;		
	ino_t		inode;		
	FILE		*fp;		
	int		fd;		

	Uchar		*header;	

	struct stdyfile	*link;		
} StdyFile;




typedef	struct workarea {
	int	refcnt;			
	int	lock;			

	struct workarea	*link;		

	Global		global;		
} WorkArea;




typedef	struct client {
	int		fd;			
	char		host[HostNameLen];	
	char		user[UserNameLen];	
	char		prog[ProgNameLen];

	WorkArea	*work;			
	StdyFile	*stdy;			
	short           version;                
	unsigned char   def_char[2];            
#if (defined(TLI) && defined(SOCK_UNIX))
	int             unix_flag;
#endif
	uid_t           uid;
	struct client   *next;
} Client;
