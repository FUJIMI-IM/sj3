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
 * $SonyRCSfile: sj3cmd.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:00:57 $
 */







#define	SJ3SERV_VERSION_NO	2



#define	SJ3_CONNECT	1	
#define	SJ3_DISCONNECT	2	

#define	SJ3_OPENDICT	11	
#define	SJ3_CLOSEDICT	12	

#define	SJ3_OPENSTDY	21	
#define	SJ3_CLOSESTDY	22	
#define	SJ3_STDYSIZE	23	

#define	SJ3_LOCK	31	
#define	SJ3_UNLOCK	32	

#define	SJ3_PH2KNJ	41	

#define	SJ3_CL2KNJ	51	
#define	SJ3_NEXTCL	52	
#define	SJ3_PREVCL	53	
#define	SJ3_CL2KNJ_ALL	54	
#define	SJ3_CL2KNJ_CNT	55	

#define	SJ3_STUDY	61	
#define	SJ3_CLSTUDY	62	

#define	SJ3_ADDDICT	71	
#define	SJ3_DELDICT	72	

#define	SJ3_MAKEDICT	81	
#define	SJ3_MAKESTDY	82	
#define	SJ3_MAKEDIR	83	
#define	SJ3_ACCESS	84	

#define	SJ3_GETDICT	91	
#define	SJ3_NEXTDICT	92	
#define	SJ3_PREVDICT	93	

#define	SJ3_WHO		100	
#define	SJ3_QUIT	101	
#define	SJ3_KILL	102	
#define	SJ3_VERSION	103	
#define	SJ3_DICTPASS	104	
#define	SJ3_DICTCMNT	105	
#define	SJ3_STDYPASS	106	
#define	SJ3_STDYCMNT	107	

#define SJ3_SETCVER     110     
#define	SJ3_PH2KNJ_EUC	111	

#define	SJ3_CL2KNJ_EUC	112	
#define	SJ3_NEXTCL_EUC	113	
#define	SJ3_PREVCL_EUC	114	
#define	SJ3_CL2KNJ_ALL_EUC	115	
#define	SJ3_CL2KNJ_CNT_EUC	116	

#define	SJ3_CLSTUDY_EUC	117	

#define	SJ3_ADDDICT_EUC	118	
#define	SJ3_DELDICT_EUC	119	

#define	SJ3_GETDICT_EUC	120	
#define	SJ3_NEXTDICT_EUC	121	
#define	SJ3_PREVDICT_EUC	122	
