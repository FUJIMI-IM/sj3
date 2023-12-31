/*
 * Copyright (c) 2004  Hiroo Ono <hiroo+sj3 at oikumene.gcd.org>
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
 * $Id: sj_func.h,v 1.1 2004/07/04 08:15:19 hiroo Exp $
 */


#ifndef	_SJ_FUNC
#define	_SJ_FUNC  1

#include <sys/types.h>
#include "sj_typedef.h"
#include "sj_struct.h"
#include "Struct.h"

typedef	void	(*VFuncNT)(unsigned char*, unsigned char*, JREC*);
typedef int	(*IFuncSetj)(unsigned char*);
typedef int	(*IFuncHira)(unsigned char*, int*);
typedef unsigned char* (*UCPFuncGK)(unsigned char*, unsigned char*, unsigned char*, int, int);
typedef unsigned char* (*UCPFuncMK)(unsigned char*, unsigned char*, int);


#endif /* _SJ_FUNC */

