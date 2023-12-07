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
 * $SonyRCSfile: Dict.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:00:55 $
 */




#define	VersionPos		0	

#define	PasswdPos		16	
#define	PasswdLen		16	


#define	StdyNormPos		32	
#define	StdyNormLen		36	
#define	StdyNormNum		40	
#define	StdyNormCnt		44	

#define	StdyClIdxPos		48	
#define	StdyClIdxLen		52	
#define	StdyClIdxStep		56	

#define	StdyClSegPos		64	
#define	StdyClSegLen		68	


#define	DictIdxPos		32	
#define	DictIdxLen		36	

#define	DictSegPos		48	
#define	DictSegLen		52	
#define	DictSegNum		56	
#define	DictSegMax		60	

#define	DictAIdxPos		64	
#define	DictAIdxLen		68	

#define	DictASegPos		80	
#define	DictASegLen		84	


#define	HeaderPos		0	
#define	HeaderLength		128	
#define	CommentPos		128	
#define	CommentLength		128	


#define	Byte2Long(c3,c2,c1,c0)	(((c3) << (8*3)) | ((c2) << (8*2)) | \
				 ((c1) << (8*1)) | ((c0) << (8*0)))
#define	StdyVersion		Byte2Long('S', 2, 0, 0)
#define	DictVersion		Byte2Long('D', 2, 0, 0)
