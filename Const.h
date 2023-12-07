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
 * $SonyRCSfile: Const.h,v $  
 * $SonyRevision: 1.4 $ 
 * $SonyDate: 1997/01/23 11:20:52 $
 *
 * $Id$
 */

/*
 * Change some default values.
 * Patched by Hidekazu Kuroki(hidekazu@cs.titech.ac.jp)		1996/8/10
 */

#ifndef _Const_H_
#define _Const_H_  1

#include "Paths.h"

#define	HostNameLen		128			
#define	UserNameLen		128
#define	ProgNameLen		128
#define	PathNameLen		1024			
#define	PassWordLen		16

#define	YomiBufSize		1024
#define	KanjiBufSize		2048

#define	MinIdxLen		512
#define	MinSegLen		512
#define	MinSegNum		1
#define	MinStyNum		512
#define	MaxStyNum		0x7fff
#define	MinClStep		1
#define	MaxClStep		256
#define	MinClLen		512
#define	MaxClLen		0x7fff

#define	DebugOutFile		ErrorOutFile
#define	DebugLevel		0
#define	ForkFlag		1
#define	MaxClientNum		512
#define	LogOutFile		ErrorOutFile
#define	PortName		"sj3"
#ifdef TLI
#define LocalHost               "localhost"
#define ProtoName               "tcp"
#define	PortNumber		"3000"
#else
#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
#define	PortNumber		3086
#else /* ! __FreeBSD__ __NetBSD__ __OpenBSD__ __DragonFly__ */
#define	PortNumber		3000
#endif /* ! __FreeBSD__ __NetBSD__ __OpenBSD__ __DragonFly__ */
#endif
#define	DirectryMode		0755
#define	DictFileMode		0644
#define	MODE_MASK		0777

#define	DefIdxLen		2048
#define	DefSegLen		2048
#define	DefSegNum		256
#define	DefStyNum		2048
#define	DefClStep		1
#define	DefClLen		2048

#define	NotLocked		-1

#define	ERROR			-1

#ifndef	FALSE
#	define	TRUE		-1
#	define	FALSE		(!TRUE)
#endif

#endif /* _Const_H_ */
