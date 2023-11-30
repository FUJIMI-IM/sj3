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





#include "sj_euc.h"
#include "sj_kcnv.h"
#include "sj_yomi.h"

Int	sj2cd_chr(euc, yomi)
Uchar	*euc;			
Reg2	Uchar	*yomi;		
{
	Reg1	Uchar	chr;

	if ((chr = *euc++) == EUC_a1) {
		chr = *euc;
		if (chr < EUC_START)
			*yomi = Y_ZENKAKU;
		else if (chr <= EUC_END)
			*yomi = Kigou[chr - EUC_START];
		else
			*yomi = Y_ZENKAKU;
	} else if (chr == EUC_a2) {
		chr = *euc;

		if (chr < EUC_START)
			*yomi = Y_ZENKAKU;
		else if (chr <= EUC_END_KIGOU)
			*yomi = Kigou[chr - EUC_START + 94];
		else
			*yomi = Y_ZENKAKU;
	} else if (chr == EUC_a3) {
		chr = *euc;
		if (chr < EUC_0)
		        *yomi = Y_ZENKAKU;
		else if (chr <= EUC_END_0)
		        *yomi = chr - EUC_0 + N_0; 
                else if (chr < EUC_A)
		        *yomi = Y_ZENKAKU;
		else if (chr <= EUC_END_A)
			*yomi = chr - EUC_A + A_A; 
		else if (chr < EUC_a)
                        *yomi = Y_ZENKAKU; 
		else if (chr <= EUC_END_a)
			*yomi = chr - EUC_a + A_a; 
		else 
		        *yomi = Y_ZENKAKU; 
	} else if (chr == EUC_a4) {
		chr = *euc;
		if (chr < EUC_START)
			*yomi = Y_ZENKAKU;
		else if (chr <= EUC_END_HIRA)
			*yomi = chr - EUC_START + _XA; 
		else
			*yomi = Y_ZENKAKU;
	} else if (chr == EUC_a5) {
		chr = *euc;
		if (chr == EUC_VU)
			*yomi = _VU;
		else if (chr == EUC_XKA)
			*yomi = _XKA;
		else if (chr == EUC_XKE)
			*yomi = _XKE;
		else
			*yomi = Y_ZENKAKU;
	}
	else {
		
		if (!(chr & 0x80)) {
			*yomi = Y_HANKAKU;
			return 1;
		} else if (chr == SS2) {
			*yomi = Y_HANKAKU;
		} else if (chr == SS3) {
			*yomi = Y_ZENKAKU;
			return 3;
		} else 
		        *yomi = Y_ZENKAKU;
	}

	return 2;
}



Int	sj2cd_str(euc, yomi, len)
Reg1	Uchar	*euc;	
Reg2	Uchar	*yomi;	
Reg3	Int	len;	
{
	
	if (!len--) return FALSE;

	
	while (*euc && len--) euc += sj2cd_chr(euc, yomi++);
	*yomi = 0;

	return *euc ? FALSE : TRUE;
}
