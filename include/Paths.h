/*
 * Copyright (c) 1997  Sony Corporation
 * Copyright (c) 1996  Hidekazu Kuroki <hidekazu at cs.titech.ac.jp>
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
 * $SonyRCSfile: Paths.h.in,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1997/01/23 11:20:55 $
 */

/*
 * Change ErrotOutFile variable.
 * Because suppress a error when sj3serv start up, change to "/dev/stderr".
 * This error is "Cannot Open "/dev/console".". 
 * Patched by Hidekazu Kuroki(hidekazu@cs.titech.ac.jp)		1996/8/10
 *
 * Change ErrotOutFile variable.
 * This default value is "/dev/null" in document.
 * Patched by Hidekazu Kuroki(hidekazu@cs.titech.ac.jp)		1996/8/11
 */
#if !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__) && !defined(__DragonFly__)
#define	ErrorOutFile		"/dev/console"
#else
#define	ErrorOutFile		"/dev/stderr"
/*#define	ErrorOutFile		"/dev/null"*/
#endif

#define SocketName              "/tmp/sj3sock"
#define LockFile                "/tmp/SJ3SERV.EXIST"

#define MainDictionary          "sj3main.dic"
#define UserDictionary          "private.dic"
#define StudyFile               "study.dat"
#define RunCmdFile "/usr/local/lib/sj3/serverrc"
#define DictRootDir "/usr/local/share/sj3/dict"

#define DEFRKFILE "/usr/local/lib/sj3/sjrk"
#define DEFRCFILE "/usr/local/lib/sj3/sjrc"
#define DEFKEYFILE "/usr/local/lib/sj3/cvtkey."