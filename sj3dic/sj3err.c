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





#include <stdio.h>
#include "sj3err.h"

sj3error(fp, code)
FILE	*fp;
int	code;
{
	unsigned char	tmp[BUFSIZ];

	switch (code) {
#define	CASE(X, str)		case (X): printout(fp, str); break;
	CASE(SJ3_InternalError,		"\245\244\245\363\245\277\241\274\245\312\245\353\245\250\245\351\241\274");
	CASE(SJ3_NormalEnd,		"\300\265\276\357\275\252\316\273");
	CASE(SJ3_ServerDown,		"\245\265\241\274\245\320\241\274\244\254\315\356\244\301\244\306\244\244\244\336\244\271");
	CASE(SJ3_OpenSocket,		"socket \244\316 open \244\313\274\272\307\324\244\267\244\336\244\267\244\277");
	CASE(SJ3_ConnectSocket,		"socket \244\316 connect \244\313\274\272\307\324\244\267\244\336\244\267\244\277");
	CASE(SJ3_GetHostByName,		"gethostbyname \244\313\274\272\307\324\244\267\244\336\244\267\244\277");
	CASE(SJ3_NotOpened,		"\245\252\241\274\245\327\245\363\244\265\244\354\244\306\244\244\244\336\244\273\244\363");
	CASE(SJ3_NotEnoughMemory,	"\245\341\245\342\245\352\244\254\302\255\244\352\244\336\244\273\244\363");
	CASE(SJ3_IllegalCommand,	"\245\263\245\336\245\363\245\311\244\254\304\352\265\301\244\265\244\354\244\306\244\244\244\336\244\273\244\363");
	CASE(SJ3_DifferentVersion,	"\245\320\241\274\245\270\245\347\245\363\244\254\260\343\244\244\244\336\244\271");
	CASE(SJ3_NoHostName,		"\245\333\245\271\245\310\314\276\244\254\244\242\244\352\244\336\244\273\244\363");
	CASE(SJ3_NoUserName,		"\245\346\241\274\245\266\314\276\244\254\244\242\244\352\244\336\244\273\244\363");
	CASE(SJ3_NotAllowedUser,	"\300\334\302\263\244\362\265\366\244\265\244\354\244\306\244\244\244\336\244\273\244\363");
	CASE(SJ3_AlreadyConnected,	"\300\334\302\263\272\321\244\307\244\271");
	CASE(SJ3_NotConnected,		"\300\334\302\263\244\265\244\354\244\306\244\244\244\336\244\273\244\363");
	CASE(SJ3_TooLongParameter,	"\245\321\245\351\245\341\241\274\245\277\244\254\304\271\244\271\244\256\244\336\244\271");
	CASE(SJ3_IllegalParameter,	"\245\321\245\351\245\341\241\274\245\277\244\254\260\333\276\357\244\307\244\271");
	CASE(SJ3_BadDictID,		"\244\275\244\316\244\350\244\246\244\312\274\255\275\361\244\317\244\242\244\352\244\336\244\273\244\363");
	CASE(SJ3_IllegalDictFile,	"\274\255\275\361\245\325\245\241\245\244\245\353\244\254\260\333\276\357\244\307\244\271");
	CASE(SJ3_IllegalStdyFile,	"\263\330\275\254\245\325\245\241\245\244\245\353\244\254\260\333\276\357\244\307\244\271");
	CASE(SJ3_IncorrectPasswd,	"\245\321\245\271\245\357\241\274\245\311\244\254\260\333\244\312\244\352\244\336\244\271");
	CASE(SJ3_FileNotExist,		"\245\325\245\241\245\244\245\353\244\254\302\270\272\337\244\267\244\336\244\273\244\363");
	CASE(SJ3_CannotAccessFile,	"\245\325\245\241\245\244\245\353\244\313\245\242\245\257\245\273\245\271\244\307\244\255\244\336\244\273\244\363");
	CASE(SJ3_CannotOpenFile,	"\245\325\245\241\245\244\245\353\244\254\245\252\241\274\245\327\245\363\244\307\244\255\244\336\244\273\244\363");
	CASE(SJ3_CannotCreateFile,	"\245\325\245\241\245\244\245\353\244\254\272\356\300\256\244\307\244\255\244\336\244\273\244\363");
	CASE(SJ3_FileReadError,		"\245\352\241\274\245\311\245\250\245\351\241\274\244\254\244\242\244\352\244\336\244\267\244\277");
	CASE(SJ3_FileWriteError,	"\245\351\245\244\245\310\245\250\245\351\241\274\244\254\244\242\244\352\244\336\244\267\244\277");
	CASE(SJ3_FileSeekError,		"\245\267\241\274\245\257\245\250\245\351\241\274\244\254\244\242\244\352\244\336\244\267\244\277");
	CASE(SJ3_StdyAlreadyOpened,	"\263\330\275\254\245\325\245\241\245\244\245\353\244\317\245\252\241\274\245\327\245\363\244\265\244\354\244\306\244\244\244\336\244\271");
	CASE(SJ3_StdyFileNotOpened,	"\263\330\275\254\245\325\245\241\245\244\245\353\244\254\245\252\241\274\245\327\245\363\244\265\244\354\244\306\244\244\244\336\244\273\244\363");
	CASE(SJ3_TooSmallStdyArea,	"\312\270\300\341\304\271\263\330\275\254\316\316\260\350\244\254\276\256\244\265\244\271\244\256\244\336\244\271");
	CASE(SJ3_LockedByOther,		"\302\276\244\316\245\257\245\351\245\244\245\242\245\363\245\310\244\313\245\355\245\303\245\257\244\265\244\354\244\306\244\336\244\271");
	CASE(SJ3_NotLocked,		"\245\355\245\303\245\257\244\265\244\354\244\306\244\244\244\336\244\273\244\363");
	CASE(SJ3_NoSuchDict,		"\244\275\244\316\244\350\244\246\244\312\274\255\275\361\244\317\244\242\244\352\244\336\244\273\244\363");
	CASE(SJ3_ReadOnlyDict,		"\275\361\244\255\271\376\244\337\244\307\244\255\244\312\244\244\274\255\275\361\244\307\244\271");
	CASE(SJ3_DictLocked,		"\274\255\275\361\244\254\245\355\245\303\245\257\244\265\244\354\244\306\244\244\244\336\244\271");
	CASE(SJ3_BadYomiString,		"\265\366\244\265\244\354\244\312\244\244\306\311\244\337\312\270\273\372\316\363\244\307\244\271");
	CASE(SJ3_BadKanjiString,	"\265\366\244\265\244\354\244\312\244\244\264\301\273\372\312\270\273\372\316\363\244\307\244\271");
	CASE(SJ3_BadHinsiCode,		"\311\312\273\354\245\263\241\274\245\311\260\333\276\357\244\307\244\271");
	CASE(SJ3_AddDictFailed,		"\274\255\275\361\305\320\317\277\244\362\274\272\307\324\244\267\244\336\244\267\244\277");
	CASE(SJ3_AlreadyExistWord,	"\244\271\244\307\244\313\302\270\272\337\244\267\244\306\244\244\244\336\244\271");
	CASE(SJ3_NoMoreDouonWord,	"\244\263\244\354\260\312\276\345\306\261\262\273\270\354\244\362\305\320\317\277\244\307\244\255\244\336\244\273\244\363");
	CASE(SJ3_NoMoreUserDict,	"\244\263\244\354\260\312\276\345\274\255\275\361\244\313\305\320\317\277\244\307\244\255\244\336\244\273\244\363");
	CASE(SJ3_NoMoreIndexBlock,	"\244\263\244\354\260\312\276\345\245\244\245\363\245\307\245\303\245\257\245\271\244\313\305\320\317\277\244\307\244\255\244\336\244\273\244\363");
	CASE(SJ3_DelDictFailed,		"\274\255\275\361\272\357\275\374\244\362\274\272\307\324\244\267\244\336\244\267\244\277");
	CASE(SJ3_NoSuchWord,		"\244\275\244\316\244\350\244\246\244\312\275\317\270\354\244\317\244\242\244\352\244\336\244\273\244\363");
	CASE(SJ3_DirAlreadyExist,	"\244\275\244\316\245\307\245\243\245\354\245\257\245\310\245\352\244\254\302\270\272\337\244\267\244\306\244\244\244\336\244\271");
	CASE(SJ3_CannotCreateDir,	"\245\307\245\243\245\354\245\257\245\310\245\352\244\254\272\356\300\256\244\307\244\255\244\336\244\273\244\363");
	CASE(SJ3_NoMoreDictData,	"\244\263\244\354\260\312\276\345\274\255\275\361\245\307\241\274\245\277\244\254\244\242\244\352\244\336\244\273\244\363");
	CASE(SJ3_UserConnected,		"\300\334\302\263\244\267\244\306\244\244\244\353\245\346\241\274\245\266\244\254\244\242\244\352\244\336\244\271");
	CASE(SJ3_TooLongPasswd,		"\245\321\245\271\245\357\241\274\245\311\244\254\304\271\244\271\244\256\244\336\244\271");
	CASE(SJ3_TooLongComment,	"\245\263\245\341\245\363\245\310\244\254\304\271\244\271\244\256\244\336\244\271");
#undef	CASE
	default:
		sprintf((char *)tmp, "\314\244\304\352\265\301\244\316\245\250\245\351\241\274(%d)", code);
		printout(fp, tmp);
		break;
	}
	printout(fp, "\r\n");
}
