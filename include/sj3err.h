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
 * $SonyRCSfile: sj3err.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:00:58 $
 */





#define	SJ3_InternalError	-1

#define	SJ3_NormalEnd		0

#define	SJ3_ServerDown		1
#define	SJ3_OpenSocket		2
#define	SJ3_ConnectSocket	3
#define	SJ3_GetHostByName	4
#define	SJ3_NotOpened		5
#define	SJ3_NotEnoughMemory	6
#define	SJ3_IllegalCommand	7

#define	SJ3_DifferentVersion	11
#define	SJ3_NoHostName		12
#define	SJ3_NoUserName		13
#define	SJ3_NotAllowedUser	14
#define	SJ3_AlreadyConnected	15
#define	SJ3_NotConnected	16

#define	SJ3_TooLongParameter	21
#define	SJ3_IllegalParameter	22

#define	SJ3_BadDictID		31
#define	SJ3_IllegalDictFile	32
#define	SJ3_IllegalStdyFile	33
#define	SJ3_IncorrectPasswd	34
#define	SJ3_FileNotExist	35
#define	SJ3_CannotAccessFile	36
#define	SJ3_CannotOpenFile	37
#define	SJ3_CannotCreateFile	38
#define	SJ3_FileReadError	39
#define	SJ3_FileWriteError	40
#define	SJ3_FileSeekError	41

#define	SJ3_StdyAlreadyOpened	51
#define	SJ3_StdyFileNotOpened	52
#define	SJ3_TooSmallStdyArea	53

#define	SJ3_LockedByOther	61
#define	SJ3_NotLocked		62

#define	SJ3_NoSuchDict		71
#define	SJ3_ReadOnlyDict	72
#define	SJ3_DictLocked		73
#define	SJ3_BadYomiString	74
#define	SJ3_BadKanjiString	75
#define	SJ3_BadHinsiCode	76

#define	SJ3_AddDictFailed	81
#define	SJ3_AlreadyExistWord	82
#define	SJ3_NoMoreDouonWord	83
#define	SJ3_NoMoreUserDict	84
#define	SJ3_NoMoreIndexBlock	85

#define	SJ3_DelDictFailed	91
#define	SJ3_NoSuchWord		92

#define	SJ3_DirAlreadyExist	101
#define	SJ3_CannotCreateDir	102

#define	SJ3_NoMoreDictData	111

#define	SJ3_UserConnected	121

#define	SJ3_TooLongPasswd	131
#define	SJ3_TooLongComment	132

#define SJ3_CannotCodeConvert   133
