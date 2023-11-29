/*-
 * SPDX-License-Identifier: MIT-open-group
 *
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
 */



#ifndef	_SJ_TYPEDEF
#define	_SJ_TYPEDEF  1

#define	RECURS

#define	TFar
#define	Far
#define	Reg1	register
#define	Reg2	register
#define	Reg3	register
#define	Reg4	register
#define	Reg5	register
#define	Reg6	register
#define	Reg7	register
#define	Reg8	register

#define	Void	void
#define	Char	char
#define	Uchar	unsigned char
#define	Short	short
#define	Ushort	unsigned short
#define	Int	int
#define	Uint	unsigned int
#define	Long	long
#define	Ulong	unsigned long


#include <sys/types.h>

typedef	u_char	TypeGroup;
typedef	u_char	TypeGram;
typedef	u_short	TypeIdxOfs;
typedef	short	TypeDicSeg;
typedef	u_short	TypeDicOfs;
typedef	ino_t	TypeDicID;
typedef	u_char	TypeCnct;
typedef	u_short	TypeStyNum;
typedef	u_char	TypeClass;
typedef	u_char	TypeBunNum;

#ifndef	NULL
#	define	NULL	0
#endif

#endif /* _SJ_TYPEDEF */

