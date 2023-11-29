/*-
 * SPDX-License-Identifier: MIT-open-group
 *
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
 */




#ifndef _SJ_SYSVDEF
#define _SJ_SYSVDEF

#ifdef SYSV
#ifndef SVR4
#define SVR4
#endif 
#endif 

#ifdef SYSTYPE_SYSV
#ifndef SVR4
#define SVR4
#endif 
#endif 

#ifdef SVR4
#define SYSV_TERMIOS
#define SIGTYPE_VOID
#endif /* SVR4 */

#ifdef linux
#if !(defined (__GLIBC__) && (__GLIBC__ >= 2))
#define LACKOF_SETLOCALE
#endif
#define SYSV_TERMIOS
#define SYSV_UTMP
#define SIGTYPE_VOID
#endif /* linux */

#endif 
