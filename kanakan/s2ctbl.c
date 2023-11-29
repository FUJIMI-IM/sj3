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




#include "sj_rename.h"
#include "sj_typedef.h"
#include "sj_yomi.h"

Uchar	kigou[] = {
		Y_L_KAKKO,
		Y_KUTEN,
		Y_TOUTEN,
		Y_COMMA,
		Y_PERIOD,
		Y_L_KAKKO,
		Y_L_KAKKO,
		Y_L_KAKKO,
		Y_BIKKURI,
		Y_BIKKURI,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		_TYOUON,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_L_KAKKO,
		Y_QUOTA,
		Y_L_KAKKO,
		Y_QUOTA,
		Y_L_KAKKO,
		Y_R_KAKKO,
		Y_L_KAKKO,
		Y_R_KAKKO,
		Y_L_KAKKO,
		Y_R_KAKKO,
		Y_L_KAKKO,
		Y_R_KAKKO,
		Y_L_KAKKO,
		Y_R_KAKKO,
		Y_L_KAKKO,
		Y_R_KAKKO,
		Y_L_KAKKO,
		Y_R_KAKKO,
		Y_L_KAKKO,
		Y_R_KAKKO,
		Y_L_KAKKO,
		Y_R_KAKKO,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_S_KIGOU2,
		Y_S_KIGOU1,
		Y_S_KIGOU1,
		Y_S_KIGOU2,
		Y_S_KIGOU1,
		Y_S_KIGOU2,
		_IGETA,
		Y_KIGOU,
		Y_KIGOU,
		_ATTO,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		_YUUBIN,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU,
		Y_KIGOU
	};
