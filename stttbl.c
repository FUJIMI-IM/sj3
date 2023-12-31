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
 * $SonyRCSfile: stttbl.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:37 $
 */




#include "sj_rename.h"
#include "sj_typedef.h"



static	unsigned char	o_settou[] = {
	0xA1,	0xb8, 0xe6,	0xa4, 0xaa
};

static	unsigned char	go_settou[] = {
	0xA1,	0xb8, 0xe6,	0xa4, 0xb4
};

static	unsigned char	dai_settou[] = {
	0x22,	0xc2, 0xe8,	0xc2, 0xe8
};

static	unsigned char	kigou_settou[] = {
	0x01
};



unsigned char	*settou_ptr[] = {
	NULL, o_settou, go_settou, dai_settou, kigou_settou
};



unsigned char	scncttbl[2][24] = {
	{	
 	     
		 0x48,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,

	     
		 0x18,  0x18,  0x00,  0x00,  0x1f,  0xf0,  0x00,  0xff,

	     
		 0x00,  0x1f,  0xf0,  0x00,  0xff,  0x00,  0x00,  0x00
	},

	{	
 	     
		 0x22,  0x40,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,

	     
		 0x00,  0x04,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,

	     
		 0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00
	}
};
